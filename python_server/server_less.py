from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import psycopg2

def connecting_to_database():
    return psycopg2.connect(
        host='127.0.0.1',
        port='5432',
        database='small_server_database',
        user='postgres',
        #пустой пароль не оставляем, postgre ругается
        # Если у вас есть несколько баз данных на одном сервере PostgreSQL, 
        # и один и тот же пользователь имеет доступ к ним, изменение пароля будет глобальным для этого пользователя, 
        # а не для конкретной базы данных. 
        password='miumiau' 
    )

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    
    def do_POST(self):
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        print(f"Received data: {post_data}")
        
        if not post_data:
            self.send_response(400) 
            self.wfile.write(json.dumps({'error': 'Empty data'}).encode('utf-8'))
            return
        
        try:
            data = json.loads(post_data)
            action = data.get('action')
            task_name = data.get('task_name')
            task_text = data.get('task_text')
            project_name = data.get('project_name')
            project_text = data.get('project_text')
            user_id = "1"  # Assuming user_id is hardcoded for simplicity
        except json.JSONDecodeError:
            self.send_response(400) 
            self.wfile.write(json.dumps({'error': 'Invalid JSON'}).encode('utf-8'))
            return

        if action == 'creation':
            response = self.task_creation(task_name, task_text, user_id)
        elif action == 'destruction':
            response = self.task_destruction(task_name, task_text, user_id)
        elif action == 'project_creation':
            response == project_creation(self, project_name, project_text, user_id)
        elif action == 'project_destruction':
            response == project_destruction(self, project_name, project_text, user_id)
        else:
            response = {
                'message': 'Invalid action. Use "creation" or "destruction".'
            }

        self.wfile.write(json.dumps(response).encode('utf-8'))

    def task_creation(self, task_name, task_text, user_id): #переделать с использованием таск id
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT * FROM user_tasks WHERE task_name = %s", (task_name,))
            if cursor.fetchone():
                return {
                    'message': 'Повтор задачи'
                }
            else:
                cursor.execute("INSERT INTO user_tasks (task_name, task_text, user_id) VALUES (%s, %s, %s)", (task_name, task_text, user_id))
                connect.commit()
                return {
                    'message': 'Task creation successful!', #менять нельзя, обрабатывается в клиенте
                    'task_name': task_name
                }
        except Exception as e:
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

    def task_destruction(self, task_name, task_text, user_id): #переделать с использованием таск id
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("DELETE FROM user_tasks WHERE task_name = %s AND task_text = %s AND user_id = %s", (task_name, task_text, user_id))
            connect.commit()
            if cursor.rowcount > 0:
                return {
                    'message': 'Destruction successful!'
                }
            else:
                return {
                    'message': 'Destruction failed. Task does not exist.'
                }
        except Exception as e:
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

def project_creation(self, project_name, project_text, user_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT * FROM project WHERE project_name = %s", (project_name,))
            if cursor.fetchone():
                return {
                    'message': 'Повтор проекта'
                }
            else:
                cursor.execute("INSERT INTO project (project_name, project_text, owner) VALUES (%s, %s, %s)", (project_name, project_text, user_id))
                connect.commit()
                return {
                    'message': 'Project creation successful!', #менять нельзя, обрабатывается в клиенте
                    'project_name': project_name
                }
        except Exception as e:
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

def project_destruction(self, project_name, project_text, user_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("DELETE FROM project WHERE project_name = %s AND project_text = %s AND owner= %s", (project_name, project_text, user_id))
            connect.commit()
            if cursor.rowcount > 0:
                return {
                    'message': 'Destruction successful!'
                }
            else:
                return {
                    'message': 'Destruction failed. Project does not exist.'
                }
        except Exception as e:
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8081):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()