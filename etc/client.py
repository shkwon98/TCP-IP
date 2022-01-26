import socket

# Enter IP or Hostname of your server
HOST = '192.168.0.10'
# Pick an open Port (1000+ recommended), must match the server port
PORT = 2000
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST,PORT))

#Lets loop awaiting for your input
while True:
	command = input('Enter your command: ')
	s.send(command.encode())
	reply = s.recv(1024).decode()
	if reply == 'Terminate':
		break
	print(reply)
