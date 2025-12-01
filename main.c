#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// ---------------------------------------------------------
// Structs
// ---------------------------------------------------------
//

// Struct Sala → árvore binária com nome e pista
typedef struct Sala {
    char nome[40];
    char pista[100];     // pista opcional da sala
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Nó da BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

//
// ---------------------------------------------------------
// Função: criarSala
// Cria dinamicamente uma sala, definindo nome e pista opcional
// ---------------------------------------------------------
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);

    if (pista != NULL)
        strcpy(nova->pista, pista);
    else
        strcpy(nova->pista, "");

    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

//
// ---------------------------------------------------------
// Função: criarNoPista
// Cria dinamicamente um nó da BST com uma pista
// ---------------------------------------------------------
PistaNode* criarNoPista(const char* pista) {
    PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
    if (!novo) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

//
// ---------------------------------------------------------
// Função: inserirPista
// Insere uma nova pista na BST de forma ordenada
// ---------------------------------------------------------
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarNoPista(pista);
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

//
// ---------------------------------------------------------
// Função: exibirPistas (in-order)
// Exibe as pistas em ordem alfabética
// ---------------------------------------------------------
void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

//
// ---------------------------------------------------------
// Função: explorarSalasComPistas
// Permite ao jogador navegar pela mansão e coleta pistas
// ---------------------------------------------------------
void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se houver pista na sala, coleta
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("Nenhuma pista aqui.\n");
        }

        printf("\nEscolha seu próximo caminho:\n");
        printf("  (e) Ir para a esquerda\n");
        printf("  (d) Ir para a direita\n");
        printf("  (s) Sair da exploração\n");
        printf("-> ");
        scanf(" %c", &opcao);

        if (opcao == 'e') {
            atual = atual->esquerda;
        }
        else if (opcao == 'd') {
            atual = atual->direita;
        }
        else if (opcao == 's') {
            printf("\nSaindo da exploração...\n");
            return;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}

//
// ---------------------------------------------------------
// Função: liberarArvoreSalas
// Libera memória da árvore de salas
// ---------------------------------------------------------
void liberarArvoreSalas(Sala* raiz) {
    if (raiz == NULL) return;

    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

//
// ---------------------------------------------------------
// Função: liberarArvorePistas
// Libera memória da BST de pistas
// ---------------------------------------------------------
void liberarArvorePistas(PistaNode* raiz) {
    if (raiz == NULL) return;

    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

//
// ---------------------------------------------------------
// main()
// Monta a mansão, inicia exploração e exibe pistas coletadas
// ---------------------------------------------------------
int main() {

    // Mapa da mansão — criado manualmente
    Sala* hall = criarSala("Hall de Entrada", "Pegadas suspeitas na porta");
    Sala* salaEstar = criarSala("Sala de Estar", "");
    Sala* cozinha = criarSala("Cozinha", "Faca com marca de sangue");
    Sala* biblioteca = criarSala("Biblioteca", "Livro arrancado da estante");
    Sala* jardim = criarSala("Jardim", "Barulho estranho entre as árvores");

    // Montagem da árvore
    hall->esquerda = salaEstar;
    hall->direita  = cozinha;
    salaEstar->esquerda = biblioteca;
    cozinha->direita = jardim;

    // Árvore de pistas coletadas (BST)
    PistaNode* arvorePistas = NULL;

    printf("=== DETECTIVE QUEST — SISTEMA DE COLETA DE PISTAS ===\n");
    explorarSalasComPistas(hall, &arvorePistas);

    printf("\n=== PISTAS COLETADAS ===\n");
    if (arvorePistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    // Libera memória
    liberarArvoreSalas(hall);
    liberarArvorePistas(arvorePistas);

    return 0;
}
