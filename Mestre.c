#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* =====================================================
   CONSTANTES DO SISTEMA
===================================================== */

#define FILA_MAX 5        // Quantidade fixa de peças futuras
#define PILHA_MAX 3       // Capacidade máxima da pilha reserva
#define HIST_MAX 10       // Histórico para desfazer jogadas

/* =====================================================
   STRUCT QUE REPRESENTA UMA PEÇA DO JOGO
===================================================== */
typedef struct {
    int id;        // Identificador único da peça
    char tipo;     // Tipo da peça (I, O, T, L, S, Z, J)
} Peca;

/* =====================================================
   STRUCT DE ESTADO (USADA PARA DESFAZER)
   Guarda um snapshot COMPLETO do sistema
===================================================== */
typedef struct {
    Peca fila[FILA_MAX];
    int inicio, fim, qtdFila;

    Peca pilha[PILHA_MAX];
    int topo;
} Estado;

/* =====================================================
   VARIÁVEIS GLOBAIS
===================================================== */

/* ---- FILA CIRCULAR ---- */
Peca fila[FILA_MAX];
int inicio = 0;
int fim = 0;
int qtdFila = 0;

/* ---- PILHA ---- */
Peca pilha[PILHA_MAX];
int topo = -1;

/* ---- HISTÓRICO PARA DESFAZER ---- */
Estado historico[HIST_MAX];
int topoHist = -1;

/* ---- CONTROLE ---- */
int proximoId = 1;
char tipos[] = {'I','O','T','L','S','Z','J'};

/* =====================================================
   GERA UMA PEÇA AUTOMATICAMENTE
===================================================== */
Peca gerarPeca() {
    Peca p;
    p.id = proximoId++;
    p.tipo = tipos[rand() % 7];
    return p;
}

/* =====================================================
   SALVA O ESTADO ATUAL (UNDO)
===================================================== */
void salvarEstado() {
    if (topoHist == HIST_MAX - 1) return;

    Estado e;

    e.inicio = inicio;
    e.fim = fim;
    e.qtdFila = qtdFila;
    e.topo = topo;

    for (int i = 0; i < FILA_MAX; i++)
        e.fila[i] = fila[i];

    for (int i = 0; i < PILHA_MAX; i++)
        e.pilha[i] = pilha[i];

    historico[++topoHist] = e;
}

/* =====================================================
   DESFAZER ÚLTIMA AÇÃO
===================================================== */
void desfazer() {
    if (topoHist < 0) {
        printf("\nNada para desfazer!\n");
        return;
    }

    Estado e = historico[topoHist--];

    inicio = e.inicio;
    fim = e.fim;
    qtdFila = e.qtdFila;
    topo = e.topo;

    for (int i = 0; i < FILA_MAX; i++)
        fila[i] = e.fila[i];

    for (int i = 0; i < PILHA_MAX; i++)
        pilha[i] = e.pilha[i];

    printf("\nÚltima jogada desfeita.\n");
}

/* =====================================================
   OPERAÇÕES DA FILA
===================================================== */
void enqueue() {
    fila[fim] = gerarPeca();
    fim = (fim + 1) % FILA_MAX;
    qtdFila++;
}

Peca dequeue() {
    Peca p = fila[inicio];
    inicio = (inicio + 1) % FILA_MAX;
    qtdFila--;
    return p;
}

/* =====================================================
   OPERAÇÕES DA PILHA
===================================================== */
void push(Peca p) {
    if (topo < PILHA_MAX - 1)
        pilha[++topo] = p;
}

Peca pop() {
    Peca vazio = {-1,'-'};
    if (topo >= 0)
        return pilha[topo--];
    return vazio;
}

/* =====================================================
   TROCAR TOPO DA PILHA COM FRENTE DA FILA
===================================================== */
void trocarFilaPilha() {
    if (topo < 0 || qtdFila == 0) {
        printf("\nTroca impossível!\n");
        return;
    }

    salvarEstado();

    Peca temp = pilha[topo];
    pilha[topo] = fila[inicio];
    fila[inicio] = temp;

    printf("\nTroca realizada.\n");
}

/* =====================================================
   INVERTER FILA COM PILHA (PARCIAL)
===================================================== */
void inverterFilaPilha() {
    if (topo < 0) {
        printf("\nPilha vazia! Inversão impossível.\n");
        return;
    }

    salvarEstado();

    for (int i = 0; i <= topo && i < qtdFila; i++) {
        int idx = (inicio + i) % FILA_MAX;
        Peca temp = fila[idx];
        fila[idx] = pilha[i];
        pilha[i] = temp;
    }

    printf("\nFila e pilha invertidas.\n");
}

/* =====================================================
   EXIBIÇÃO DO ESTADO DO JOGO
===================================================== */
void mostrarEstado() {
    printf("\n--- FILA DE PEÇAS ---\n");
    int idx = inicio;
    for (int i = 0; i < qtdFila; i++) {
        printf("ID:%d Tipo:%c\n", fila[idx].id, fila[idx].tipo);
        idx = (idx + 1) % FILA_MAX;
    }

    printf("\n--- PILHA DE RESERVA ---\n");
    if (topo < 0)
        printf("Vazia\n");
    else
        for (int i = topo; i >= 0; i--)
            printf("ID:%d Tipo:%c\n", pilha[i].id, pilha[i].tipo);
}

/* =====================================================
   MENU
===================================================== */
void menu() {
    printf("\n=== TETRIS STACK - NIVEL MESTRE ===\n");
    printf("1 - Jogar peça\n");
    printf("2 - Reservar peça\n");
    printf("3 - Usar peça reservada\n");
    printf("4 - Trocar pilha com fila\n");
    printf("5 - Desfazer última jogada\n");
    printf("6 - Inverter fila com pilha\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
}

/* =====================================================
   FUNÇÃO PRINCIPAL
===================================================== */
int main() {
    srand(time(NULL));

    // Inicializa a fila com 5 peças
    for (int i = 0; i < FILA_MAX; i++)
        enqueue();

    int opcao;
    Peca p;

    do {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                salvarEstado();
                p = dequeue();
                printf("\nJogada -> %c\n", p.tipo);
                enqueue();
                break;

            case 2:
                salvarEstado();
                p = dequeue();
                push(p);
                enqueue();
                break;

            case 3:
                salvarEstado();
                p = pop();
                if (p.id != -1)
                    printf("\nUsada reserva -> %c\n", p.tipo);
                break;

            case 4:
                trocarFilaPilha();
                break;

            case 5:
                desfazer();
                break;

            case 6:
                inverterFilaPilha();
                break;
        }

        mostrarEstado();

    } while (opcao != 0);

    printf("\nEncerrando jogo...\n");
    return 0;
}