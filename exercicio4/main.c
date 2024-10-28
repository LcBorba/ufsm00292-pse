// EXERCICIO 4
// LUCAS BORBA

#include <stdio.h>
#include <stdbool.h>
#include "pt.h"

#define STX 0x02  // Início da transmissão
#define ETX 0x03  // Fim da transmissão

// Função para calcular checksum simples (soma dos dados)
unsigned char calculaChecksum(char* dados, unsigned char quantidade) {
    unsigned char checksum = 0;
    for (int i = 0; i < quantidade; i++) {
        checksum += dados[i];
    }
    return checksum;
}

// Estrutura de dados para FSM do transmissor
struct pt estadoTransmissor;
char* dadosTransmissor;
unsigned char qtdDadosTransmissor;
unsigned char checksumTransmissor;
int indiceTransmissor = 0;

// Estrutura de dados para FSM do receptor
struct pt estadoReceptor;
char* dadosReceptor;
unsigned char qtdDadosReceptor;
unsigned char checksumReceptor;
int indiceReceptor = 0;

// Protothread do transmissor
static PT_THREAD(estadoFSMTransmissor(struct pt *pt)) {
    PT_BEGIN(pt);

    // Enviando STX
    printf("Transmissor: Enviando STX.\n");
    printf("STX: 0x%02X\n", STX);
    PT_YIELD(pt);

    // Enviando quantidade de dados
    printf("Transmissor: Enviando quantidade de dados.\n");
    printf("QTD_DADOS: %d\n", qtdDadosTransmissor);
    PT_YIELD(pt);

    // Enviando dados
    indiceTransmissor = 0;
    while (indiceTransmissor < qtdDadosTransmissor) {
        printf("Transmissor: Enviando dado[%d]: %c\n", indiceTransmissor, dadosTransmissor[indiceTransmissor]);
        indiceTransmissor++;
        PT_YIELD(pt);
    }

    // Enviando checksum
    printf("Transmissor: Enviando checksum.\n");
    printf("CHK: 0x%02X\n", checksumTransmissor);
    PT_YIELD(pt);

    // Enviando ETX
    printf("Transmissor: Enviando ETX.\n");
    printf("ETX: 0x%02X\n", ETX);
    PT_END(pt);
}

// Protothread do receptor
static PT_THREAD(estadoFSMReceptor(struct pt *pt)) {
    PT_BEGIN(pt);

    // Recebendo STX
    printf("Receptor: Recebendo STX.\n");
    printf("STX recebido: 0x%02X\n", STX);
    PT_YIELD(pt);

    // Recebendo quantidade de dados
    printf("Receptor: Recebendo quantidade de dados.\n");
    printf("QTD_DADOS recebido: %d\n", qtdDadosReceptor);
    PT_YIELD(pt);

    // Recebendo dados
    indiceReceptor = 0;
    while (indiceReceptor < qtdDadosReceptor) {
        printf("Receptor: Recebendo dado[%d]: %c\n", indiceReceptor, dadosReceptor[indiceReceptor]);
        indiceReceptor++;
        PT_YIELD(pt);
    }

    // Recebendo checksum
    printf("Receptor: Recebendo checksum.\n");
    printf("CHK recebido: 0x%02X\n", checksumReceptor);
    unsigned char checksumCalculado = calculaChecksum(dadosReceptor, qtdDadosReceptor);
    if (checksumCalculado == checksumReceptor) {
        printf("Checksum correto!\n");
    } else {
        printf("Erro de checksum! Calculado: 0x%02X, Recebido: 0x%02X\n", checksumCalculado, checksumReceptor);
    }
    PT_YIELD(pt);

    // Recebendo ETX
    printf("Receptor: Recebendo ETX.\n");
    printf("ETX recebido: 0x%02X\n", ETX);
    PT_END(pt);
}

int main() {
    char dados[] = {'T', 'E', 'S', 'T', 'E', ' ', 'P', 'T'}; // Exemplo de dados a serem transmitidos
    unsigned char qtdDados = sizeof(dados);
    unsigned char checksum = calculaChecksum(dados, qtdDados);

    // Inicializando protothreads
    PT_INIT(&estadoTransmissor);
    PT_INIT(&estadoReceptor);

    // Simulando o loop principal
    printf("Iniciando FSM do Transmissor:\n");
    while (estadoFSMTransmissor(&estadoTransmissor) != PT_ENDED) {
        // O loop principal do transmissor continua até que a protothread termine
    }

    printf("\nIniciando FSM do Receptor:\n");
    dadosReceptor = dados;
    qtdDadosReceptor = qtdDados;
    checksumReceptor = checksum;
    while (estadoFSMReceptor(&estadoReceptor) != PT_ENDED) {
        // O loop principal do receptor continua até que a protothread termine
    }

    return 0;
}
