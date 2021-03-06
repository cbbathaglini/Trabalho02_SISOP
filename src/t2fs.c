#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/t2fs.h"
#include "../include/apidisk.h"

#define ERRO -1
#define SUCESSO 0
#define MAXBLOCOS 256

int entradasPorDir; //Número máximo de entradas em um diretório
int initialized = 0; //Flag para saber se estruturas do sistema foram carregadas do disco
int entriesPerSector=0; //
int tamBloco;
int lastindex=0;
int entradasPorSet;

t_MBR mbr;
t_SUPERBLOCO superbloco;
DWORD* fat;
t_entradaDir* raiz;
unsigned int bitmap[MAXBLOCOS];




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

	int i;

	BYTE sectorBuffer[SECTOR_SIZE];
	memset(sectorBuffer, '\0', SECTOR_SIZE);

	if(sectors_per_block < 4){
		printf("Numero de Setores muito baixo. Valor mínimo = 4 \n");
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

	 t_SUPERBLOCO superblock;

	 superblock.numBlocos = mbr.SetorFinalParticao1 / sectors_per_block;
	 superblock.setoresPorBloco = sectors_per_block;
	
	 for(i=0;i<4;i++){
		 superblock.bitmap[i] = 0;
	 }
	 
	 

	 //////CÁLCULO DO TAMANHO DA FAT :
	 int fatBytes = 4 * superbloco.numBlocos;		// FAT vai ser unsigned int = 4 bytes
	 superblock.tamanhoFAT = fatBytes / SECTOR_SIZE;	//Tamanho da FAT em Setores


	 //No pior dos casos, FAT ocupa 8 setores (2 setores p/ bloco) e 2 blocos
	 //se não puder fazer IF, diretório raiz então começa a partir do suposto bloco 4 = Setor 7 (posição fixa)

	/*if(sectors_per_block==2){
		 superblock.blocoDirRaiz = 6;
	 }
	 else  */
	 
	 if(sectors_per_block==4){
		 superblock.blocoDirRaiz = 3;
	 }
	 else{
		 superblock.blocoDirRaiz = 2;
	 }
	
	memcpy(sectorBuffer, &superblock, SECTOR_SIZE);
	int teste = write_sector(SUPERBLOCKSECTOR, sectorBuffer);		//Grava Superbloco
	
	//Cria um vetor de entrada de diretórios pra ser o diretório raiz
	//Marca todos como desocupado (filetype=0)
	//passa todos esses dados pra um vetorzão pra ser gravado
	//Grava diretório raiz:

	
	entradasPorDir = (SECTOR_SIZE*sectors_per_block) / sizeof(t_entradaDir); //Checar Conta
	
	t_entradaDir entradas[entradasPorDir];

	for(i=0;i<entradasPorDir;i++){

		entradas[i].fileType=0;
		
	}

	BYTE writingBuffer[SECTOR_SIZE*sectors_per_block]; // Tamanho bloco em bytes = tamanhosetor*setoresPorBloco

	memcpy(writingBuffer,entradas,sizeof(entradas));

	int teste2 = writeBlock(writingBuffer,superblock.blocoDirRaiz,superblock.blocoDirRaiz,sectors_per_block);

	if(teste ==1 && teste2 ==1){
		return SUCESSO;
	}
	else{
		return ERRO;
	}
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
	int i;

	if( teste != 0 ){
		printf("não leu\n");
		return;
	}
	else {

		t_SUPERBLOCO superblock;

		short* numblocos = (short*) sectorBuffer;
		superblock.numBlocos = *numblocos;

		short* setores = (short*)(sectorBuffer+2);
		superblock.setoresPorBloco = *setores;

		short* tamFAT = (short*)(sectorBuffer+4);
		superblock.tamanhoFAT = *tamFAT;

		short* dirRaiz = (short*)(sectorBuffer+6);
		superblock.blocoDirRaiz = *dirRaiz;

		
		int* bitmap1 = (int*)(sectorBuffer+8);
		int* bitmap2 = (int*)(sectorBuffer+12);
		int* bitmap3 = (int*)(sectorBuffer+16);
		int* bitmap4 = (int*)(sectorBuffer+20);

		superblock.bitmap[0]= *bitmap1;
		superblock.bitmap[1]= *bitmap2;
		superblock.bitmap[2]= *bitmap3;
		superblock.bitmap[3]= *bitmap4;

		

		return superblock;
	}
}


//Loads the bitmap with 32 bits of "decimal"
void buildsBitmap(int decimal,int piece){

	int i, result[32];

	for(i=0;i<32;i++)
		result[i]=0;
	
	for(i=0;decimal>0;i++){
		result[i]=decimal%2;
		decimal=decimal/2;
	}

	for(i=32-1;i>=0;i--){
		bitmap[32-1-i + (piece*32)] = result[i];
	}

	return;
}

int initializeFAT(){

	BYTE sector[SECTOR_SIZE];
	int k;

	entriesPerSector = SECTOR_SIZE/4;
	lastindex = (superbloco.tamanhoFAT * entriesPerSector)-1;

	fat = malloc(SECTOR_SIZE * superbloco.tamanhoFAT);

	for(k=0;k < superbloco.tamanhoFAT; k++){

		if(read_sector((FATSECTOR+k), sector) != 0){
			printf("não leu\n");
			return ERRO;
		}
		else{
			memcpy(fat + (entriesPerSector * k), sector, SECTOR_SIZE);
		}
	}

	return SUCESSO;
}

void initializeEverything(){

	int i;
	
	mbr = readsMBR();
	
	superbloco = readsSuperblock();
	
	for(i=0;i<4;i++){
		buildsBitmap(superbloco.bitmap[i],i);
	}

	int teste = initializeFAT();
	
	if(teste != 0){
		printf("Não alocou FAT \n");
		return;
	}

	entradasPorDir = (SECTOR_SIZE* superbloco.setoresPorBloco) / sizeof(t_entradaDir); //Checar Conta
	entradasPorSet = SECTOR_SIZE/sizeof(t_entradaDir);
	tamBloco = SECTOR_SIZE*superbloco.setoresPorBloco;

	for(i=0;i<superbloco.blocoDirRaiz;i++){
		bitmap[i]=1;						//Blocos antes do diretório raiz, estão já ocupados
		fat[i]=1;					    	// pelo superbloco e pela fat.
	}
	
	int teste2 = initializeRoot();
	if(teste2==ERRO){
		printf("root not initiated\n");
		return;
	}

	/* for(i=0; i< superbloco.blocoDirRaiz;i++ ){

		printf("indice bitmap %d\n", bitmap[i]);		//Prints de teste de alocação
		printf("counteúdo fat %d\n", fat[i]);

	}*/
	
	for(i=0;i<32;i++){
		printf("%d",bitmap[i+32]);
	}
	printf("\n");

	initialized=1;	//everything setup!
	return;
	

}

int initializeRoot(){

	raiz=malloc(tamBloco);
	int i;
	BYTE sector[SECTOR_SIZE];
	int dataSectorStart = superbloco.setoresPorBloco *superbloco.blocoDirRaiz; //CHECAR SE CONTA ESTÁ CERTA

	for(i=0;i<superbloco.setoresPorBloco;i++){

		int position = dataSectorStart + (superbloco.blocoDirRaiz * superbloco.setoresPorBloco);
		if( read_sector(position+i,sector) != 0 ){

			printf("Não gravou bloco\n");
			return ERRO;
		}

		memcpy(raiz+ (entradasPorSet*i),sector,SECTOR_SIZE);
	}

	bitmap[superbloco.blocoDirRaiz]=1;
	return SUCESSO;

}


int writeBlock(BYTE *buffer,int block, int raiz, int setoresPBloco){

	int i;
	int dataSectorStart = setoresPBloco*raiz; //CHECAR SE CONTA ESTÁ CERTA
	BYTE sector[SECTOR_SIZE];
	memset(sector,'\0',SECTOR_SIZE);

	for(i=0;i<setoresPBloco;i++){

		int position = dataSectorStart + (block * setoresPBloco);
		memcpy(sector,(buffer + (SECTOR_SIZE*i)), SECTOR_SIZE);

		if((write_sector(position+i,sector)) !=0){
			return ERRO;
		}
	}
	return SUCESSO;
}


