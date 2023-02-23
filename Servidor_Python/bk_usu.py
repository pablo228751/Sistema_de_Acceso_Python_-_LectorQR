import socket
import time
import os
from datetime import datetime, timedelta


var=open("config.dat","r")
var_lineas=var.readlines()
var.close()

HOST = var_lineas[4][13:-5].rstrip()
PORT = 999



def datos_leidos(datos):
	fec_hor = datetime.now()
	fecha = fec_hor.strftime('%Y/%m/%d')
	hora= fec_hor.strftime('%H:%M:%S')
	file = open("bk_usu.txt", "a")
	file.write("\nFecha: "+fecha+" Hora: "+hora+"\n")
	for dato in datos:
		print (dato.strip())
		file.write(dato+"\n")
	##file.write(os.linesep)
	file.close()

def recibir_lista_usuarios():
	cadenaTCP=''
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((HOST, PORT))
		s.sendall(b"*")
		try:
			data = s.recv(1024)
			while data:
				data = s.recv(1024)
				if(str(data) != ''):
					cadenaTCP = cadenaTCP + str(data)
			s.close
		except Exception as e:
			print("error: ",e)
			s.close
	return cadenaTCP




cadena=recibir_lista_usuarios()

eliminar_caracteres = "'b\\rn"

for x in range(len(eliminar_caracteres)):
    cadena = cadena.replace(eliminar_caracteres[x],"")

##print(cadena)

datos = cadena.split(";")

datos_leidos(datos)