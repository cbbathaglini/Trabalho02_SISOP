#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/t2fs.h"
#include "../include/apidisk.h"

void help() {
	
	printf ("Testing program - read and write setores do arquivo t2fs_disk.dat\n");
	printf ("   DISPLAY - d <setor>\n");
	printf ("\n");
	printf ("   HELP    - ?\n");
	printf ("   FIM     - f\n");
}

int main(int argc, char *argv[])
{
	int i;
	int teste = format2(4);
//	printf("%d\n", teste);

	initializeEverything();


	/* printf("Número de blocos %d \n", superbloco.numBlocos);
	printf("Setores Por Bloco %d\n", superbloco.setoresPorBloco);
	printf("Tamanho em Setores da FAT %d\n", superbloco.tamanhoFAT);
	printf("Bloco do Diretório Raiz %d\n", superbloco.blocoDirRaiz);
	printf("Número decimal do bitmap %d\n", superbloco.bitmap); */
	
	

    return 0;
}
