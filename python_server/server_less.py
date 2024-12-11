from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import psycopg2
import logging
import requests

logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger(__name__)

def connecting_to_database():
    return psycopg2.connect(
        host='127.0.0.1',
        port='5432',
        database='small_server_database',
        user='postgres',
        password=''
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
            task_name = data.get('task_name')
            task_text = data.get('task_text')

            project_id = data.get('project_id')
            foreign_project_id = data.get('foreign_project_id')
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

    def send_invitation(self, server_url, project_name, project_text, subtask_text, subtask_weight, owner_id, subtask_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            invitation_data = {
                "action": "invite_user",
                "project_name": project_name,
                "project_text": project_text,
                "subtask_text": subtask_text,
                "subtask_weight": subtask_weight,
                "owner_id": owner_id,
                "subtask_id": subtask_id
            }
            response = requests.post(server_url, json=invitation_data)
            response.raise_for_status()
            logger.debug(f"Приглашение отправлено на сервер {server_url}")
            return response.json()
        except requests.RequestException as e:
            logger.error(f"Ошибка при отправке приглашения: {e}")
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

    def handle_agreement(self, project_name, project_description, subtask_text, subtask_weight, owner_id, subtask_id, user_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("INSERT INTO projects (project_name, project_description, owner_id) VALUES (%s, %s, %s)RETURNING project_id",(project_name, project_description, user_id))
            project_id = cursor.fetchone()[0]

            cursor.execute("NSERT INTO subtasks (project_id, subtask_text, subtask_weight, taker_id)VALUES (%s, %s, %s, %s)",(project_id, subtask_text, subtask_weight, user_id))

            connect.commit()
            logger.debug(f"Проект {project_name} сохранен для пользователя {user_id}")

            self.notify_owner_server(owner_id, project_id, subtask_id)

            return {
                'message': 'Copy of a project created succesfully'
            }
        except Exception as e:
            logger.error(f"Ошибка при обработке согласия пользователя: {e}")
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

    def agee_notification(self, owner_id, project_id, subtask_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()

            cursor.execute("SELECT server_url FROM servers WHERE id = %s", (owner_id,))
            server_url = cursor.fetchone()[0]

            notification_data = {
                "action": "subtask taken",
                "project_id": project_id,
                "subtask_id": subtask_id
            }

            response = requests.post(server_url, json=notification_data)
            response.raise_for_status()
            logger.debug(f"Уведомление отправлено на сервер {server_url}")
        except requests.RequestException as e:
            logger.error(f"Ошибка при отправке уведомления: {e}")
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

    def project_destruction(self, project_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            cursor.execute("SELECT COUNT(*) FROM subtasks WHERE project_id = %s AND status != 'completed'",(project_id,)
            )
            incomplete_subtasks_count = cursor.fetchone()[0]

            if incomplete_subtasks_count > 0:
                logger.warning(f"Проект {project_id} не может быть удален, так как не все подзадачи выполнены.")
                return {
                    'error': 'Проект не может быть удален, так как не все подзадачи выполнены.'
                }
            cursor.execute(
                """DELETE FROM projects WHERE project_id = %s""",(project_id,)
            )
            connect.commit()

            logger.debug(f"Проект {project_id} успешно удален.")
            return {
                'message': 'Проект успешно удален.'
            }
        except Exception as e:
            logger.error(f"Ошибка при удалении проекта: {e}")
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()  

    def task_creation(self, task_name, task_text, login): #РАБОТАЕТ
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()
            #получаем логин и находим user_id тк хранить его как будто не очень удобно
            cursor.execute("SELECT user_id FROM users_small_server WHERE login = %s", (login,))
            user_id_list = cursor.fetchall()
            if not user_id_list:
                return {'message': 'User not found'}
            user_id = user_id_list[0][0]

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

    def handle_register(self, login, user_id, server_url): #РАБОТАЕТ обновление таблицы маленького сервера при добавлении к нему поьзователя
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

    def task_destruction(self, task_id): #РАБОТАЕТ
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

    def project_creation(self, project_id, foreign_project_id, project_name, project_text, 
                          login,
                          subtask1_text, subtask1_weight,
                          subtask2_text, subtask2_weight,
                          subtask3_text, subtask3_weight
                         ): #ВВЕСТИ ДВОЙНОЙ ID
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

            #изначально foreign id иницилизируется обычным id, нужен он для однозначности проекта при пересылке
            logger.debug("Executing query: INSERT INTO project (project_name, project_text, owner) VALUES (%s, %s, %s)")
            cursor.execute("INSERT INTO project (project_name, project_text, owner) VALUES (%s, %s, %s)", (project_name, project_text, user_id))
            connect.commit()

            cursor.execute("SELECT MAX(project_id) FROM project")
            project_id = cursor.fetchone()[0]
            
            logger.debug("Executing query: INSERT INTO subtasks (project, subtask1_text, taker1_id, subtask1_weight) VALUES (%s, %s, %s, %s)")
            cursor.execute("INSERT INTO subtasks (project, subtask1_text, taker1_id, subtask1_weight) VALUES (%s, %s, %s, %s)", (project_id, subtask1_text, user_id, subtask1_weight))
            connect.commit()
            
            cursor.execute("SELECT subtask1_id FROM subtasks WHERE project = %s", (project_id,))
            subtask1_id_list = cursor.fetchall()
            subtask1_id = subtask1_id_list[0][0]

            logger.debug("Executing query: INSERT INTO subtasks (project, subtask2_text, taker2_id, subtask2_weight) VALUES (%s, %s, %s, %s)")
            cursor.execute("INSERT INTO subtasks (project, subtask2_text, taker2_id, subtask2_weight) VALUES (%s, %s, %s, %s)", (project_id, subtask2_text, user_id, subtask2_weight))
            connect.commit()

            cursor.execute("SELECT subtask2_id FROM subtasks WHERE project = %s", (project_id,))
            subtask2_id_list = cursor.fetchall()
            subtask2_id = subtask2_id_list[0][0]

            logger.debug("Executing query: INSERT INTO subtasks (project, subtask3_text, taker3_id, subtask3_weight) VALUES (%s, %s, %s, %s)")
            cursor.execute("INSERT INTO subtasks (project, subtask3_text, taker3_id, subtask3_weight) VALUES (%s, %s, %s, %s)", (project_id, subtask3_text, user_id, subtask3_weight))
            connect.commit()

            cursor.execute("SELECT subtask3_id FROM subtasks WHERE project = %s", (project_id,))
            subtask3_id_list = cursor.fetchall()
            subtask3_id = subtask3_id_list[0][0]

            logger.debug("Project creation successful")

            return {
                'message': 'Project creation successful!', #менять нельзя, обрабатывается в клиенте
                'project_name': project_name,
                'project_id': project_id,
                'foreign_project_id': foreign_project_id,
                'subtask1_id': subtask1_id,
                'subtask2_id': subtask2_id,
                'subtask3_id': subtask3_id
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
    def delete_project(self, project_id):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()

            # Проверяем, все ли подзадачи выполнены
            cursor.execute("SELECT COUNT(*) FROM subtasks WHERE project_id = %s AND status != 'completed'",(project_id,)
            )
            incomplete_subtasks_count = cursor.fetchone()[0]

            if incomplete_subtasks_count > 0:
                logger.warning(f"Проект {project_id} не может быть удален, так как не все подзадачи выполнены.")
                return {
                    'error': 'This project can\'t be deteted, not all subtasks are completed'
                }
            cursor.execute("DELETE FROM projects WHERE project_id = %s",(project_id,)
            )
            connect.commit()

            logger.debug(f"Проект успешно удален.")
            return {
                'message': 'Project deleted.'
            }
        except Exception as e:
            logger.error(f"Ошибка при удалении проекта: {e}")
            return {
                'error': str(e)
            }
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

    def project_destruction(self, project_id, foreign_project_id, login, project_name, project_text): #будет удалять проект ТОЛЬКО если его удаляет хозяин, если не хозяин ему нельзя удалять
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
            cursor.execute("SELECT owner FROM projects WHERE project_id = %s AND foreign_project_id =%s ", (project_id, foreign_project_id))
            owner_list = cursor.fetchall()
            owner = owner_list[0][0]
            if (owner != user_id): 
                return {'message': 'It is not your project'}


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

    def get_info(self, login):   #РАБОТАЕТ вопрос с распилрм ответа у Артема
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
            cursor.execute("SELECT project_name, project_text FROM projects WHERE owner = %s", (user_id,))
            list_of_projects = cursor.fetchall()
            return {
                    'message': 'List sended',
                    'list_of_tasks': list_of_tasks,
                    'list_of_projects': list_of_projects
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
        
 
    

def run(server_class=HTTPServer, handler_class=SimpleHTTPRequestHandler, port=8083):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logger.info(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == "__main__":
    run()