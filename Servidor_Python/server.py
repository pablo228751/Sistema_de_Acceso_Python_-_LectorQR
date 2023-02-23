import socket
import time
import os
from datetime import datetime, timedelta


var=open("config.dat","r")
var_lineas=var.readlines()
var.close()

HOST = var_lineas[4][13:-5].rstrip()
PORT = 999
var_lineas=[]



def grabar_usuComp(lista_usu):
	file = open("usuarios_comp.txt", "w")
	for i in lista_usu:
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
datos_DIC={}

#### LERR ARCHIVO DE USUARIOS Y CARGARLO EN EL DIC
usu_nvos=leer_ARCHIVO("usuarios.txt")
for i in usu_nvos:
	datos_DIC[i.rstrip()]= "*T;"

print("Dic1 con DATOS NUEVOS",datos_DIC)

### LERR ARCHIVO DE USUARIOS_COMP (Copia Local de la Placa), PARA UNIFICAR EN EL DIC
usu_c=leer_ARCHIVO("usuarios_comp.txt")
usu_comp=[]
if usu_c:
	for i in usu_c:
		usu_comp.append(i.rstrip())


if usu_comp:
	for i in usu_comp:
		quitar=''
		datos_DIC[i.rstrip()[:-3]]= i[-3:]
		for k in datos_DIC:			
			if (i.rstrip()[:-3] == k):
				#print("Coincide el de Placa : ",i.rstrip()[:-3], " con archivo nuevo: ",k)
				quitar=k
				break
		if quitar:
			datos_DIC.pop(quitar)
print("Dic2 PARA GRABAR EN PLACA: ",datos_DIC)

### Aqui SUBIR REGISTROS A BASe de Datos de la PLACA

if(datos_DIC):
	cadena_usu=''
	for k,v in datos_DIC.items():
		cadena_usu = cadena_usu+k+v+"\n"
	grabar_base(cadena_usu)

time.sleep(3)

### FIN SUBIR REGISTROS A BASe de Datos de la PLACA




### Sobreescibir usuarios_comp con los Datos Actuales de la base de la Placa
if(datos_DIC):
	cadena=recibir_lista_usuarios()
	if cadena:
		cadena.remove('')
		print("Base de datos PLACA: ",cadena)
		grabar_usuComp(cadena) ### Grabar en usuarios_com.txt
else:
	print("El diccionario está vacio")





"""
### RECIBE LISTA GUARDADA EN LA sd DE LA PLACA
cadena=recibir_lista_usuarios()
eliminar_caracteres = "'b\\rn"

for x in range(len(eliminar_caracteres)):
    cadena = cadena.replace(eliminar_caracteres[x],"").strip()

##print(cadena)

datos = cadena.strip().split(";")
datos.remove('')
print(datos)

datos_leidos(datos) ##Funcion para guardar los datos leidos



## Guardar los != F
for i in datos:	
	if i[-1:] != 'F':
		datos_DIC[i[:-2]]= i[-2:]+";"



print(datos_DIC)




#borrar_base()


cadena_usu=''
for k,v in datos_DIC.items():
	cadena_usu = cadena_usu+k+v


print(cadena_usu)
#grabar_base(cadena_usu)
"""




