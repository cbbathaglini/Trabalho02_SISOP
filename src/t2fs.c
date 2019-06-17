#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/t2fs.h"
#include "../include/apidisk.h"

#define ERRO -1
#define SUCESSO 0

/*-----------------------------------------------------------------------------
Função:	Informa a identificação dos desenvolvedores do T2FS.
-----------------------------------------------------------------------------*/
int identify2 (char *name, int size) {
    if(strncpy(name, "Gabriel Barros de Paula - 240427 - Carine Bertagnolli Bathaglini- 274715 - Henrique da Silva Barboza  - 272730", size)){
        return SUCESSO;
    }
	return ERRO;
}

/*-----------------------------------------------------------------------------
Função:	Formata logicamente o disco virtual t2fs_disk.dat para o sistema de
		arquivos T2FS definido usando blocos de dados de tamanho 
		corresponde a um múltiplo de setores dados por sectors_per_block.
-----------------------------------------------------------------------------*/
int format2 (int sectors_per_block) {

	BYTE sectorBuffer[SECTOR_SIZE];
	memset(sectorBuffer, '\0', SECTOR_SIZE);

	if(sectors_per_block < 2){
		printf("Numero de Setores muito baixo. Valor mínimo = 2 \n");
		return ERRO;
	}
	else if( (sectors_per_block % 2 ) != 0){
		printf("Um numero impar de setores p/ bloco não aproveita os setores do disco, tente um numero par.\n");
		return ERRO;
	}
	 
	 t_MBR mbr  = readsMBR(); // Lê dados do MBR para criar o superbloco

	 if(sectors_per_block > mbr.SetorFinalParticao1){
		 printf("Não há setores suficientes na partição do Disco\n");
		 return ERRO;
	 }

	 t_SUPERBLOCO superbloco;

	 superbloco.numBlocos = mbr.SetorFinalParticao1 / sectors_per_block;
	 superbloco.setoresPorBloco = sectors_per_block;
	 superbloco.bitmap=0;

	 //////CÁLCULO DO TAMANHO DA FAT :
	 int fatBytes = 4 * superbloco.numBlocos;		// FAT vai ser unsigned int = 4 bytes
	 superbloco.tamanhoFAT = fatBytes / SECTOR_SIZE;	//Tamanho da FAT em Setores


	 //No pior dos casos, FAT ocupa 8 setores (2 setores p/ bloco) e 2 blocos
	 //se não puder fazer IF, diretório raiz então começa a partir do suposto bloco 4 = Setor 7 (posição fixa)

	 if(sectors_per_block==2){
		 superbloco.blocoDirRaiz = 6;
	 }
	 else if(sectors_per_block==4){
		 superbloco.blocoDirRaiz = 3;
	 }
	 else{
		 superbloco.blocoDirRaiz = 2;
	 }
	
	memcpy(sectorBuffer, &superbloco, SECTOR_SIZE);
	int teste = write_sector(SUPERBLOCKSECTOR, sectorBuffer);
	
	return teste;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um novo arquivo no disco e abrí-lo,
		sendo, nesse último aspecto, equivalente a função open2.
		No entanto, diferentemente da open2, se filename referenciar um 
		arquivo já existente, o mesmo terá seu conteúdo removido e 
		assumirá um tamanho de zero bytes.
-----------------------------------------------------------------------------*/
FILE2 create2 (char *filename) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para remover (apagar) um arquivo do disco. 
-----------------------------------------------------------------------------*/
int delete2 (char *filename) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função que abre um arquivo existente no disco.
-----------------------------------------------------------------------------*/
FILE2 open2 (char *filename) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para fechar um arquivo.
-----------------------------------------------------------------------------*/
int close2 (FILE2 handle) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para realizar a leitura de uma certa quantidade
		de bytes (size) de um arquivo.
-----------------------------------------------------------------------------*/
int read2 (FILE2 handle, char *buffer, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para realizar a escrita de uma certa quantidade
		de bytes (size) de  um arquivo.
-----------------------------------------------------------------------------*/
int write2 (FILE2 handle, char *buffer, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para truncar um arquivo. Remove do arquivo 
		todos os bytes a partir da posição atual do contador de posição
		(current pointer), inclusive, até o seu final.
-----------------------------------------------------------------------------*/
int truncate2 (FILE2 handle) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Altera o contador de posição (current pointer) do arquivo.
-----------------------------------------------------------------------------*/
int seek2 (FILE2 handle, DWORD offset) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um novo diretório.
-----------------------------------------------------------------------------*/
int mkdir2 (char *pathname) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para remover (apagar) um diretório do disco.
-----------------------------------------------------------------------------*/
int rmdir2 (char *pathname) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para alterar o CP (current path)
-----------------------------------------------------------------------------*/
int chdir2 (char *pathname) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para obter o caminho do diretório corrente.
-----------------------------------------------------------------------------*/
int getcwd2 (char *pathname, int size) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função que abre um diretório existente no disco.
-----------------------------------------------------------------------------*/
DIR2 opendir2 (char *pathname) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para ler as entradas de um diretório.
-----------------------------------------------------------------------------*/
int readdir2 (DIR2 handle, DIRENT2 *dentry) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para fechar um diretório.
-----------------------------------------------------------------------------*/
int closedir2 (DIR2 handle) {
	return -1;
}

/*-----------------------------------------------------------------------------
Função:	Função usada para criar um caminho alternativo (softlink) com
		o nome dado por linkname (relativo ou absoluto) para um 
		arquivo ou diretório fornecido por filename.
-----------------------------------------------------------------------------*/
int ln2 (char *linkname, char *filename) {
	return -1;
}


///////////////////////////FUNÇÕES AUXILIARES////////////////////////////

//Lê os dados do MBR (Sector 0) e retorna um MBR preenchido.
t_MBR readsMBR(){

	unsigned char sectorBuffer[SECTOR_SIZE];
	int teste = read_sector(0,sectorBuffer);

	if( teste != 0 ){
		printf("não leu\n");
		return;
	}
	else {

		t_MBR mbr;

		short* versao = (short*) sectorBuffer;
		mbr.versaoDisco = (int) *versao;
	//	printf("Versão Disco %d \n", mbr.versaoDisco);

		short* tamSetor = (short*)(sectorBuffer+2);
		mbr.tamSetor = (int) *tamSetor;
	//	printf("tamSetor %d \n", mbr.tamSetor);

		short* ByteInicialTabeladeParticoes = (short*)(sectorBuffer+4);
		mbr.BInicialTabela = (int) *ByteInicialTabeladeParticoes;
	//	printf("Byte Inicial Tabela de particoes -: %d \n",mbr.BInicialTabela);

		short* numeroParticoes = (short*)(sectorBuffer+6);
		mbr.NumParticoes = (int) *numeroParticoes;
	//	printf("Numero de Particoes: %d \n", mbr.NumParticoes);

		int* setorInicioPart1 = (int*)(sectorBuffer+8);
		mbr.SetorInicialParticao1 = *setorInicioPart1;
	//	printf("%d\n", mbr.SetorInicialParticao1);

		int* setorFimPart1 = (int*)(sectorBuffer+12);
		mbr.SetorFinalParticao1 = *setorFimPart1;
	//	printf("%d\n", mbr.SetorFinalParticao1);

		


		return mbr;

		}


}

t_SUPERBLOCO readsSuperblock(){

	unsigned char sectorBuffer[SECTOR_SIZE];
	int teste = read_sector(SUPERBLOCKSECTOR,sectorBuffer);

	if( teste != 0 ){
		printf("não leu\n");
		return;
	}
	else {

		t_SUPERBLOCO superbloco;

		short* numblocos = (short*) sectorBuffer;
		superbloco.numBlocos = *numblocos;

		short* setores = (short*)(sectorBuffer+2);
		superbloco.setoresPorBloco = *setores;

		short* tamFAT = (short*)(sectorBuffer+4);
		superbloco.tamanhoFAT = *tamFAT;

		short* dirRaiz = (short*)(sectorBuffer+6);
		superbloco.blocoDirRaiz = *dirRaiz;

		int* bitm = (int*)(sectorBuffer+8);
		superbloco.bitmap = *bitm;

		return superbloco;
	}
}


