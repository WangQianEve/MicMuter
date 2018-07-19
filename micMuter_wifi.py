# encoding=utf-8
import socket
# import MicVolumeChanger
import time

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # if ipv6 then socket.AF_INET6
# s.bind(('10.10.211.156', 4000))
s.bind(('127.0.0.1', 9999))
s.listen(5)

# MicVolumeChanger.initMic()

while True:
    sock, addr = s.accept()
    while True:
        data = sock.recv(1024)
        time.sleep(1)
        if not data:
        	break
        print data
        # MicVolumeChanger.setMuteMicrophone(bool(int(data[-1])))
    sock.close()
    print 'connection closed'

