#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3

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

// Estrutura que representa a pilha de peças reservadas
typedef struct {
    Peca pecas[TAMANHO_PILHA];  // Array de peças reservadas
    int topo;                    // Índice do topo da pilha (-1 = vazia)
} PilhaReserva;

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
 * Inicializa a pilha de reserva vazia
 * @param pilha - Ponteiro para a estrutura da pilha
 */
void inicializarPilha(PilhaReserva *pilha) {
    pilha->topo = -1;  // Pilha começa vazia
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
 * Verifica se a pilha está vazia
 * @param pilha - Ponteiro para a estrutura da pilha
 * @return int - 1 se vazia, 0 caso contrário
 */
int pilhaVazia(PilhaReserva *pilha) {
    return pilha->topo == -1;
}

/**
 * Verifica se a pilha está cheia
 * @param pilha - Ponteiro para a estrutura da pilha
 * @return int - 1 se cheia, 0 caso contrário
 */
int pilhaCheia(PilhaReserva *pilha) {
    return pilha->topo == TAMANHO_PILHA - 1;
}

/**
 * Adiciona uma nova peça gerada ao final da fila
 * @param fila - Ponteiro para a estrutura da fila
 */
void adicionarPecaNaFila(FilaPecas *fila) {
    if (filaCheia(fila)) {
        return;  // Não adiciona se já está cheia
    }
    
    // Gera e insere nova peça no final
    Peca novaPeca = gerarPeca(fila->proximoId);
    fila->pecas[fila->tras] = novaPeca;
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->tamanho++;
    fila->proximoId++;
}

/**
 * Remove e retorna a peça da frente da fila
 * @param fila - Ponteiro para a estrutura da fila
 * @return Peca - Peça removida
 */
Peca removerDaFila(FilaPecas *fila) {
    Peca pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    return pecaRemovida;
}

/**
 * Joga uma peça (remove da fila e adiciona nova)
 * @param fila - Ponteiro para a estrutura da fila
 */
void jogarPeca(FilaPecas *fila) {
    if (filaVazia(fila)) {
        printf("\n❌ Erro: A fila está vazia!\n");
        return;
    }
    
    // Remove a peça da frente
    Peca pecaJogada = removerDaFila(fila);
    printf("\n✓ Peça [%c %d] jogada com sucesso!\n", pecaJogada.nome, pecaJogada.id);
    
    // Adiciona uma nova peça no final da fila
    adicionarPecaNaFila(fila);
    printf("✓ Nova peça [%c %d] adicionada à fila.\n", 
           fila->pecas[(fila->tras - 1 + TAMANHO_FILA) % TAMANHO_FILA].nome,
           fila->pecas[(fila->tras - 1 + TAMANHO_FILA) % TAMANHO_FILA].id);
}

/**
 * Reserva uma peça (move da fila para a pilha)
 * @param fila - Ponteiro para a estrutura da fila
 * @param pilha - Ponteiro para a estrutura da pilha
 */
void reservarPeca(FilaPecas *fila, PilhaReserva *pilha) {
    if (filaVazia(fila)) {
        printf("\n❌ Erro: A fila está vazia!\n");
        return;
    }
    
    if (pilhaCheia(pilha)) {
        printf("\n❌ Erro: A pilha de reserva está cheia!\n");
        return;
    }
    
    // Remove da fila e adiciona na pilha
    Peca pecaReservada = removerDaFila(fila);
    pilha->topo++;
    pilha->pecas[pilha->topo] = pecaReservada;
    
    printf("\n✓ Peça [%c %d] movida para a pilha de reserva!\n", 
           pecaReservada.nome, pecaReservada.id);
    
    // Adiciona uma nova peça no final da fila
    adicionarPecaNaFila(fila);
    printf("✓ Nova peça [%c %d] adicionada à fila.\n", 
           fila->pecas[(fila->tras - 1 + TAMANHO_FILA) % TAMANHO_FILA].nome,
           fila->pecas[(fila->tras - 1 + TAMANHO_FILA) % TAMANHO_FILA].id);
}

/**
 * Usa uma peça reservada (remove do topo da pilha)
 * @param pilha - Ponteiro para a estrutura da pilha
 */
void usarPecaReservada(PilhaReserva *pilha) {
    if (pilhaVazia(pilha)) {
        printf("\n❌ Erro: A pilha de reserva está vazia!\n");
        return;
    }
    
    // Remove do topo da pilha
    Peca pecaUsada = pilha->pecas[pilha->topo];
    pilha->topo--;
    
    printf("\n✓ Peça reservada [%c %d] usada com sucesso!\n", 
           pecaUsada.nome, pecaUsada.id);
}

/**
 * Exibe o estado atual da fila e da pilha
 * @param fila - Ponteiro para a estrutura da fila
 * @param pilha - Ponteiro para a estrutura da pilha
 */
void exibirEstado(FilaPecas *fila, PilhaReserva *pilha) {
    printf("\n========================================\n");
    printf("ESTADO ATUAL\n");
    printf("========================================\n");
    
    // Exibe a fila
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
    printf("\n");
    
    // Exibe a pilha (do topo para a base)
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("[VAZIA]");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
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
    printf("│ 1 - Jogar peça                      │\n");
    printf("│ 2 - Reservar peça                   │\n");
    printf("│ 3 - Usar peça reservada             │\n");
    printf("│ 0 - Sair                            │\n");
    printf("└─────────────────────────────────────┘\n");
    printf("Escolha uma opção: ");
}

/**
 * Função principal do programa
 */
int main() {
    FilaPecas fila;
    PilhaReserva pilha;
    int opcao;
    
    // Inicializa gerador de números aleatórios
    srand(time(NULL));
    
    // Inicializa a fila com peças e a pilha vazia
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    printf("╔════════════════════════════════════════╗\n");
    printf("║   BEM-VINDO AO TETRIS STACK!          ║\n");
    printf("║   Sistema de Gerenciamento de Peças   ║\n");
    printf("║   com Fila e Pilha de Reserva         ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    // Exibe estado inicial
    exibirEstado(&fila, &pilha);
    
    // Loop principal do programa
    do {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                // Jogar peça (remover da fila)
                jogarPeca(&fila);
                exibirEstado(&fila, &pilha);
                break;
                
            case 2:
                // Reservar peça (mover da fila para a pilha)
                reservarPeca(&fila, &pilha);
                exibirEstado(&fila, &pilha);
                break;
                
            case 3:
                // Usar peça reservada (remover da pilha)
                usarPecaReservada(&pilha);
                exibirEstado(&fila, &pilha);
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