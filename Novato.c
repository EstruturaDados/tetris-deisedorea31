#include <stdio.h>   // Biblioteca para entrada e saída (printf, scanf)
#include <stdlib.h>  // Biblioteca para funções utilitárias (rand, srand)
#include <time.h>    // Biblioteca para usar a hora do sistema (time)

// Tamanho fixo da fila
#define MAX 5

/* -----------------------------------
   Estrutura que representa uma peça
----------------------------------- */
typedef struct {
    int id;         // Identificador único da peça
    char nome[2];   // Tipo da peça (I, O, T, L, etc.)
} Peca;

/* -----------------------------------
   Variáveis globais da fila circular
----------------------------------- */
Peca fila[MAX];     // Array que representa a fila
int inicio = 0;     // Índice do início da fila
int fim = 0;        // Índice do final da fila
int quantidade = 0;// Quantidade atual de elementos na fila
int proximoId = 1;  // Controla o ID único das peças

// Tipos possíveis de peças do Tetris
char tiposPeca[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};

/* -----------------------------------
   Função que gera automaticamente
   uma nova peça
----------------------------------- */
Peca gerarPeca() {
    Peca p;

    p.id = proximoId++;                     // Define um ID único
    p.nome[0] = tiposPeca[rand() % 7];      // Sorteia um tipo de peça
    p.nome[1] = '\0';                       // Final da string

    return p;                               // Retorna a peça criada
}

/* -----------------------------------
   Enfileirar peça (enqueue)
   Insere no final da fila
----------------------------------- */
void enqueue() {

    // Verifica se a fila está cheia
    if (quantidade == MAX) {
        printf("\nFila cheia! Não é possível inserir nova peça.\n");
        return;
    }

    // Insere a peça na posição 'fim'
    fila[fim] = gerarPeca();

    // Avança o índice do fim de forma circular
    fim = (fim + 1) % MAX;

    // Incrementa a quantidade de elementos
    quantidade++;
}

/* -----------------------------------
   Desenfileirar peça (dequeue)
   Remove do início da fila
----------------------------------- */
void dequeue() {

    // Verifica se a fila está vazia
    if (quantidade == 0) {
        printf("\nFila vazia! Nenhuma peça para jogar.\n");
        return;
    }

    // Exibe a peça que está sendo jogada
    printf("\nPeça jogada: ID %d | Tipo %s\n",
           fila[inicio].id, fila[inicio].nome);

    // Avança o índice do início de forma circular
    inicio = (inicio + 1) % MAX;

    // Decrementa a quantidade
    quantidade--;

    // Insere automaticamente uma nova peça no final
    enqueue();
}

/* -----------------------------------
   Exibe o estado atual da fila
----------------------------------- */
void exibirFila() {

    if (quantidade == 0) {
        printf("\nFila vazia.\n");
        return;
    }

    printf("\nFila de peças futuras:\n");

    int idx = inicio;

    // Percorre a fila respeitando a circularidade
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] ID:%d Tipo:%s\n",
               i + 1, fila[idx].id, fila[idx].nome);

        idx = (idx + 1) % MAX;
    }
}

/* -----------------------------------
   Exibe o menu do sistema
----------------------------------- */
void menu() {
    printf("\n==== TETRIS STACK - FILA DE PEÇAS ====\n");
    printf("1 - Visualizar fila\n");
    printf("2 - Jogar peça\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

/* -----------------------------------
   Função principal
----------------------------------- */
int main() {

    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    // Inicializa a fila com 5 peças automáticas
    for (int i = 0; i < MAX; i++) {
        enqueue();
    }

    int opcao;

    // Loop principal do programa
    do {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibirFila();
                break;

            case 2:
                dequeue();
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
