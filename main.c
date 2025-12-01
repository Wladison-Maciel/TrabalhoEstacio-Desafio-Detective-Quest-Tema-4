#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------
// Struct que representa cada sala da mansão
// ---------------------------------------------------------
typedef struct Sala {
    char nome[40];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// ---------------------------------------------------------
// Função: criarSala
// Cria dinamicamente uma sala, atribui o nome e inicializa
// os ponteiros filhos com NULL.
// ---------------------------------------------------------
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}

// ---------------------------------------------------------
// Função: explorarSalas
// Permite ao jogador navegar pela mansão escolhendo entre
// esquerda (e), direita (d) ou sair (s).
// A exploração continua até chegar a um nó-folha.
// ---------------------------------------------------------
void explorarSalas(Sala* atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se for sala sem caminhos, encerra a exploração
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Essa sala não possui mais caminhos. Exploração encerrada!\n");
            return;
        }

        printf("Escolha um caminho:\n");
        printf("  (e) Ir para a esquerda\n");
        printf("  (d) Ir para a direita\n");
        printf("  (s) Sair da exploração\n");
        printf("-> ");
        scanf(" %c", &opcao);

        if (opcao == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Não há caminho à esquerda!\n");
            }
        }
        else if (opcao == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Não há caminho à direita!\n");
            }
        }
        else if (opcao == 's') {
            printf("Exploração encerrada pelo jogador.\n");
            return;
        }
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// ---------------------------------------------------------
// Função main
// Monta a árvore da mansão e inicia a exploração.
// ---------------------------------------------------------
int main() {
    // Criação manual das salas (estrutura fixa)
    Sala* hall = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");

    // Montagem da árvore (mapa da mansão)
    //               Hall
    //              /    \
    //      Sala Estar   Cozinha
    //        /              \
    //  Biblioteca          Jardim

    hall->esquerda = salaEstar;
    hall->direita  = cozinha;

    salaEstar->esquerda = biblioteca;
    cozinha->direita = jardim;

    // Início da exploração
    printf("=== Detective Quest — Exploracao da Mansao ===\n");
    explorarSalas(hall);

    // Liberação da memória
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);

    return 0;
}
