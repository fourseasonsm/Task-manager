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
            
            task_id = data.get('task_id')
            login = data.get('login')
            task_name = data.get('task_name')
            task_text = data.get('task_text')

            project_id = data.get('project_id')
            project_name = data.get('project_name')
            project_text = data.get('project_text')

            subtask_id = data.get('subtask_id')
            subtask_text = data.get('subtask_text')
            subtask_weight = data.get('subtask_weight')
            taker_id = data.get('taker_id')

            login = data.get('login')
            user_id = data.get('user_id')
            server_url = data.get('server_url')

        except json.JSONDecodeError:
            self.send_response(400) 
            self.wfile.write(json.dumps({'error': 'Invalid JSON'}).encode('utf-8'))
            return

        if action == 'creation':
            response = self.task_creation(task_name, task_text, login)
        elif action == 'register':
            response = self.handle_register(login, user_id, server_url)
        elif action == 'destruction':
            response = self.task_destruction(task_id)
        elif action == 'project_creation':
            response = self.project_creation(project_id, project_name, project_text, login)
        elif action == 'project_destruction':
            response = self.project_destruction(project_name, project_text, login)
        elif action == 'add_subtask':
            response = self.add_subtask(project_id, taker_id, subtask_id, subtask_text, subtask_weight)
        elif action == 'list_of_tasks':
            response = self.get_info(login)
        else:
            response = {
                'message': 'Invalid action.'
            }

        self.wfile.write(json.dumps(response).encode('utf-8'))

    def task_creation(self, task_name, task_text, login):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT user_id FROM users_small_server WHERE login = %s", (login,))
            user_id_list = cursor.fetchall()
            if not user_id_list:
                return {'message': 'User not found'}
            user_id = user_id_list[0][0]
            print(user_id)
            logger.debug("Executing query: INSERT INTO tasks (task_name, task_text, owner) VALUES (%s, %s, %s)")
            cursor.execute("INSERT INTO tasks (task_name, task_text, owner) VALUES (%s, %s, %s)", (task_name, task_text, user_id))
            connect.commit()
            # Берем максимальный ID, так как нумерация автоматическая и возрастающая
            cursor.execute("SELECT MAX(task_id) FROM tasks")
            task_id = cursor.fetchone()[0]
            logger.debug("Task creation successful")
            return {
                'message': 'Task creation successful!', #менять нельзя, обрабатывается в клиенте
                'task_name': task_name,
                'task_id': task_id,
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

    def handle_register(self, login, user_id, server_url):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            
            logger.debug("Executing query: UPDATE users SET server_url = %s WHERE login = %s")
            cursor.execute("INSERT INTO users_small_server (user_id, score, server_url, isOnline, login) VALUES (%s, 0, %s, false, %s)", (user_id, server_url, login))  
            connect.commit()
            logger.debug("Registration successful")
            return {
                'message': 'Registration successful!'
            }
        except Exception as e:
            logger.error(f"Error during registration: {e}")
            return {
                'error': str(e)
            }

    def task_destruction(self, task_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            logger.debug("Executing query: DELETE FROM tasks WHERE task_id = %s")
            cursor.execute("DELETE FROM tasks WHERE task_id = %s", (task_id,))
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

    def project_creation(self, project_name, project_text, login):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT user_id FROM users_small_server WHERE login = %s", (login,))
            user_id_list = cursor.fetchall()
            if not user_id_list:
                return {'message': 'User not found'}
            user_id = user_id_list[0][0]
            logger.debug("Executing query: INSERT INTO project (project_name, project_text, owner) VALUES (%s, %s, %s)")
            cursor.execute("INSERT INTO project (project_name, project_text, owner) VALUES (%s, %s, %s)", (project_name, project_text, user_id))
            connect.commit()

            cursor.execute("SELECT MAX(project_id) FROM project")
            project_id = cursor.fetchone()[0]

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

    def project_destruction(self, project_name, project_text, login): #ОНО НЕ РАБОЧЕЕ НАДО ПЕРЕДЕЛАТЬ но для этого нужны запросы к серверам
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT user_id FROM users_small_server WHERE login = %s", (login,))
            user_id_list = cursor.fetchall()
            if not user_id_list:
                return {'message': 'User not found'}
            user_id = user_id_list[0][0]
            #вот тут должно быть получение информации о том что мы поудаляли допзадачи на остальных серверах
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

    def add_subtask(self, project_id, taker_id, subtask_id, subtask_text, subtask_weight): #тут функция меняет в бд взявшего подзадачу, все еще вопрос с индексацией проекта
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            logger.debug("Executing query: SELECT * FROM project WHERE project_id = %s")
            cursor.execute("SELECT * FROM project WHERE project_id = %s", (project_id,))
            if cursor.fetchone():
                logger.debug("Executing query: INSERT INTO subtask (subtask_id, subtask_text, taken_by, subtask_weight) VALUES (%s, %s, %s, %s)")
                cursor.execute("INSERT INTO subtask (subtask_id, subtask_text, taken_by, subtask_weight) VALUES (%s, %s, %s, %s)", (subtask_id, 
                                                                                                                                   subtask_text, 
                                                                                                                                   taker_id, 
                                                                                                                                   subtask_weight))
                connect.commit()
                logger.debug("Пользователь успешно назначен")
                return {
                    'message': 'owner of subtask has changed', #менять нельзя, обрабатывается в клиенте
                    'subtask_id': subtask_id,
                    'taken_by' : taker_id            
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

    def get_info(self, login):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            logger.debug("Executing query: SELECT task_name, task_text FROM tasks WHERE owner = %s")
             # Получаем user_id по логину
            cursor.execute("SELECT user_id FROM users_small_server WHERE login = %s", (login,))
            user_id_list = cursor.fetchall()
            if not user_id_list:
                return {'message': 'User not found'}
            user_id = user_id_list[0][0]
        
            # Получаем список задач для данного user_id
            cursor.execute("SELECT task_name, task_text FROM tasks WHERE owner = %s", (user_id,))
            list_of_tasks = cursor.fetchall()
            print(list_of_tasks)
            return {
                    'message': 'List sended',
                    'list_of_tasks': list_of_tasks,
                }
        except Exception as e:
            logger.error(f"Error during getting info: {e}")
            return {
            'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()
        
    def send_copy(self, project_id, subtask_id, taker_id): #упаковка проекта происходит на сервере хозяина проекта
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            logger.debug("Executing query: SELECT project_name, project_text FROM projects WHERE project_id = %s")
            cursor.execute("SELECT project_name, project_text FROM projects WHERE project_id= %s", (project_id,))
            about_project = cursor.fetchall() #посылаем название и описание, вопрос что делать с id, на другом сервере он станет неоднозначным,
                                            #надо наверное еще таблицу заводить для совместных проектов Можно завести еще одно поле для 
                                            # хранения Id проекта на этом сервере и на другом, тогда процесс распутывания будет быстрее
                                            #UPD: храним project_id и from_another_server_project_id. У хозяина проекта эти id будут совпадать
            logger.debug("Executing query: SELECT subtask_text FROM subtasks WHERE subtask_id = %s")
            cursor.execute("SELECT subtask_text, taken_by, subtask_weight FROM subtasks WHERE subtask_id = %s", (subtask_id,))
            about_subtask = cursor.fetchall()
            all_info = about_project + about_subtask #это надо как то упаковать и отослать по адресу taker_id
            return {
                    'message': 'Information about project and subtask sended',
                    'list_of_tasks': all_info,
                }
        except Exception as e:
            logger.error(f"Error during getting info: {e}")
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()
 
    def search_dependences(self, project_id, login): #эта фигня тоже на сервере хозяина проекта должна запускаться
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            logger.debug("Executing query: SELECT project_name, project_text FROM projects WHERE project_id = %s")
            cursor.execute("SELECT  taken_by FROM subtasks WHERE project= %s", (project_id,)) #вот это мы берем, и на сервере взявшего по id 
            #пользоватя который взял находим по комбинации from_another_server_project_id + taken_by (id проекта на сервере-хозяине, на сервере который хранит копию будет другой id)
            takers_id = cursor.fetchall() #массив всех кто взял, отправить это на большой сервер и послать им сигнал на выполнение удалялки проекта (а какого именно проекта будет определяться по from_another_server_project_id)
        except Exception as e:
            logger.error(f"Error during searching for dependences: {e}")
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
    logger.info(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()