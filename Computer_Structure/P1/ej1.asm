.text
.globl main

main:
    la $t0, array1
    la $t2, array2
    la $t1, count
    lw $t1, 0($t1)

Loop:
    lw $t3, 0($t0)
    lw $t4, 0($t2)
    add $t3, $t3, $t4
    sw $t3, 0($t0)
    addi $t0, $t0, 4
    addi $t2, $t2, 4
    addi $t1, $t1, -1
    bne $t1, $0, Loop

fin:
    addi $v0,$0,10 # la llamada para salir del programa
    syscall

# segmento de datos
.data

array1: .word 3, 4, 9, 8, 5, 1
array2: .word 3, 4, 9, 8, 5, 1
count: .word 6

#Este programa guarda en $t0 y en $t2 el mismo array, en $t1 un array con un elemento 
#contador con valor 6, que luego es añadido individualmente a la variable

#Luego entra en un bucle en el que:
#   - Añade a $t3 y $t4 el primer elemento de los respectivos arrays
#   - Guarda en $t3 la suma de estos
#   - Guarda en el primer elemento de $t0 (donde estaba almacenado el primer array)
#     esa suma
#   - Le suma 4 a cada array, lo que se traduce en en el siguiente ciclo del bucle se 
#     trabajará con el siguiente elemento de ambos arrays
#   - Se resta 1 a $t1, donde estaba el contador
#   - Si el contador no llegó a 0 entonces sigue el bucle
#   - Tras el bucle se añade en $v0 el valor 10, lo que significa que termina el programa

#En resumen, el programa guardó en $t0 la suma de los dos arrays