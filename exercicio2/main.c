// EXERCICIO 2
// LUCAS BORBA

#include <stdio.h>
#include <stdbool.h>

#define STX 0x02  // Início da transmissão
#define ETX 0x03  // Fim da transmissão

// Macros de testes
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testesExecutados++; \
                                if (mensagem) return mensagem; } while (0)

// Estados do transmissor
typedef enum {
    TRANSMISSOR_OCIOSO,          // Estado ocioso
    TRANSMISSOR_STX,             // Envio de STX
    TRANSMISSOR_QTD,             // Envio da quantidade de dados
    TRANSMISSOR_DADOS,           // Envio dos dados
    TRANSMISSOR_CHK,             // Envio do checksum
    TRANSMISSOR_ETX              // Envio do ETX (fim)
} EstadoTransmissor;

// Estados do receptor
typedef enum {
    RECEPTOR_OCIOSO,             // Estado ocioso
    RECEPTOR_STX,                // Recebendo STX
    RECEPTOR_QTD,                // Recebendo quantidade de dados
    RECEPTOR_DADOS,              // Recebendo dados
    RECEPTOR_CHK,                // Recebendo checksum
    RECEPTOR_ETX                 // Recebendo ETX (fim)
} EstadoReceptor;

// Função para calcular checksum simples (soma dos dados)
unsigned char calcularChecksum(char* dados, unsigned char qtd) {
    unsigned char checksum = 0;
    for (int i = 0; i < qtd; i++) {
        checksum += dados[i];
    }
    return checksum;
}

// Função do transmissor usando FSM
void fsmTransmissor(char* dados, unsigned char qtdDados) {
    EstadoTransmissor estado = TRANSMISSOR_OCIOSO;
    unsigned char checksum = calcularChecksum(dados, qtdDados);
    bool transmissaoCompleta = false;
    int i = 0;

    while (!transmissaoCompleta) {
        switch (estado) {
            case TRANSMISSOR_OCIOSO:
                printf("Transmissor: Estado OCIOSO.\n");
                estado = TRANSMISSOR_STX;
                break;

            case TRANSMISSOR_STX:
                printf("Transmissor: Enviando STX.\n");
                printf("STX: 0x%02X\n", STX);
                estado = TRANSMISSOR_QTD;
                break;

            case TRANSMISSOR_QTD:
                printf("Transmissor: Enviando quantidade de dados.\n");
                printf("QTD_DADOS: %d\n", qtdDados);
                estado = TRANSMISSOR_DADOS;
                break;

            case TRANSMISSOR_DADOS:
                printf("Transmissor: Enviando dados.\n");
                for (i = 0; i < qtdDados; i++) {
                    printf("Dado[%d]: %c\n", i, dados[i]);
                }
                estado = TRANSMISSOR_CHK;
                break;

            case TRANSMISSOR_CHK:
                printf("Transmissor: Enviando checksum.\n");
                printf("CHK: 0x%02X\n", checksum);
                estado = TRANSMISSOR_ETX;
                break;

            case TRANSMISSOR_ETX:
                printf("Transmissor: Enviando ETX.\n");
                printf("ETX: 0x%02X\n", ETX);
                transmissaoCompleta = true;
                estado = TRANSMISSOR_OCIOSO;
                break;

            default:
                printf("Transmissor: Estado desconhecido.\n");
                estado = TRANSMISSOR_OCIOSO;
                break;
        }
    }
}

// Função do receptor usando FSM
void fsmReceptor(char* dadosRecebidos, unsigned char qtdDados, unsigned char chkRecebido) {
    EstadoReceptor estado = RECEPTOR_OCIOSO;
    unsigned char checksumCalculado = calcularChecksum(dadosRecebidos, qtdDados);
    bool recepcaoCompleta = false;
    int i = 0;

    while (!recepcaoCompleta) {
        switch (estado) {
            case RECEPTOR_OCIOSO:
                printf("Receptor: Estado OCIOSO.\n");
                estado = RECEPTOR_STX;
                break;

            case RECEPTOR_STX:
                printf("Receptor: Recebendo STX.\n");
                printf("STX recebido: 0x%02X\n", STX);
                estado = RECEPTOR_QTD;
                break;

            case RECEPTOR_QTD:
                printf("Receptor: Recebendo quantidade de dados.\n");
                printf("QTD_DADOS recebido: %d\n", qtdDados);
                estado = RECEPTOR_DADOS;
                break;

            case RECEPTOR_DADOS:
                printf("Receptor: Recebendo dados.\n");
                for (i = 0; i < qtdDados; i++) {
                    printf("Dado[%d]: %c\n", i, dadosRecebidos[i]);
                }
                estado = RECEPTOR_CHK;
                break;

            case RECEPTOR_CHK:
                printf("Receptor: Recebendo checksum.\n");
                printf("CHK recebido: 0x%02X\n", chkRecebido);
                if (checksumCalculado == chkRecebido) {
                    printf("Checksum correto!\n");
                } else {
                    printf("Erro de checksum! Calculado: 0x%02X, Recebido: 0x%02X\n", checksumCalculado, chkRecebido);
                }
                estado = RECEPTOR_ETX;
                break;

            case RECEPTOR_ETX:
                printf("Receptor: Recebendo ETX.\n");
                printf("ETX recebido: 0x%02X\n", ETX);
                recepcaoCompleta = true;
                estado = RECEPTOR_OCIOSO;
                break;

            default:
                printf("Receptor: Estado desconhecido.\n");
                estado = RECEPTOR_OCIOSO;
                break;
        }
    }
}

// Testes
char* teste_calcularChecksum() {
    char dados[] = {'T', 'E', 'S', 'T', 'E'};
    unsigned char esperado = 'T' + 'E' + 'S' + 'T' + 'E'; // Soma dos valores ASCII
    unsigned char resultado = calcularChecksum(dados, sizeof(dados));
    verifica("Teste de cálculo de checksum falhou!", resultado == esperado);
    return NULL; // Teste passou
}

char* teste_transmissaoRecebimento() {
    char dados[] = {'T', 'E', 'S', 'T', 'E', ' ', 'F', 'S', 'M'};
    unsigned char qtdDados = sizeof(dados);
    unsigned char checksum = calcularChecksum(dados, qtdDados);

    // Simulando transmissão e recebimento
    printf("Iniciando FSM do Transmissor:\n");
    fsmTransmissor(dados, qtdDados);

    printf("\nIniciando FSM do Receptor:\n");
    fsmReceptor(dados, qtdDados, checksum);

    return NULL; // Teste passou
}

int main() {
    unsigned int testesExecutados = 0;

    // Executando testes
    printf("Executando testes...\n");
    executa_teste(teste_calcularChecksum);
    executa_teste(teste_transmissaoRecebimento);

    printf("Todos os testes passaram!\n");
    return 0;
}
