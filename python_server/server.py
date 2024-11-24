from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import psycopg2

# подключение через функцию, глобальная переменная выдает кучу ошибок которые я не хочу чинить
def connecting_to_database():
    return psycopg2.connect(
        host='127.0.0.1',
        port='5432',
        database='server_database',
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
        else:
            response = {
                'message': 'Invalid action. Use "register" or "login".'
            }

        self.wfile.write(json.dumps(response).encode('utf-8'))

    def handle_register(self, login, password, email):
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT * FROM users WHERE login = %s", (login,))
            if cursor.fetchone():
                return {
                    'message': 'Ошибка регистрации, такой логин уже существует'
                }
            cursor.execute("SELECT * FROM users WHERE email = %s", (email,))  # Исправленный запрос
            if cursor.fetchone():
                return {
                    'message': 'Ошибка регистрации, пользователь с такой почтой уже существует'
                }
            cursor.execute("INSERT INTO users (login, password, email) VALUES (%s, %s, %s)", (login, password, email))
            connect.commit()
            cursor.close()
            connect.close()
            return {
                'message': 'Registration successful!', #менять нельзя, обрабатывается в клиенте
                'login': login
            }
        except Exception as e:
            connect.rollback()  # Отменяем транзакцию если она кривая
            print(f"Возникла ошибка связанная с базой данных: {e}")
            return {
                'message': 'Регистрация не завершена, ошибка транзакции'
            }

    def handle_login(self, login, password):
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT * FROM users WHERE login = %s AND password = %s", (login, password))
            if cursor.fetchone():
                return {
                    'message': 'Login successful!', #менять нельзя, обрабатывается в клиенте
                    'login': login
                }
            else:
                return {
                    'message': 'Неправильный логин или пароль'
                }
        except Exception as e:
            connect.rollback()  # Отменяем транзакцию если она кривая
            print(f"Возникла ошибка связанная с базой данных: {e}")
            return {
                'message': 'Вход не выполнен, ошибка транзакции'
            }

def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()