from http.server import BaseHTTPRequestHandler, HTTPServer
import json

# Хранение зарегистрированных пользователей
users = {}

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
        
        # Парсим данные из JSON
        try:
            data = json.loads(post_data)
            action = data.get('action')
            username = data.get('username')
            password = data.get('password')
        except json.JSONDecodeError:
            self.send_response(400)  # Bad Request
            self.wfile.write(json.dumps({'error': 'Invalid JSON'}).encode('utf-8'))
            return

        # Обработка действий
        if action == 'register':
            if username in users:
                response = {
                    'message': 'Registration failed. User already exists.'
                }
            else:
                users[username] = password
                response = {
                    'message': 'Registration successful!',
                    'username': username
                }
        elif action == 'login':
            if username in users and users[username] == password:
                response = {
                    'message': 'Login successful!',
                    'username': username
                }
            else:
                print(username)
                response = {
                    'message': 'Login failed. Invalid username or password.'
                }
        elif action == 'status':
            response = {
                'message': 'Server live!',
            }
        else:
            response = {
                'message': 'Invalid action. Use "register" or "login".'
            }

        self.wfile.write(json.dumps(response).encode('utf-8'))

def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()
