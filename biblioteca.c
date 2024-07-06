#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "biblioteca.h"

void inicializarListinha(Lista *listinha) {
    *listinha = crialista();
}

void lerPortaSerial(HANDLE hSerial, int vai, Lista *listinha) {
    if (vai == 0) {
        char buffer[256];
        DWORD bytesRead;
        int index = 0;

        printf("Aguardando mensagem da porta serial...\n");

        while (1) {
            if (ReadFile(hSerial, &buffer[index], 1, &bytesRead, NULL)) {
                if (bytesRead > 0) {
                    if (buffer[index] == '\n') {
                        buffer[index] = '\0'; 
                        printf("Mensagem recebida: %s\n", buffer);

                        if (strlen(buffer) > 0) {
                            Backup novo_arquivo;
                            strcpy(novo_arquivo.info, buffer);  
                            No *novo_no = criaNo(novo_arquivo);
                            addNoNaLista(novo_no, listinha);
                        } else {
                            printf("Mensagem vazia recebida, não será adicionada à lista.\n");
                        }

                        return;
                    }
                    index++;
                    if (index >= sizeof(buffer) - 1) {
                        buffer[index] = '\0';
                        printf("Buffer cheio, mensagem parcial: %s\n", buffer);

                        if (strlen(buffer) > 0) {
                            Backup novo_arquivo;
                            strcpy(novo_arquivo.info, buffer);  
                            No *novo_no = criaNo(novo_arquivo);
                            addNoNaLista(novo_no, listinha);
                        } else {
                            printf("Mensagem vazia recebida, não será adicionada à lista.\n");
                        }
                        
                        return;
                    }
                }
            } else {
                DWORD dwError = GetLastError();
                printf("Erro ao ler dados. Código do erro: %lu\n", dwError);
                return;
            }

            Sleep(10);
        }
    }
}

void escreverPortaSerial(HANDLE hSerial) {
    char data[256];
    DWORD bytesWritten;

    printf("Digite os dados a serem enviados ou 'exit' para voltar ao menu:\n");

    while (1) {
        fgets(data, sizeof(data), stdin);
        data[strcspn(data, "\n")] = 0; 

        if (strcmp(data, "exit") == 0) {
            break;
        }

        if (!WriteFile(hSerial, data, strlen(data), &bytesWritten, NULL)) {
            DWORD dwError = GetLastError();
            printf("Erro ao enviar dados. Código do erro: %lu\n", dwError);
            return;
        }
        printf("Dados enviados: %s\n", data);
    }
}

No* criaNo(Backup salva) {
    No* novo_no = (No*)malloc(sizeof(No)); // malloc para alocar memória

    novo_no->info = salva;
    novo_no->proximo = NULL; // Indica que está no último nó
    novo_no->anterior = NULL; // Indica que está no último nó
    
    return novo_no;
}

void addNoNaLista(No *no, Lista *list) {
    if (list->contador == 0) {
        list->primeiro = no; 
        list->marcador = no;
        list->ultimo = no;
    } else {
        list->ultimo->proximo = no; 
        list->marcador = list->ultimo;
        list->ultimo = no;
        list->ultimo->anterior = list->marcador;
    }
    list->contador++;
}

Lista crialista() {
    Lista nova_lista = {NULL, NULL, 0}; 
    return nova_lista;
}

void imprimirBackupinho(Backup salva) {
    printf("Informacao: %s\n", salva.info);
    printf("--------------------------------\n");
}

void imprimirBackup(Lista *list) {
    list->marcador = list->primeiro;

    while (list->marcador != NULL) {
        imprimirBackupinho(list->marcador->info);
        list->marcador = list->marcador->proximo;
    }
}
