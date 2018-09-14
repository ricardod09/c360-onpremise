#Python 101 en 10min (asi se comenta el codigo)
#Ejecutar este programa en cmd: python python101.py

#Importar librerias, si deseas instalar una nueva
#enel cmd  deberas ejecutar pip install <libreria>
import sys

#Variables
#No necesitan ser declaradas :)
entero = 5
decimal = 1.5
cadena = "Hola"
cadena2 = 'Chao'
booleano = True
array = ['uno','dos','tres']
diccionario = {"nombre":"diego","apellido":"montufar","edad":30}
nulo = None

#Constantes
MAX_NUM = 1000

#Funciones
def saludar():
    print(cadena)

#Funciones con parametros
def saludarlo(saludo):
    print(saludo)

#Funciones retornan valores, no es necesario especificar el tipo de datos
#Es posible asignar valores por defecto a los parametros, si no se pasa el parametro,
#tomara el valor por defecto
def saludarte(despido,saludo=cadena2):
    print(saludo)
    return despido

#Iteraciones, es similar con bucles while
def iterar():
    for i in array:
        print(i)

#Python sigue un flujo de ejecucion secuencial, por lo que podemos pone codigo
#que no esta dentro de Funciones
total = MAX_NUM * decimal
print(total)

#Este es el analogo al void main de java :)
if __name__== "__main__":
    saludar()
    saludarlo("Holi")
    print(saludarte(despido="chao"))
    iterar()
