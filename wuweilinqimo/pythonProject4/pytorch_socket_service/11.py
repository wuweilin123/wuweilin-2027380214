import socket

# 定义服务端的 IP 和端口号
HOST = '127.0.0.1'
PORT = 8000

# 创建 Socket 对象
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 绑定服务端的 IP 和端口号
s.bind((HOST, PORT))

# 开始监听端口，最多同时处理 5 个连接请求
s.listen(5)

print('Server listening on {}:{}'.format(HOST, PORT))

while True:
    # 接收客户端连接请求，返回一个新的 Socket 对象和客户端的 IP 地址和端口号
    conn, addr = s.accept()

    # 打印客户端的 IP 和端口号
    print('Connected by {}:{}'.format(addr[0], addr[1]))

    # 从客户端接收数据，最多一次接收 1024 字节
    data = conn.recv(1024)

    # 如果接收到的数据不为空，则原样返回给客户端
    if data:
        conn.sendall(data)
        print('Received: {}'.format(data))

    # 关闭连接
    conn.close()