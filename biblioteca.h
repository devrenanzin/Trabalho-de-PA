#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

typedef enum tipo {
    LER = 1,
    ESCREVER,
    SAIR,
    IMPRIMIR
} TipoTela;

typedef struct backup {
    char info[40];
} Backup;

typedef struct no {
    Backup info;
    struct no *proximo;
    struct no *anterior;
} No;

typedef struct lista {
    No *primeiro;
    No *marcador;
    No *ultimo;
    int contador;
} Lista;

void inicializarListinha(Lista *listinha);
void lerPortaSerial(HANDLE hSerial, int vai, Lista *listinha);
void escreverPortaSerial(HANDLE hSerial);
No* criaNo(Backup salva);
void addNoNaLista(No *no, Lista *list);
Lista crialista();
void imprimirBackupinho(Backup salva);
void imprimirBackup(Lista *list);
