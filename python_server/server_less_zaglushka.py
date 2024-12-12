from http.server import BaseHTTPRequestHandler, HTTPServer
import json

# Хранение зарегистрированных пользователей
tasks = {'test0' :['test_1' ,'test1'],'test1' :['test_1' ,'test1']}
test = tasks['test0'][0]+',' + tasks['test0'][1]
print (test)
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
            user_name = data.get('login')
            task_name = data.get('task_name')
            task_text = data.get('task_text')
        except json.JSONDecodeError:
            self.send_response(400)  # Bad Request
            self.wfile.write(json.dumps({'error': 'Invalid JSON'}).encode('utf-8'))
            return
        if action == 'creation':
            # Обработка действий
            if task_name  in tasks[user_name]:
                print (action)
                response = {
                'message': 'Task creation failed. Task already exists.'
                }
            else:
                tasks[user_name].append(task_name)
                tasks[user_name].append(task_text)
                print (action)
                response = {
                'message': 'Task creation successful!',
                'task_name': task_name
                }
        elif action == 'list_of_tasks':
            if user_name in tasks:
                list_of_tasks = ""
                for i in range(0, len(tasks[user_name])-1) :
                    if (i % 2 == 0):
                        list_of_tasks = list_of_tasks+tasks[user_name][i] + ',' + tasks[user_name][i+1]+ ','
                    print (list_of_tasks)
                response = {
                    'message': 'List sended',
                    'list_of_tasks': list_of_tasks,
                }
            else:
                tasks[user_name] = [];
                response = {
                'message': 'Task list empty!',
                }
        elif action == 'destruction':
            check_destruction = 0
            for i in range(0, len(tasks[user_name])) :
                if task_name  == tasks[user_name][i] and tasks[user_name][i+1] == task_text:
                    tasks[user_name].pop(i)
                    tasks[user_name].pop(i)
                    check_destruction = 1
                    response = {
                        'message': 'Destruction successful!',
                    }
            if check_destruction == 0:
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
