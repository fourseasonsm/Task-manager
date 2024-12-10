from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import psycopg2
import hashlib
import requests
import logging

print(a)
# Настройки логирования
logging.basicConfig(level=logging.DEBUG, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

# Функция для распределения по серверам
def assign_server(user_identifier, servers):
    hash_value = int(hashlib.md5(user_identifier.encode()).hexdigest(), 16)
    return servers[hash_value % len(servers)]

def send_user_data_to_server(server_url, login, user_id):
    data = {'action': 'register', 'login': login, 'user_id': user_id, 'server_url': server_url}
    json_data = json.dumps(data, indent=4)
    try:
        response = requests.post(server_url, data=json_data)
        response.raise_for_status()  # Raise an exception for bad status codes
    except requests.RequestException as e:
        print(f"Error sending user data to server: {e}")

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
        logger.debug(f"Received data: {post_data}")
        
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
            # server_url = data.get ('server_url')

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
            response = self.handle_logout(login)
        elif action == 'list_of_user':
            response = self.online_users()
        else:
            response = {
                'message': 'Invalid action.'
            }

        self.wfile.write(json.dumps(response).encode('utf-8'))
    
    def online_users(self):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT login FROM users WHERE isOnline = true")
            users_online_list = cursor.fetchall()  
            return {
                'message': 'List sended',
                'list_of_users': users_online_list,
            }
        except Exception as e:
            if connect:
                connect.rollback()  
            logger.error(f"Возникла ошибка связанная с базой данных: {e}")
            return {'message': 'Ошибка транзакции'}
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()
    
    def handle_register(self, login, password, email):
        connect = None
        cursor = None
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
                "INSERT INTO users (login, password, email, server_url, isOnline, scrore) VALUES (%s, %s, %s, %s, false, 0)",
                (login, password, email, server_url)
            )
        
            connect.commit()
            
            cursor.execute("SELECT user_id FROM users WHERE login = %s", (login,))
            user_id = cursor.fetchone()[0]
            send_user_data_to_server(server_url, login, user_id)

            return {
                'message': 'Registration successful!',  # менять нельзя, обрабатывается в клиенте
                'login': login,
            }
        except Exception as e:
            if connect:
                connect.rollback()  # Отмена транзакции в случае ошибки
            logger.error(f"Возникла ошибка связанная с базой данных: {e}")
            return {'message': 'Регистрация не завершена, ошибка транзакции'}
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()
    
    def handle_login(self, login, password):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            # Изменяем запрос, чтобы выбрать server_url
            cursor.execute("SELECT server_url FROM users WHERE login = %s AND password = %s", (login, password))
            
            # Извлекаем результат
            result = cursor.fetchone()
            
            if result:
                server_url = result[0]  # Получаем server_url из результата
                cursor.execute("UPDATE users SET isOnline = true WHERE login = %s", (login,))
                connect.commit()               
                return {
                        'message': 'Login successful!',
                        'login': login,
                        'server': server_url
                    }
            else:
                return {'message': 'Неправильный логин или пароль'}
        except Exception as e:
            if connect:
                connect.rollback()
            logger.error(f"Ошибка базы данных: {e}")
            return {'message': 'Вход не выполнен, ошибка транзакции'}
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()
    
    def handle_logout(self, login):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            # Изменяем запрос, чтобы выбрать server_url
            cursor.execute("SELECT server_url FROM users WHERE login = %s", (login,))
            
            # Извлекаем результат
            result = cursor.fetchone()
            if result:
                server_url = result[0]  # Получаем server_url из результата
                cursor.execute("UPDATE users SET isOnline = false WHERE login = %s", (login,))
                connect.commit()
                return {
                        'message': 'Logout successful!'
                    }          
            else:
                return {'message': 'Ошибка выхода'}
            
        except Exception as e:
            if connect:
                connect.rollback()
            logger.error(f"Ошибка базы данных: {e}")
            return {'message': 'Вход не выполнен, ошибка транзакции'}
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logger.info(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()