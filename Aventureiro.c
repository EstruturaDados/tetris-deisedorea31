#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------------- CONSTANTES ---------------- */

#define FILA_MAX 5     // Capacidade fixa da fila
#define PILHA_MAX 3    // Capacidade máxima da pilha

/* ---------------- STRUCT DA PEÇA ---------------- */

typedef struct {
    int id;         // Identificador único
    char tipo;      // Tipo da peça: I, O, T, L...
} Peca;

/* ---------------- FILA CIRCULAR ---------------- */

Peca fila[FILA_MAX];
int inicio = 0;        // Índice do início da fila
int fim = 0;           // Índice do final da fila
int qtdFila = 0;       // Quantidade atual de peças da fila

/* ---------------- PILHA LINEAR ---------------- */

Peca pilha[PILHA_MAX];
int topo = -1;         // Topo da pilha (-1 = vazia)

/* ---------------- CONTROLE ---------------- */

int proximoId = 1;     // Gera IDs únicos
char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};

/* =================================================
   FUNÇÃO: Gera uma peça automaticamente
================================================= */
Peca gerarPeca() {
    Peca p;
    p.id = proximoId++;
    p.tipo = tipos[rand() % 7];
    return p;
}

/* =================================================
   FUNÇÕES DA FILA
================================================= */

// Insere uma peça no final da fila
void enqueue() {
    fila[fim] = gerarPeca();
    fim = (fim + 1) % FILA_MAX;
    qtdFila++;
}

// Remove a peça da frente da fila
Peca dequeue() {
    Peca p = fila[inicio];
    inicio = (inicio + 1) % FILA_MAX;
    qtdFila--;
    return p;
}

/* =================================================
   FUNÇÕES DA PILHA
================================================= */

// Empilha uma peça (push)
void push(Peca p) {
    if (topo == PILHA_MAX - 1) {
        printf("\nPilha cheia! Não é possível reservar.\n");
        return;
    }
    pilha[++topo] = p;
}

// Desempilha uma peça (pop)
Peca pop() {
    if (topo == -1) {
        printf("\nPilha vazia! Nenhuma peça reservada.\n");
        Peca vazia = {-1, '-'};
        return vazia;
    }
    return pilha[topo--];
}

/* =================================================
   EXIBIÇÃO DO ESTADO DO JOGO
================================================= */

void mostrarFila() {
    printf("\nFila (Peças Futuras):\n");
    int i, idx = inicio;

    for (i = 0; i < qtdFila; i++) {
        printf("ID:%d Tipo:%c\n", fila[idx].id, fila[idx].tipo);
        idx = (idx + 1) % FILA_MAX;
    }
}

void mostrarPilha() {
    printf("\nPilha (Reserva):\n");

    if (topo == -1) {
        printf("Vazia\n");
        return;
    }

    for (int i = topo; i >= 0; i--) {
        printf("ID:%d Tipo:%c\n", pilha[i].id, pilha[i].tipo);
    }
}

void mostrarEstado() {
    mostrarFila();
    mostrarPilha();
}

/* =================================================
   MENU
================================================= */

void menu() {
    printf("\n=== TETRIS STACK - NIVEL AVENTUREIRO ===\n");
    printf("1 - Jogar peça\n");
    printf("2 - Reservar peça\n");
    printf("3 - Usar peça reservada\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

/* =================================================
   PROGRAMA PRINCIPAL
================================================= */

int main() {
    srand(time(NULL));

    // Inicializa a fila com 5 peças automáticas
    for (int i = 0; i < FILA_MAX; i++) {
        enqueue();
    }

    int opcao;
    Peca p;

    do {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {

            // Jogar peça (remove da fila e repõe)
            case 1:
                p = dequeue();
                printf("\nPeça jogada -> ID:%d Tipo:%c\n", p.id, p.tipo);
                enqueue();
                mostrarEstado();
                break;

            // Reservar peça (da fila para a pilha)
            case 2:
                if (topo < PILHA_MAX - 1) {
                    p = dequeue();
                    push(p);
                    enqueue();
                    printf("\nPeça reservada.\n");
                }
                mostrarEstado();
                break;

            // Usar peça reservada (pop)
            case 3:
                p = pop();
                if (p.id != -1) {
                    printf("\nPeça usada da reserva -> ID:%d Tipo:%c\n", 
                           p.id, p.tipo);
                }
                mostrarEstado();
                break;

            case 0:
                printf("\nEncerrando jogo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
``