import socket
import time
import os
from datetime import datetime, timedelta


var=open("config.dat","r")
var_lineas=var.readlines()
var.close()

HOST = var_lineas[4][13:-5].rstrip()
PORT = 999
cantLineas = int(var_lineas[0][11:])
print("CANT DE LINEAS: ",cantLineas)
var_lineas=[]



def grabar_usuComp(lista_usu):
	file = open("usuarios_comp.txt", "w")
	for i in lista_usu:
		file.write(i+";\n")
	file.close()

def sobreescribir_usuariosTXT(lista_usu):
	file = open("usuarios.txt", "w")
	for i in lista_usu:
		file.write(i+"\n")
	file.close()

def sobreescibir_usuComp(lista_usuComp):
	file = open("usuarios_comp.txt", "w")
	for i in lista_usuComp:
		file.write(i+";\n")
	file.close()

def recibir_lista_usuarios():
	cadenaTCP=''
	datos=[]
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((HOST, PORT))
		s.sendall(b"*")
		try:
			data = s.recv(1024)
			while data:
				data = s.recv(1024)
				if(str(data) != ''):
					cadenaTCP = cadenaTCP + str(data)
			if(cadenaTCP):
				eliminar_caracteres = "'b\\rn"
				for x in range(len(eliminar_caracteres)):
					cadenaTCP = cadenaTCP.replace(eliminar_caracteres[x],"")
					datos = cadenaTCP.split(";")
					#datos.remove('')
			s.close
			return datos
		except Exception as e:
			print("error: ",e)
			s.close
	return datos

def borrar_base():
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((HOST, PORT))
		s.sendall(b"$") 
		s.close

def grabar_base(cadena_usu):
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((HOST, PORT))
		s.sendall(b"#"+str.encode(cadena_usu))
		s.close

def leer_ARCHIVO(txt):
	cadenaUsu=''
	try:
		var=open(txt,"r")
		var_lineas=var.readlines()
		var.close()
		return var_lineas
	except Exception as e:
		#print("Error al leer el archivo: ",e)
		return



#################### MAIN ######################


### 1- Debe sobreescribir usuarios.txt y reducir tamao del archivos si es que tiene mas que los de la variable cantLineas (500) registros
### 2- Crear una lista de usuarios.txt
### 3- Leer la base de la tarjeta SD, crear una lista Actual:
###   a)Eliminar repetidos si es que los tiene y grabar lista definitiva en usuarios_comp
### 4- Comparar y subir la lista a la Base de datos SD definitiva. Actualizar usuarios_comp.txt

datos_DIC={}

#### LERR ARCHIVO DE USUARIOS Y CARGARLO EN EL DIC
usu_nvos=leer_ARCHIVO("usuarios.txt")
tam_UsuPrimario=len(usu_nvos)
for i in range(tam_UsuPrimario):
	usu_nvos[i]=usu_nvos[i].rstrip()

### Acortar lista si supera el tamanio establecio en la variable canLineas a 20 registros
if tam_UsuPrimario > cantLineas:
	usu_nvos = usu_nvos[-20:]
	sobreescribir_usuariosTXT(usu_nvos)

try:
	usu_nvos.remove('')
except Exception as e:
	print("Error: ",e )
print("Lista usuarios.txt: ",usu_nvos)

cadena=recibir_lista_usuarios()
if cadena:
	cadena.remove('')
	print("Lista Base de datos PLACA: ",cadena)





### 1- Agregar a Diccionario con los registros de Usuario.txt primero
if usu_nvos:
	for i in usu_nvos:
		datos_DIC[i]="*T"
print("datos_DIC1: ",datos_DIC)

### 2- Agregar a Diccionario los registro de SD de la placa
if cadena:
	for i in cadena:
		datos_DIC[i[:-2]]=i[-2:]
print("datos_DIC2: ",datos_DIC)


### 3 Eliminar y sobreescribir en usuarios.txt registros con estado=F
if cadena:
	for i in cadena:
		if i[-1:] == 'F':
			usu_nvos.remove(i[:-2])
print("Paso 3 Nueva lista usuarios.txt: ",usu_nvos)
if usu_nvos:
	sobreescribir_usuariosTXT(usu_nvos)

### 4- Borrar base SD
borrar_base();
print("Se borró base SD")

time.sleep(3)

### 5- Grabar en SD Nuevos registros sin los estados= F

if(datos_DIC):
	cadena_usu=''
	for k,v in datos_DIC.items():
		if v[-1:] != 'F':
			cadena_usu = cadena_usu+k+v+";\n"
	grabar_base(cadena_usu)



### 6- Sobreescribir usuarios_com.txt con los datos nuevos y si estado F
lista_usuComp=[]
if(datos_DIC):
	for k,v in datos_DIC.items():
		if v[-1:] != 'F':
			lista_usuComp.append(k+v)
if(lista_usuComp):
	sobreescibir_usuComp(lista_usuComp)





