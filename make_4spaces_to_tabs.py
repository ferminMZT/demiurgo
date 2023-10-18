#!/usr/bin/env python

# \file make_4spaces_to_tabs.py
# Coge el fichero MakeFile en el directorio en curso y lo modifica substituyendo cada 4 espacios en blanco consecutivos
# por un tabulador y lo salva. De esta manera se evitarian ciertos errores en los makes...

# Nombre del archivo Makefile
makefile_path = './Makefile'

# Lee el contenido del Makefile
with open(makefile_path, 'r') as file:
    lines = file.readlines()

# Reemplaza 4 espacios consecutivos por un tabulador en cada línea
new_lines = [line.replace('    ', '\t') for line in lines]

# Escribe el contenido modificado de vuelta al Makefile
with open(makefile_path, 'w') as file:
    file.writelines(new_lines)

print("Se han convertido espacios en blanco a tabulaciones en el Makefile.")