import socket
import traceback
import concurrent.futures
import json
import sys
HOST = '127.0.0.1'  # 本地主机IP
# PORT = random.randint(40000,50000)        # 监听端口号
ENCODING = 'utf-8'  # 字符编码方式
PORT=8000


class onProcess():
    def __init__(self,predictor):
        self._predict = predictor

    def onReceive(self,conn,data):
        msg=dict()
        try:
            cmd= data['type']
            msg['type'] = cmd
            if cmd =='classify':
                try:
                    detail = data['params']
                    img_path = detail['path']
                    msg['index'] = detail['index']
                    ret = self._predict.get_prediction(img_path)
                    print("classify result",ret)
                    if ret is not None:
                        if len(ret) == 2:
                            msg['class']= ret[0]
                        else:
                            msg['class'] = -1

                except Exception as e:
                    except_msg = traceback.format_exc()
                    msg['index'] = e
                    msg['class'] = -1

                    print(except_msg)

            else:
                msg['index'] = '命令错误'
                msg['class'] = -1
        except Exception  as e:
            except_msg = traceback.format_exc()
            msg['result_code'] = 5
            msg['message'] = except_msg

        message = json.dumps(msg,ensure_ascii=False)
        message = message.encode(ENCODING)
        conn.sendall(message)


class Socket_server():
    def __init__(self,ip=HOST,port=PORT,onListener=None):
        self.ip=ip
        self.port=port
        self.onListener=onListener
        self.is_start=True

    def process_data(self,conn, data):
        print('receive data--->' + str(data))
        try:
            msg = json.loads(data)
            self.onListener.onReceive(conn,msg)
        except Exception as e:
            except_msg = traceback.format_exc()
            print(except_msg)
            conn.sendall(except_msg.encode(ENCODING))

    def handle_client(self, conn, addr):
        with conn:
            print('Connected by', addr)

            while self.is_start:
                data = conn.recv(10240)
                if not data:
                    break
                data = data.decode(ENCODING)
                self.process_data(conn, data)
            conn.close()

    def start(self):
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        self.server.bind((HOST, PORT))
        self.server.listen()
        
        print('Server listening on', (HOST, PORT))
        with concurrent.futures.ThreadPoolExecutor() as executor:
            while self.is_start:
                try:
                    conn, addr = self.server.accept()
                    executor.submit(self.handle_client, conn, addr)
                except Exception as e:
                    print(e)

    def stop(self):
        print("stop server")
        self.is_start=False
        self.server.close()
        if self.onListener:
            self.onListener.onClose()
        sys.exit(0)




