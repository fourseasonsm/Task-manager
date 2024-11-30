from http.server import BaseHTTPRequestHandler, HTTPServer
import json

# Хранение зарегистрированных пользователей
tasks = {}
class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    
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
            task_name = data.get('task_name')
            task_text = data.get('task_text')
        except json.JSONDecodeError:
            self.send_response(400)  # Bad Request
            self.wfile.write(json.dumps({'error': 'Invalid JSON'}).encode('utf-8'))
            return

        if action == 'creation':
            # Обработка действий
            if task_name  in tasks and tasks[task_name] == task_text:
                response = {
                'message': 'Task creation failed. Task already exists.'
            }
            else:
                tasks[task_name] = task_text
                response = {
                'message': 'Task creation successful!',
                'task_name': task_name
            }
        elif action == 'destruction':
            if task_name in tasks and tasks[task_name] == task_text:
                del tasks[task_name]
                response = {
                    'message': 'Destruction successful!',
                }
            else:
                response = {
                    'message': 'Destruction failed. Task doesnt exists.'
                }
        else:
            response = {
                'message': 'Invalid action. Use "creation" or "destruction".'
            }


        self.wfile.write(json.dumps(response).encode('utf-8'))

def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8079):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()
