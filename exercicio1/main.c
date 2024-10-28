// EXERCICIO 1
// LUCAS BORBA

#include <stdio.h>
#include <stdlib.h>

// Macros de testes baseadas em minUnit
#define verifica(mensagem, teste) do { if (!(teste)) return mensagem; } while (0)
#define executa_teste(teste) do { char *mensagem = teste(); testes_executados++; \
                                if (mensagem) return mensagem; } while (0)

int testes_executados = 0;

typedef struct {
    int *buffer;  // Array para armazenar os elementos
    int size;     // Tamanho do buffer
    int begin;    // Índice do início
    int end;      // Índice do fim
    int full;     // Flag para indicar se o buffer está cheio
} Buffer;

// Cria o buffer
Buffer *criar(int size) {
    Buffer *buffer = malloc(sizeof(Buffer));
    if (buffer == NULL) return NULL;

    buffer->buffer = malloc(sizeof(int) * size);
    if (buffer->buffer == NULL) {
        free(buffer);
        return NULL;
    }

    buffer->size = size;
    buffer->begin = 0;
    buffer->end = 0;
    buffer->full = 0;
    return buffer;
}

// Adiciona elemento ao buffer
int adiciona(int x, Buffer *buffer) {
    if (buffer->full) return -1;  // Buffer cheio, não adiciona novo elemento

    buffer->buffer[buffer->end] = x;                    // adiciona novo elemento
    buffer->end = (buffer->end + 1) % buffer->size;

    if (buffer->end == buffer->begin) {
        buffer->full = 1;  // indica quando o buffer está cheio após adicionar o novo elemento
    }

    return 1;
}

// Lê e remove um elemento do buffer
int leitura(Buffer *buffer) {
    if (buffer->begin == buffer->end && !buffer->full) return -1;  // Buffer vazio, nada pode ser lido ou removido

    int valor = buffer->buffer[buffer->begin];
    buffer->begin = (buffer->begin + 1) % buffer->size;
    buffer->full = 0;  // indica que o buffer não está cheio

    return valor;       // retorna elemento lido
}

// Libera a memória alocada para o buffer
void liberar(Buffer *buffer) {
    free(buffer->buffer);
    free(buffer);
}

// Testes do buffer circular
static char *teste_criacao() {
    Buffer *buffer = criar(5);
    verifica("erro: buffer não criado corretamente", buffer != NULL);
    verifica("erro: tamanho do buffer incorreto", buffer->size == 5);
    liberar(buffer);
    return 0;
}

static char *teste_adiciona() {                                              // testa se o codigo adiciona elementos ao buffer e indica qunado está cheio
    Buffer *buffer = criar(3);
    verifica("erro: adicionar ao buffer falhou", adiciona(1, buffer) == 1);
    verifica("erro: adicionar ao buffer falhou", adiciona(2, buffer) == 1);
    verifica("erro: adicionar ao buffer falhou", adiciona(3, buffer) == 1);
    verifica("erro: deveria retornar -1 ao adicionar no buffer cheio", adiciona(40, buffer) == -1);
    liberar(buffer);
    return 0;
}

static char *teste_leitura() {                  // adiciona elementos ao buffer e teste se eles são lidos corretamente e se é indicado quando o buffer está vazio
    Buffer *buffer = criar(3);
    adiciona(1, buffer);
    adiciona(2, buffer);

    verifica("erro: deveria retornar 1", leitura(buffer) == 1);
    verifica("erro: deveria retornar 2", leitura(buffer) == 2);
    verifica("erro: deveria retornar -1 ao ler do buffer vazio", leitura(buffer) == -1);  // Buffer vazio
    liberar(buffer);
    return 0;
}

// Função para executar todos os testes
static char *executa_testes(void) {
    executa_teste(teste_criacao);
    executa_teste(teste_adiciona);
    executa_teste(teste_leitura);
    return 0;
}

int main() {
    char *resultado = executa_testes();
    if (resultado != 0) {
        printf("%s\n", resultado);
    } else {
        printf("TODOS OS TESTES PASSARAM\n");
    }
    printf("Testes executados: %d\n", testes_executados);
    return resultado != 0;
}
