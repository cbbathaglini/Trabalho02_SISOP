
/**
 *
 * Teste que retorna o nome dos integrantes do grupo
 *
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/t2fs.h"
#include "../include/apidisk.h"

int main(int argc, char *argv[]) {
    int	resultado = -1;
	int tam = 170;
	char *name = (char *)malloc(tam * sizeof(char));

	resultado = identify2(name, tam);
    if(resultado == 0) {
        printf(name);
    }else{
        printf("Erro ao exibir os nomes.\n");
    }

}
