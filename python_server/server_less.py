from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import psycopg2
import logging

logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)

def connecting_to_database():
    return psycopg2.connect(
        host='127.0.0.1',
        port='5432',
        database='small_server_database',
        user='postgres',
        password='miumiau'
    )

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    
    def do_POST(self):
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        logger.debug(f"Received data: {post_data}")
        
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
            response = self.project_creation(project_name, project_text, user_id)
        elif action == 'project_destruction':
            response = self.project_destruction(project_name, project_text, user_id)
        else:
            response = {
                'message': 'Invalid action. Use "creation" or "destruction".'
            }

        self.wfile.write(json.dumps(response).encode('utf-8'))

    def task_creation(self, task_name, task_text, user_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            
            logger.debug("Executing query: SELECT * FROM tasks WHERE task_name = %s")
            cursor.execute("SELECT * FROM tasks WHERE task_name = %s", (task_name,))
            if cursor.fetchone():
                logger.debug("Task already exists")
                return {
                    'message': 'Повтор задачи'
                }
            else:
                logger.debug("Executing query: INSERT INTO tasks (task_name, task_text, owner) VALUES (%s, %s, %s)")
                cursor.execute("INSERT INTO tasks (task_name, task_text, owner) VALUES (%s, %s, %s)", (task_name, task_text, user_id))
                connect.commit()
                logger.debug("Task creation successful")
                return {
                    'message': 'Task creation successful!', #менять нельзя, обрабатывается в клиенте
                    'task_name': task_name
                }
        except Exception as e:
            logger.error(f"Error during task creation: {e}")
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

    def task_destruction(self, task_name, task_text, user_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            
            logger.debug("Executing query: DELETE FROM tasks WHERE task_name = %s AND task_text = %s AND owner = %s")
            cursor.execute("DELETE FROM tasks WHERE task_name = %s AND task_text = %s AND owner = %s", (task_name, task_text, user_id))
            connect.commit()
            if cursor.rowcount > 0:
                logger.debug("Task destruction successful")
                return {
                    'message': 'Destruction successful!'
                }
            else:
                logger.debug("Task does not exist")
                return {
                    'message': 'Destruction failed. Task does not exist.'
                }
        except Exception as e:
            logger.error(f"Error during task destruction: {e}")
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
            
            logger.debug("Executing query: SELECT * FROM project WHERE project_name = %s")
            cursor.execute("SELECT * FROM project WHERE project_name = %s", (project_name,))
            if cursor.fetchone():
                logger.debug("Project already exists")
                return {
                    'message': 'Повтор проекта'
                }
            else:
                logger.debug("Executing query: INSERT INTO project (project_name, project_text, owner) VALUES (%s, %s, %s)")
                cursor.execute("INSERT INTO project (project_name, project_text, owner) VALUES (%s, %s, %s)", (project_name, project_text, user_id))
                connect.commit()
                project_id = cursor.execute("SELECT project_id FROM project WHERE project_name = %s", (project_name,))
                logger.debug("Project creation successful")
                return {
                    'message': 'Project creation successful!', #менять нельзя, обрабатывается в клиенте
                    'project_name': project_name,
                    'project_id': project_id
                }
        except Exception as e:
            logger.error(f"Error during project creation: {e}")
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

    def project_destruction(self, project_name, project_text, user_id): #НУЖДАЕТСЯ В СЕРЬЕЗНОМ ОБДУМЫВАНИИ
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            
            logger.debug("Executing query: DELETE FROM project WHERE project_name = %s AND project_text = %s AND owner = %s")
            cursor.execute("DELETE FROM project WHERE project_name = %s AND project_text = %s AND owner = %s", (project_name, project_text, user_id))
            connect.commit()
            if cursor.rowcount > 0:
                logger.debug("Project destruction successful")
                return {
                    'message': 'Destruction successful!'
                }
            else:
                logger.debug("Project does not exist")
                return {
                    'message': 'Destruction failed. Project does not exist.'
                }
        except Exception as e:
            logger.error(f"Error during project destruction: {e}")
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

    def add_subtask(self, project_id, user_id, subtask_name, subtask_text, subtask_weight):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            logger.debug("Executing query: SELECT * FROM project WHERE project_id = %s")
            cursor.execute("SELECT * FROM project WHERE project_id = %s", (project_id,))
            if cursor.fetchone():
                logger.debug("Executing query: INSERT INTO  subtask (subtask_name, subtask_text, taken_by, subtask_weight) VALUES (%s, %s, %s, %s)")
                cursor.execute("INSERT INTO  subtask (subtask_name, subtask_text, taken_by, subtask_weight) VALUES (%s, %s, %s, %s)", (subtask_name, 
                                                                                                                                   subtask_text, 
                                                                                                                                   user_id, 
                                                                                                                                   subtask_weight))
                connect.commit()
                logger.debug("Пользователь успешно назначен")
                return {
                    'message': 'Subtask creation successful!', #менять нельзя, обрабатывается в клиенте
                    'subtask_name': subtask_name,
                    'user_id' : user_id            
                    }
            else:
                logger.debug("Project not exists")
                return {
                    'message': 'Попытка назначить на несуществующий проект'
                }
        except Exception as e:
            logger.error(f"Error during project creation: {e}")
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()


def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8083):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logger.info(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()