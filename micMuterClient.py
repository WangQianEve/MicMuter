# encoding=utf-8
import socket
import time
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # if ipv6 then socket.AF_INET6
s.connect(('127.0.0.1', 9999))

for data in ['Michael', 'Tracy', 'Sarah']:
    s.send(data)
    time.sleep(1)

s.close()
