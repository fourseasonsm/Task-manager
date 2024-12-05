from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import psycopg2
import hashlib

# Хранение зарегистрированных пользователей
users = {}
users_online = { 'test1' :'test1' , 'test2' : 'test2'}

# Функция для распределения по серверам
def assign_server(user_identifier, servers):

    hash_value = int(hashlib.md5(user_identifier.encode()).hexdigest(), 16)
    return servers[hash_value % len(servers)]

# подключение через функцию, глобальная переменная выдает кучу ошибок которые я не хочу чинить
def connecting_to_database():
    return psycopg2.connect(
        host='127.0.0.1',
        port='5432',
        database='task_manager',
        user='postgres',
        password='miumiau'
    )

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # Устанавливаем код состояния 200 (OK)
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        
        # Формируем ответ
        response = {
            'message': 'Hello, this is a GET response!',
            'path': self.path
        }
        self.wfile.write(json.dumps(response).encode('utf-8'))

    def do_POST(self):
        # Устанавливаем код состояния 200 (OK)
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        
        # Получаем длину данных
        content_length = int(self.headers['Content-Length'])
        # Читаем данные
        post_data = self.rfile.read(content_length)
        
        # Логируем входящие данные для отладки
        print(f"Received data: {post_data}")
        
        # Проверка на пустые данные
        if not post_data:
            self.send_response(400)  # Bad Request
            self.wfile.write(json.dumps({'error': 'Empty data'}).encode('utf-8'))
            return
        
        # Парсим данные из JSON
        try:
            data = json.loads(post_data)
            action = data.get('action')
            login = data.get('login')
            password = data.get('password')
            email = data.get('email')
            # server_url = data.get ('url_of_small_server')

        except json.JSONDecodeError:
            self.send_response(400)  # Bad Request
            self.wfile.write(json.dumps({'error': 'Invalid JSON'}).encode('utf-8'))
            return

        # Обработка действий
        if action == 'register':
            response = self.handle_register(login, password, email)
        elif action == 'login':
            response = self.handle_login(login, password)
        elif action == 'status':
            response = {
                'message': 'Server live!',
            }
        elif action == 'logout':
            del users_online[login]
            response = {
                'message': 'Logout successful!',
            }
        elif action == 'list_of_user':
            list_of_user = ",".join(users_online.keys())
            response = {
                'message': 'List sended',
                'list_of_users': list_of_user,
            }
        else:
            response = {
                'message': 'Invalid action. Use "register" or "login".'
            }

        self.wfile.write(json.dumps(response).encode('utf-8'))

    def handle_register(self, login, password, email):
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            
            # Проверка на существующий логин
            cursor.execute("SELECT * FROM users WHERE login = %s", (login,))
            if cursor.fetchone():
                return {'message': 'Ошибка регистрации, такой логин уже существует'}
            
            # Проверка на существующую почту
            cursor.execute("SELECT * FROM users WHERE email = %s", (email,))
            if cursor.fetchone():
                return {'message': 'Ошибка регистрации, пользователь с такой почтой уже существует'}
            
            # Список доступных серверов
            servers = [
                {'host': '127.0.0.1', 'port': 8081},
                {'host': '127.0.0.1', 'port': 8082},
                {'host': '127.0.0.1', 'port': 8083},
            ]
            
            # Выбор сервера для нового пользователя
            assigned_server = assign_server(login, servers)
            server_url = f"http://{assigned_server['host']}:{assigned_server['port']}"
            
            # Добавление нового пользователя с адресом сервера
            cursor.execute(
                "INSERT INTO users (login, password, email, url_of_small_server) VALUES (%s, %s, %s, %s)",
                (login, password, email, server_url)
            )
            
            connect.commit()
            cursor.close()
            connect.close()
            
            return {
                'message': 'Registration successful!',  # менять нельзя, обрабатывается в клиенте
                'login': login,
            }
        except Exception as e:
            connect.rollback()  # Отмена транзакции в случае ошибки
            print(f"Возникла ошибка связанная с базой данных: {e}")
            return {'message': 'Регистрация не завершена, ошибка транзакции'}

    def handle_login(self, login, password, server_url):
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT * FROM users WHERE login = %s AND password = %s AND url_of_small_server = %s", (login, password, server_url, ))
            if cursor.fetchone():
                return {
                    'message': 'Login successful!',
                    'login': login,
                    'server': server_url
                }
            else:
                return {'message': 'Неправильный логин или пароль'}
        except Exception as e:
            connect.rollback()
            print(f"Ошибка базы данных: {e}")
            return {'message': 'Вход не выполнен, ошибка транзакции'}

def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()