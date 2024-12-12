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
            target_login = data.get('target_login')
            project_name = data.get('project_name')
            project_text = data.get('project_text')


            subtask_id = data.get('subtask_id')
            subtask_status = data.get('subtask_status')
            subtask_text = data.get('subtask_text')
            subtask_weight = data.get('subtask_weight')
            taker_login = data.get('taker_login')

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
        elif action == 'list_of_tasks':
            response = self.get_info(login)
        elif action == 'send_invitation':
            response = self.send_invitation(target_login, project_name, project_text, subtask_text, subtask_weight, login, subtask_id)
        elif action == 'agree':
            response = self.handle_agreement(project_name, project_text, subtask_text, subtask_weight, login, subtask_id, taker_login)
        elif action == 'decline':
            response = self.handle_user_decline(project_name, project_text, subtask_text, subtask_weight, login, subtask_id, taker_login)
        elif action == 'subtask_status_update':
            response = self.handle_subtask_status_update(project_id, subtask_id, subtask_status, taker_login, login)
        elif action == 'notify_owner':
            response = self.notify_owner_server(project_id, subtask_id, subtask_status)
        else:
            response = {
                'message': 'Invalid action.'
            }

        self.wfile.write(json.dumps(response).encode('utf-8'))

    def send_invitation(self, target_login, project_name, project_text, subtask_text, subtask_weight, login, subtask_id):
        try:
            #ищем куда отправлять с помощью запроса к большому серверу
            user_server_url = self.get_user_server_url(target_login)
            if not user_server_url:
                return {
                    'error': 'Сервер пользователя не найден'
                }   
            # данные о проекте и предложенной подзадаче
            invitation_data = {
                "action": "invite_user",
                "target_login": target_login,  # логин пользователя, которому отправляется приглашение
                "project_name": project_name,
                "project_text": project_text,
                "subtask_text": subtask_text,
                "subtask_weight": subtask_weight,
                "login": login,  # логин хозяина проекта
                "subtask_id": subtask_id
            }

            # отправляем приглашение по найденному адресу
            response = requests.post(user_server_url, json=invitation_data)
            response.raise_for_status()
            logger.debug(f"Приглашение отправлено на сервер {user_server_url}")
            return response.json()
        except requests.RequestException as e:
            logger.error(f"Ошибка при отправке приглашения: {e}")
            return {
                'error': str(e)
            }

    def get_user_server_url(self, target_login):
        try:
            central_server_url = "http://127.0.0.1:8080"

            # отправляем запрос на поиск url куда отправлять
            response = requests.post(central_server_url, json={"target_login": target_login})
            response.raise_for_status()

            # получаем адрес сервера пользователя
            user_info = response.json()
            if user_info.get('message') == 'User info sended':
                return user_info['list_of_users'][0]['server_url']
            else:
                logger.error(f"Ошибка при получении информации о сервере пользователя: {user_info.get('error')}")
                return None
        except requests.RequestException as e:
            logger.error(f"Ошибка при запросе информации о сервере пользователя: {e}")
            return None

    def handle_agreement(self, project_name, project_text, subtask_text, subtask_weight, owner_login, subtask_id, taker_login):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()

            # Сохраняем проект и подзадачу
            cursor.execute("INSERT INTO projects (project_name, project_text, owner_login, taker_login) VALUES (%s, %s, %s, %s) RETURNING project_id", (project_name, project_text, owner_login, taker_login))
            project_id = cursor.fetchone()[0]

            cursor.execute("INSERT INTO subtasks (project_id, subtask_text, subtask_weight, taker_login, status) VALUES (%s, %s, %s, %s, 'accepted')",(project_id, subtask_text, subtask_weight, taker_login))

            connect.commit()
            logger.debug(f"Проект {project_name} сохранен для пользователя {taker_login}")

            # Отправляем уведомление обратно на сервер-хозяина
            self.notify_owner_server(owner_login, project_id, subtask_id)

            return {
                'message': 'Проект успешно сохранен'
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

    def notify_owner_server(self, owner_login, project_id, subtask_id, status):
    
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()

            # Получаем URL сервера-хозяина
            owner_server_url = self.get_user_server_url(owner_login)
            if not owner_server_url:
                return {
                    'error': 'Сервер пользователя не найден'
                }  

            # Подготовка данных для уведомления
            notification_data = {
                "action": "subtask_status_update",
                "project_id": project_id,
                "subtask_id": subtask_id,
                "status": status  # Статус: "accepted" или "declined"
            }

            # Отправка уведомления
            response = requests.post(owner_server_url, json=notification_data)
            response.raise_for_status()
            logger.debug(f"Уведомление отправлено на сервер {owner_server_url}")
        except requests.RequestException as e:
            logger.error(f"Ошибка при отправке уведомления: {e}")
        finally:
            if cursor:
                cursor.close()
            if connect:
                connect.close()

    def handle_user_decline(self, project_name, owner_login, subtask_id, taker_login):
        try:
            # Отправляем уведомление обратно на сервер-хозяина с указанием статуса "declined"
            self.notify_owner_server(owner_login, None, subtask_id, "declined")

            logger.debug(f"Пользователь {taker_login} отказался от проекта {project_name}")

            return {
                'message': 'Отказ от проекта успешно обработан'
            }
        except Exception as e:
            logger.error(f"Ошибка при обработке отказа пользователя: {e}")
            return {
                'error': str(e)
            }

    def handle_subtask_status_update(self, project_id, subtask_id, status, taker_login, owner_login):
        connect = None
        cursor = None
        try:
            connect = connecting_to_database()
            cursor = connect.cursor()

            if status == "accepted":
                # Если подзадача принята, обновляем статус
                cursor.execute("UPDATE subtasks SET status = 'accepted', taker_login = %s WHERE project_id = %s AND subtask_id = %s", (taker_login, project_id, subtask_id))
            elif status == "declined":
                # Если подзадача отклонена, возвращаем ее статус к исходному (взятому хозяином проекта)
                cursor.execute(
                "UPDATE subtasks SET status = 'in_progress', taker_login = %s WHERE project_id = %s AND subtask_id = %s", (owner_login, project_id, subtask_id))

            connect.commit()
            logger.debug(f"Статус подзадачи {subtask_id} обновлен на '{status}'")
        except Exception as e:
            logger.error(f"Ошибка при обработке уведомления: {e}")
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

    def project_creation(self, project_id, owner_id, project_name, project_text, 
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
                'owner_id': owner_id,
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