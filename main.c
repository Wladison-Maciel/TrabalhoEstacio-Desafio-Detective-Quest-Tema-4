#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 64
#define MAX_PISTA 128
#define HASH_SIZE 31   // tamanho simples para a tabela hash (primo pequeno)

// ---------------------------
// Structs
// ---------------------------

// Nó da árvore binária (salas)
typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA]; // pista opcional (string vazia = sem pista)
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

// Nó da BST de pistas (armazenadas em ordem alfabética)
// Cada nó armazena a pista e uma contagem de quantas vezes foi coletada.
typedef struct PistaNode {
    char pista[MAX_PISTA];
    int contagem;
    struct PistaNode* esquerda;
    struct PistaNode* direita;
} PistaNode;

// Entrada da tabela hash (lista encadeada para colisões)
// chave = pista, valor = nome do suspeito
typedef struct HashEntry {
    char pista[MAX_PISTA];
    char suspeito[MAX_NOME];
    struct HashEntry* proximo;
} HashEntry;

// Tabela hash é um array de ponteiros para HashEntry
typedef HashEntry* HashTable[HASH_SIZE];

// ---------------------------
// Funções para Salas (Árvore binária de cômodos)
// ---------------------------

/*
 * criarSala()
 * Cria dinamicamente um cômodo (Sala), atribui nome e pista (pista pode ser NULL ou "")
 * Retorna ponteiro para a sala criada (alocado com malloc).
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        fprintf(stderr, "Erro: falha ao alocar memória para Sala.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(nova->nome, nome, MAX_NOME-1); nova->nome[MAX_NOME-1] = '\0';
    if (pista && pista[0] != '\0') {
        strncpy(nova->pista, pista, MAX_PISTA-1); nova->pista[MAX_PISTA-1] = '\0';
    } else {
        nova->pista[0] = '\0';
    }
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ---------------------------
// Funções para BST de pistas
// ---------------------------

PistaNode* criarNoPista(const char* pista) {
    PistaNode* no = (PistaNode*) malloc(sizeof(PistaNode));
    if (!no) {
        fprintf(stderr, "Erro: falha ao alocar memória para PistaNode.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(no->pista, pista, MAX_PISTA-1); no->pista[MAX_PISTA-1] = '\0';
    no->contagem = 1;
    no->esquerda = no->direita = NULL;
    return no;
}

/*
 * inserirPista()
 * Insere (ou incrementa) a pista na BST de modo ordenado.
 * Se a pista já existe, incrementa sua contagem; caso contrário, insere novo nó.
 * Usa recursividade; retorna a raiz atualizada.
 */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (!pista || pista[0] == '\0') return raiz; // nada a inserir

    if (raiz == NULL) {
        return criarNoPista(pista);
    }

    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) {
        raiz->contagem += 1;
    } else if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}

/*
 * exibirPistasInOrder()
 * Percorre a BST em ordem e exibe cada pista junto com sua contagem.
 */
void exibirPistasInOrder(PistaNode* raiz) {
    if (!raiz) return;
    exibirPistasInOrder(raiz->esquerda);
    printf("  \"%s\" (coletada %d vez(es))\n", raiz->pista, raiz->contagem);
    exibirPistasInOrder(raiz->direita);
}

/*
 * liberarArvorePistas()
 * Libera a memória da BST de pistas (pós-ordem).
 */
void liberarArvorePistas(PistaNode* raiz) {
    if (!raiz) return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

// ---------------------------
// Funções para Tabela Hash (associação pista -> suspeito)
// ---------------------------

/*
 * hashFunc()
 * Função de dispersão simples: soma dos caracteres modulo HASH_SIZE.
 */
unsigned int hashFunc(const char* chave) {
    unsigned int h = 0;
    for (const char* p = chave; *p; ++p) {
        h = (h * 31u) + (unsigned char)(*p);
    }
    return h % HASH_SIZE;
}

/*
 * inserirNaHash()
 * Insere uma associação pista -> suspeito na tabela hash.
 * Se já existir uma entrada com a mesma pista, substitui o suspeito.
 */
void inserirNaHash(HashTable tabela, const char* pista, const char* suspeito) {
    if (!pista || pista[0] == '\0' || !suspeito) return;
    unsigned int idx = hashFunc(pista);
    HashEntry* atual = tabela[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) { // já existe -> atualiza suspeito
            strncpy(atual->suspeito, suspeito, MAX_NOME-1);
            atual->suspeito[MAX_NOME-1] = '\0';
            return;
        }
        atual = atual->proximo;
    }
    // nova entrada
    HashEntry* novo = (HashEntry*) malloc(sizeof(HashEntry));
    if (!novo) {
        fprintf(stderr, "Erro: falha ao alocar memória para HashEntry.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(novo->pista, pista, MAX_PISTA-1); novo->pista[MAX_PISTA-1] = '\0';
    strncpy(novo->suspeito, suspeito, MAX_NOME-1); novo->suspeito[MAX_NOME-1] = '\0';
    novo->proximo = tabela[idx];
    tabela[idx] = novo;
}

/*
 * encontrarSuspeito()
 * Consulta a tabela hash e retorna o nome do suspeito associado à pista.
 * Retorna NULL se a pista não estiver mapeada.
 */
const char* encontrarSuspeito(HashTable tabela, const char* pista) {
    if (!pista) return NULL;
    unsigned int idx = hashFunc(pista);
    HashEntry* atual = tabela[idx];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

/*
 * liberarHash()
 * Libera todas as entradas da tabela hash.
 */
void liberarHash(HashTable tabela) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashEntry* atual = tabela[i];
        while (atual) {
            HashEntry* prox = atual->proximo;
            free(atual);
            atual = prox;
        }
        tabela[i] = NULL;
    }
}

// ---------------------------
// Função de Exploração (interatividade)
// ---------------------------

/*
 * explorarSalas()
 * Navega pela árvore de cômodos (a partir de 'atual'), identifica pistas em cada sala,
 * exibe a pista e adiciona a pista à BST de pistas coletadas.
 *
 * Parâmetros:
 *   atual       - ponteiro para a sala atual (Sala*)
 *   arvorePistas - ponteiro para a raiz da BST de pistas (PistaNode**)
 *   tabela      - tabela hash para consulta/registro (HashTable)
 *
 * A função permite as opções: e (esquerda), d (direita), s (sair).
 */
void explorarSalas(Sala* atual, PistaNode** arvorePistas, HashTable tabela) {
    char opcao;
    if (!atual) {
        printf("Mapa vazio.\n");
        return;
    }

    while (atual) {
        printf("\nVocê entrou em: %s\n", atual->nome);

        // Se houver pista nesta sala, mostra e adiciona
        if (atual->pista[0] != '\0') {
            printf("  -> Pista encontrada: \"%s\"\n", atual->pista);
            // adiciona à BST (se já existir, incrementa contagem)
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        } else {
            printf("  -> Nenhuma pista nesta sala.\n");
        }

        // opções ao jogador
        printf("\nEscolha:\n  (e) esquerda\n  (d) direita\n  (s) sair da exploração\n-> ");
        scanf(" %c", &opcao);

        if (opcao == 'e') {
            if (atual->esquerda) atual = atual->esquerda;
            else {
                printf("Não há caminho à esquerda.\n");
                // permanece na sala atual
            }
        } else if (opcao == 'd') {
            if (atual->direita) atual = atual->direita;
            else {
                printf("Não há caminho à direita.\n");
            }
        } else if (opcao == 's') {
            printf("Exploração encerrada pelo jogador.\n");
            return;
        } else {
            printf("Opção inválida. Use 'e', 'd' ou 's'.\n");
        }
    }
}

// ---------------------------
// Verificação final do suspeito
// ---------------------------

/*
 * contarPistasPorSuspeito()
 * Percorre a BST de pistas (in-order) e conta quantas pistas apontam para 'suspeitoAlvo'
 * usando a tabela hash (encontrarSuspeito). Retorna o total encontrado.
 *
 * Usamos recursão para percorrer a árvore.
 */
int contarPistasPorSuspeitoRec(PistaNode* raiz, HashTable tabela, const char* suspeitoAlvo) {
    if (!raiz) return 0;
    int cont = 0;
    cont += contarPistasPorSuspeitoRec(raiz->esquerda, tabela, suspeitoAlvo);

    const char* suspeito = encontrarSuspeito(tabela, raiz->pista);
    if (suspeito && strcmp(suspeito, suspeitoAlvo) == 0) {
        cont += raiz->contagem; // somar quantas vezes a pista foi coletada
    }

    cont += contarPistasPorSuspeitoRec(raiz->direita, tabela, suspeitoAlvo);
    return cont;
}

/*
 * verificarSuspeitoFinal()
 * Recebe a BST de pistas coletadas, a tabela hash e o nome do suspeito acusado.
 * Retorna 1 se o número de pistas apontando para o suspeito for >= 2, caso contrário 0.
 */
int verificarSuspeitoFinal(PistaNode* arvorePistas, HashTable tabela, const char* acusado) {
    if (!arvorePistas) return 0;
    int total = contarPistasPorSuspeitoRec(arvorePistas, tabela, acusado);
    return (total >= 2) ? 1 : 0;
}

// ---------------------------
// Funções utilitárias: liberar árvore de salas
// ---------------------------

void liberarArvoreSalas(Sala* raiz) {
    if (!raiz) return;
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

// ---------------------------
// main(): monta mapa, popula hash e conduz jogo
// ---------------------------
int main() {
    // --- Montagem do mapa (fixo) ---
    // Exemplo simples com 7 salas:
    //
    //               Hall
    //              /    \
    //         SalaEstar  Cozinha
    //         /    \       \
    //   Biblioteca  Corredor  Jardim
    //      /
    //  Porão
    //
    Sala* hall = criarSala("Hall de Entrada", "Pegadas molhadas próximo à entrada");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo quebrado com marcas");
    Sala* cozinha = criarSala("Cozinha", "Faca com marca de sangue");
    Sala* biblioteca = criarSala("Biblioteca", "Página arrancada de um romance");
    Sala* corredor = criarSala("Corredor", "");
    Sala* jardim = criarSala("Jardim", "Pegadas indo para o portão");
    Sala* porao = criarSala("Porão", "Luz de lanterna e marcas de lama");

    // conectar
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = corredor;

    biblioteca->esquerda = porao;
    cozinha->direita = jardim;

    // --- Preparar a tabela hash (mapa pista -> suspeito) ---
    // Inicializa com NULLs
    HashTable tabela;
    for (int i = 0; i < HASH_SIZE; i++) tabela[i] = NULL;

    // Preenchimento manual das associações (exemplo)
    inserirNaHash(tabela, "Pegadas molhadas próximo à entrada", "Sr. Branco");
    inserirNaHash(tabela, "Copo quebrado com marcas", "Sra. Verde");
    inserirNaHash(tabela, "Faca com marca de sangue", "Sr. Preto");
    inserirNaHash(tabela, "Página arrancada de um romance", "Sra. Rosa");
    inserirNaHash(tabela, "Pegadas indo para o portão", "Sr. Branco");
    inserirNaHash(tabela, "Luz de lanterna e marcas de lama", "Sr. Preto");
    // (podemos ter várias pistas apontando para o mesmo suspeito)

    // --- BST vazia de pistas coletadas ---
    PistaNode* arvorePistas = NULL;

    // Mensagem inicial
    printf("=== DETECTIVE QUEST: Acuse o culpado ===\n");
    printf("Explore a mansão a partir do Hall de Entrada.\n");
    printf("Use 'e' para esquerda, 'd' para direita e 's' para sair da exploração.\n");

    // --- Exploração interativa ---
    explorarSalas(hall, &arvorePistas, tabela);

    // --- Exibir pistas coletadas ---
    printf("\n=== Pistas coletadas (em ordem alfabética) ===\n");
    if (!arvorePistas) {
        printf("Nenhuma pista coletada.\n");
    } else {
        exibirPistasInOrder(arvorePistas);
    }

    // --- Acusação final ---
    char acusado[MAX_NOME];
    printf("\nInforme o nome do suspeito que deseja acusar (ex: \"Sr. Branco\"): ");
    // ler com scanf para token sem espaços; se quiser nomes com espaços, usar fgets
    // usaremos fgets para aceitar espaços
    getchar(); // consome '\n' remanescente
    if (!fgets(acusado, MAX_NOME, stdin)) {
        acusado[0] = '\0';
    } else {
        // remove newline final
        acusado[strcspn(acusado, "\n")] = '\0';
    }

    if (acusado[0] == '\0') {
        printf("Nenhum nome fornecido. Encerrando.\n");
    } else {
        int resultado = verificarSuspeitoFinal(arvorePistas, tabela, acusado);
        if (resultado) {
            printf("\nResultado: Há evidências suficientes. %s é considerado culpado!\n", acusado);
        } else {
            printf("\nResultado: Pistas insuficientes contra %s. Acusação rejeitada.\n", acusado);
        }
    }

    // --- Limpeza de memória ---
    liberarArvoreSalas(hall);
    liberarArvorePistas(arvorePistas);
    liberarHash(tabela);

    printf("\nPrograma finalizado. Memória liberada.\n");
    return 0;
}
