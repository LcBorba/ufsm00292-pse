// EXERCICIO 3
// LUCAS BORBA

#include <stdio.h>
#include <stdbool.h>

#define STX 0x02  // Início da transmissão
#define ETX 0x03  // Fim da transmissão

// Macros de testes
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testes_executados++; \
                                if (mensagem) return mensagem; } while (0)

int testes_executados = 0;

// Estados do transmissor
typedef enum {
    TRANSMISSOR_IDLE,
    TRANSMISSOR_STX,
    TRANSMISSOR_QTD,
    TRANSMISSOR_DATA,
    TRANSMISSOR_CHK,
    TRANSMISSOR_ETX
} EstadoTransmissor;

// Variáveis do transmissor
typedef void (*FuncaoEstadoTransmissor)();
FuncaoEstadoTransmissor estadoAtualTransmissor;

char* dadosTransmissor;
unsigned char qtdDadosTransmissor;
unsigned char checksumTransmissor;
int indiceTransmissor = 0;

void estadoTransmissorIdle();
void estadoTransmissorSTX();
void estadoTransmissorQTD();
void estadoTransmissorData();
void estadoTransmissorCHK();
void estadoTransmissorETX();

// Função para calcular checksum
unsigned char calculaChecksum(char* dados, unsigned char qtd) {
    unsigned char checksum = 0;
    for (int i = 0; i < qtd; i++) {
        checksum += dados[i];
    }
    return checksum;
}

void inicializaFSMTransmissor(char* dados, unsigned char qtdDados) {
    dadosTransmissor = dados;
    qtdDadosTransmissor = qtdDados;
    checksumTransmissor = calculaChecksum(dados, qtdDados);
    indiceTransmissor = 0;
    estadoAtualTransmissor = estadoTransmissorSTX;
}

// Implementação dos estados do transmissor
void estadoTransmissorIdle() {
    printf("Transmissor: Estado IDLE.\n");
    estadoAtualTransmissor = NULL;
}
void estadoTransmissorSTX() {
    printf("Transmissor: Enviando STX.\n");
    printf("STX: 0x%02X\n", STX);
    estadoAtualTransmissor = estadoTransmissorQTD;
}
void estadoTransmissorQTD() {
    printf("Transmissor: Enviando quantidade de dados.\n");
    printf("QTD_DADOS: %d\n", qtdDadosTransmissor);
    estadoAtualTransmissor = estadoTransmissorData;
}
void estadoTransmissorData() {
    if (indiceTransmissor < qtdDadosTransmissor) {
        printf("Transmissor: Enviando dado[%d]: %c\n", indiceTransmissor, dadosTransmissor[indiceTransmissor]);
        indiceTransmissor++;
    } else {
        estadoAtualTransmissor = estadoTransmissorCHK;
    }
}
void estadoTransmissorCHK() {
    printf("Transmissor: Enviando checksum.\n");
    printf("CHK: 0x%02X\n", checksumTransmissor);
    estadoAtualTransmissor = estadoTransmissorETX;
}
void estadoTransmissorETX() {
    printf("Transmissor: Enviando ETX.\n");
    printf("ETX: 0x%02X\n", ETX);
    estadoAtualTransmissor = estadoTransmissorIdle;
}

// Função para executar a FSM do transmissor
void executaFSMTransmissor() {
    while (estadoAtualTransmissor != NULL) {
        estadoAtualTransmissor();
    }
}

// Teste da FSM do Transmissor
char* teste_fsmTransmissor() {
    char dados[] = {'A', 'B', 'C'};
    inicializaFSMTransmissor(dados, 3);
    executaFSMTransmissor();
    verifica("Erro: FSM do transmissor não retornou para o estado IDLE", estadoAtualTransmissor == NULL);
    verifica("Erro: Checksum calculado incorretamente", checksumTransmissor == ('A' + 'B' + 'C'));
    return NULL;
}

// Estados do receptor
typedef enum {
    RECEPTOR_IDLE,
    RECEPTOR_STX,
    RECEPTOR_QTD,
    RECEPTOR_DATA,
    RECEPTOR_CHK,
    RECEPTOR_ETX
} EstadoReceptor;

// Variáveis do receptor
typedef void (*FuncaoEstadoReceptor)();
FuncaoEstadoReceptor estadoAtualReceptor;

char* dadosReceptor;
unsigned char qtdDadosReceptor;
unsigned char checksumReceptor;
int indiceReceptor = 0;

void estadoReceptorIdle();
void estadoReceptorSTX();
void estadoReceptorQTD();
void estadoReceptorData();
void estadoReceptorCHK();
void estadoReceptorETX();

void inicializaFSMReceptor(char* dados, unsigned char qtdDados, unsigned char chk) {
    dadosReceptor = dados;
    qtdDadosReceptor = qtdDados;
    checksumReceptor = chk;
    indiceReceptor = 0;
    estadoAtualReceptor = estadoReceptorSTX;
}

// Implementação dos estados do receptor
void estadoReceptorIdle() {
    printf("Receptor: Estado IDLE.\n");
    estadoAtualReceptor = NULL;
}
void estadoReceptorSTX() {
    printf("Receptor: Recebendo STX.\n");
    printf("STX recebido: 0x%02X\n", STX);
    estadoAtualReceptor = estadoReceptorQTD;
}
void estadoReceptorQTD() {
    printf("Receptor: Recebendo quantidade de dados.\n");
    printf("QTD_DADOS recebido: %d\n", qtdDadosReceptor);
    estadoAtualReceptor = estadoReceptorData;
}
void estadoReceptorData() {
    if (indiceReceptor < qtdDadosReceptor) {
        printf("Receptor: Recebendo dado[%d]: %c\n", indiceReceptor, dadosReceptor[indiceReceptor]);
        indiceReceptor++;
    } else {
        estadoAtualReceptor = estadoReceptorCHK;
    }
}
void estadoReceptorCHK() {
    printf("Receptor: Recebendo checksum.\n");
    printf("CHK recebido: 0x%02X\n", checksumReceptor);
    unsigned char checksumCalculado = calculaChecksum(dadosReceptor, qtdDadosReceptor);

    if (checksumCalculado == checksumReceptor) {
        printf("Checksum correto!\n");
    } else {
        printf("Erro de checksum! Calculado: 0x%02X, Recebido: 0x%02X\n", checksumCalculado, checksumReceptor);
        estadoAtualReceptor = estadoReceptorIdle;
    }
    estadoAtualReceptor = estadoReceptorETX;
}
void estadoReceptorETX() {
    printf("Receptor: Recebendo ETX.\n");
    printf("ETX recebido: 0x%02X\n", ETX);
    estadoAtualReceptor = estadoReceptorIdle;
}

// Função para executar a FSM do receptor
void executaFSMReceptor() {
    while (estadoAtualReceptor != NULL) {
        estadoAtualReceptor();
    }
}

// Teste da FSM do Receptor
char* teste_fsmReceptor() {
    char dados[] = {'A', 'B', 'C'};
    unsigned char chk = calculaChecksum(dados, 3);
    inicializaFSMReceptor(dados, 3, chk);
    executaFSMReceptor();
    verifica("Erro: FSM do receptor não retornou para o estado IDLE", estadoAtualReceptor == NULL);
    return NULL;
}

// Teste da função calculaChecksum
char* teste_calculaChecksum() {
    char dados[] = {'A', 'B', 'C'};
    verifica("Erro: Checksum incorreto", calculaChecksum(dados, 3) == ('A' + 'B' + 'C'));
    return NULL;
}

// Função principal de execução de testes
char* todosOsTestes() {
    executa_teste(teste_calculaChecksum);
    executa_teste(teste_fsmTransmissor);
    executa_teste(teste_fsmReceptor);
    return NULL;
}

int main() {
    char* resultado = todosOsTestes();
    if (resultado != NULL) {
        printf("%s\n", resultado);
    } else {
        printf("TODOS OS TESTES PASSARAM!\n");
    }
    printf("Testes executados: %d\n", testes_executados);

    printf("\nIniciando FSM do Transmissor:\n");
    char dados[] = {'T', 'E', 'S', 'T', 'E', ' ', 'P', 'F'};
    unsigned char qtdDados = sizeof(dados);
    unsigned char checksum = calculaChecksum(dados, qtdDados);
    inicializaFSMTransmissor(dados, qtdDados);
    executaFSMTransmissor();

    printf("\nIniciando FSM do Receptor:\n");
    inicializaFSMReceptor(dados, qtdDados, checksum);
    executaFSMReceptor();

    return resultado != NULL;
}
