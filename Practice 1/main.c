/* 
 * File:   main.c
 * Author: Jose Lopez
 *C.I.:26.893.008
 *Asigantura: Microprocessors I
 *
 * Created on 30 de abril de 2024, 09:02 AM
 */

#include <stdio.h>
#include <stdlib.h>

int contar_unos(int *arreglo, int num_elementos) {
    int contador = 0;
    
    for (int i = 0; i <= num_elementos; i++) {
        int valor = arreglo[i];
        
        while (valor != 0) {
            if (valor & 1) {
                contador++;
            }
            valor = valor >> 1;
        }
    }
    
    return contador;
}

int main() {
    int arreglo[100];
    
    // Llenar el arreglo con valores iguales al doble del índice correspondiente
    for (int i = 0; i < 100; i++) {
        arreglo[i] = i * 2;
    }
    
    // Llamar a la función contar_unos con el arreglo y el número de elementos deseados
    volatile int num_unos = contar_unos(arreglo, 10);
    
    return num_unos;
    printf("%f",num_unos);
}
