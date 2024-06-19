#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


void lerPortaSerial(HANDLE hSerial) {
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
                    return;
                }
                index++;
                if (index >= sizeof(buffer) - 1) {
                    buffer[index] = '\0';
                    printf("Buffer cheio, mensagem parcial: %s\n", buffer);
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

int main() {
    const char *portaSerial = "COM5"; 
    HANDLE hSerial = CreateFile(portaSerial, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hSerial == INVALID_HANDLE_VALUE) {
        DWORD dwError = GetLastError();
        printf("Erro ao abrir a porta serial. Código do erro: %lu\n", dwError);
        return 1;
    }

    DCB dcbSerialParams = {0};
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

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        DWORD dwError = GetLastError();
        printf("Erro ao configurar timeouts. Código do erro: %lu\n", dwError);
        CloseHandle(hSerial);
        return 1;
    }

    int escolha;
    while (1) {
        printf("\nEscolha a operaçao:\n");
        printf("1. Ler da porta serial\n");
        printf("2. Escrever na porta serial\n");
        printf("3. Sair\n");
        printf("Digite sua escolha: ");
        scanf("%d", &escolha);
        getchar(); 

        if (escolha == 1) {
            lerPortaSerial(hSerial);
        } else if (escolha == 2) {
            escreverPortaSerial(hSerial);
        } else if (escolha == 3) {
            printf("Saindo...\n");
            break;
        } else {
            printf("Escolha inválida. Tente novamente.\n");
        }
    }

    CloseHandle(hSerial);
    return 0;
}
