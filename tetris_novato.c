#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // Identificador único da peça
} Peca;

// Estrutura que representa a fila circular de peças
typedef struct {
    Peca pecas[TAMANHO_FILA];  // Array de peças
    int frente;                 // Índice da frente da fila
    int tras;                   // Índice do final da fila
    int tamanho;                // Quantidade atual de peças na fila
    int proximoId;              // Próximo ID a ser atribuído
} FilaPecas;

/**
 * Gera uma peça aleatória com um ID único
 * @param id - Identificador único para a peça
 * @return Peca - Nova peça gerada
 */
Peca gerarPeca(int id) {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'};
    
    // Seleciona um tipo aleatório
    novaPeca.nome = tipos[rand() % 4];
    novaPeca.id = id;
    
    return novaPeca;
}

/**
 * Inicializa a fila de peças com elementos iniciais
 * @param fila - Ponteiro para a estrutura da fila
 */
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
    fila->proximoId = 0;
    
    // Preenche a fila com peças iniciais
    for (int i = 0; i < TAMANHO_FILA; i++) {
        fila->pecas[i] = gerarPeca(fila->proximoId);
        fila->proximoId++;
        fila->tamanho++;
        fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    }
}

/**
 * Verifica se a fila está vazia
 * @param fila - Ponteiro para a estrutura da fila
 * @return int - 1 se vazia, 0 caso contrário
 */
int filaVazia(FilaPecas *fila) {
    return fila->tamanho == 0;
}

/**
 * Verifica se a fila está cheia
 * @param fila - Ponteiro para a estrutura da fila
 * @return int - 1 se cheia, 0 caso contrário
 */
int filaCheia(FilaPecas *fila) {
    return fila->tamanho == TAMANHO_FILA;
}

/**
 * Remove uma peça da frente da fila (dequeue)
 * @param fila - Ponteiro para a estrutura da fila
 * @return Peca - Peça removida
 */
Peca jogarPeca(FilaPecas *fila) {
    if (filaVazia(fila)) {
        printf("\n❌ Erro: A fila está vazia!\n");
        Peca pecaVazia = {' ', -1};
        return pecaVazia;
    }
    
    // Remove a peça da frente
    Peca pecaJogada = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    printf("\n✓ Peça [%c %d] jogada com sucesso!\n", pecaJogada.nome, pecaJogada.id);
    return pecaJogada;
}

/**
 * Adiciona uma nova peça ao final da fila (enqueue)
 * @param fila - Ponteiro para a estrutura da fila
 */
void inserirPeca(FilaPecas *fila) {
    if (filaCheia(fila)) {
        printf("\n❌ Erro: A fila está cheia! Jogue uma peça primeiro.\n");
        return;
    }
    
    // Gera e insere nova peça no final
    Peca novaPeca = gerarPeca(fila->proximoId);
    fila->pecas[fila->tras] = novaPeca;
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->tamanho++;
    fila->proximoId++;
    
    printf("\n✓ Nova peça [%c %d] inserida com sucesso!\n", novaPeca.nome, novaPeca.id);
}

/**
 * Exibe o estado atual da fila
 * @param fila - Ponteiro para a estrutura da fila
 */
void exibirFila(FilaPecas *fila) {
    printf("\n========================================\n");
    printf("Fila de peças: ");
    
    if (filaVazia(fila)) {
        printf("[VAZIA]");
    } else {
        int indice = fila->frente;
        for (int i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
            indice = (indice + 1) % TAMANHO_FILA;
        }
    }
    
    printf("\n========================================\n");
}

/**
 * Exibe o menu de opções
 */
void exibirMenu() {
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│       TETRIS STACK - MENU           │\n");
    printf("├─────────────────────────────────────┤\n");
    printf("│ 1 - Jogar peça (dequeue)            │\n");
    printf("│ 2 - Inserir nova peça (enqueue)     │\n");
    printf("│ 0 - Sair                            │\n");
    printf("└─────────────────────────────────────┘\n");
    printf("Escolha uma opção: ");
}

/**
 * Função principal do programa
 */
int main() {
    FilaPecas fila;
    int opcao;
    
    // Inicializa gerador de números aleatórios
    srand(time(NULL));
    
    // Inicializa a fila com peças
    inicializarFila(&fila);
    
    printf("╔════════════════════════════════════════╗\n");
    printf("║   BEM-VINDO AO TETRIS STACK!          ║\n");
    printf("║   Sistema de Gerenciamento de Peças   ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    // Exibe estado inicial da fila
    exibirFila(&fila);
    
    // Loop principal do programa
    do {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                // Jogar peça (remover da frente)
                jogarPeca(&fila);
                exibirFila(&fila);
                break;
                
            case 2:
                // Inserir nova peça (adicionar no final)
                inserirPeca(&fila);
                exibirFila(&fila);
                break;
                
            case 0:
                // Sair do programa
                printf("\n👋 Obrigado por jogar Tetris Stack!\n");
                printf("Desenvolvido pela ByteBros.\n\n");
                break;
                
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }
        
    } while(opcao != 0);
    
    return 0;
}