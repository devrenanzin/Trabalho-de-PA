#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "biblioteca.h"

#define ZERANDO 0

int main() {
    const char *portaSerial = "COM6"; 
    HANDLE hSerial = CreateFile(portaSerial, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial == INVALID_HANDLE_VALUE) {
        DWORD dwError = GetLastError();
        printf("Erro ao abrir a porta serial. Código do erro: %lu\n", dwError);
        return 1;
    }

    DCB dcbSerialParams = {ZERANDO};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        DWORD dwError = GetLastError();
        printf("Erro ao obter o estado da porta serial. Código do erro: %lu\n", dwError);
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        DWORD dwError = GetLastError();
        printf("Erro ao configurar a porta serial. Código do erro: %lu\n", dwError);
        CloseHandle(hSerial);
        return 1;
    }

    COMMTIMEOUTS timeouts = {ZERANDO};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = ZERANDO;
    timeouts.ReadTotalTimeoutMultiplier = ZERANDO;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        DWORD dwError = GetLastError();
        printf("Erro ao configurar timeouts. Código do erro: %lu\n", dwError);
        CloseHandle(hSerial);
        return 1;
    }

    Lista listinha;
    inicializarListinha(&listinha);

    int escolha;
    while (1) {
        printf("\nEscolha a operacao:\n");
        printf("1 -> Ler da porta serial\n");
        printf("2 -> Escrever na porta serial\n");
        printf("3 -> Encerrar o codigo\n");
        printf("4 -> Historico de mensagens recebidas\n");
        printf("Digite sua escolha: ");
        scanf("%d", &escolha);
        getchar();

        switch (escolha) {
            case LER:
                lerPortaSerial(hSerial,0,&listinha);
                break;
            case ESCREVER:
                escreverPortaSerial(hSerial);
                break;
            case SAIR:
                printf("Saindo...\n");
                CloseHandle(hSerial);
                return 0;

            case IMPRIMIR:
                imprimirBackup(&listinha);
                break;
            default:
                printf("Escolha inválida. Tente novamente.\n");
                break;
        }
    }

    CloseHandle(hSerial);
    return 0;
}
