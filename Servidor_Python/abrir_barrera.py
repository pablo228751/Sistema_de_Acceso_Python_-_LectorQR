import socket

var=open("config.dat","r")
var_lineas=var.readlines()
var.close()

HOST = var_lineas[4][13:-5].rstrip()
PORT = 999



def abrir_barrera():
	try:
		with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
			s.connect((HOST, PORT))
			s.sendall(b"~")
			s.close
			print("BARRERA ABIERTA")
	except Exception as e:
			print("error: ",e)




abrir_barrera()