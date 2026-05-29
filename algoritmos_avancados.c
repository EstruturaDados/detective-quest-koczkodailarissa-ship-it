#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ============================================================================
//         DESAFIO DETECTIVE QUEST – ÁRVORES E TABELA HASH
// ============================================================================
//
// OBJETIVOS:
// - Nível Novato: Árvore binária para mapa da mansão.
// - Nível Aventureiro: Árvore de busca para pistas.
// - Nível Mestre: Tabela hash para relacionar pistas e suspeitos.
// - Modularização total em funções.
// - Uso de struct, árvore binária, BST, hash e boas práticas.
//
// ============================================================================

// --- Estruturas ---
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

typedef struct Pista {
    char texto[50];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

typedef struct NodoHash {
    char pista[50];
    char suspeito[50];
    struct NodoHash* prox;
} NodoHash;

// --- Constantes ---
#define TAM_HASH 10

// --- Variáveis globais ---
Pista* raizPistas = NULL;
NodoHash* tabelaHash[TAM_HASH];

// --- Protótipos ---
Sala* criarSala(const char* nome);
void explorarSalas(Sala* atual);

Pista* inserirPista(Pista* raiz, const char* texto);
void listarPistas(Pista* raiz);

int funcaoHash(const char* chave);
void inicializarHash();
void inserirHash(const char* pista, const char* suspeito);
void listarAssociacoes();
void suspeitoMaisCitado();

// --- Função Principal ---
int main() {
    inicializarHash();

    // 🌱 Nível Novato: Mapa da Mansão
    Sala* hall = criarSala("Hall de Entrada");
    hall->esquerda = criarSala("Biblioteca");
    hall->direita = criarSala("Cozinha");
    hall->esquerda->esquerda = criarSala("Sótão");
    hall->esquerda->direita = criarSala("Sala Secreta");
    hall->direita->esquerda = criarSala("Despensa");
    hall->direita->direita = criarSala("Jardim");

    printf("Explorando a mansão...\n");
    explorarSalas(hall);

    // 🔍 Nível Aventureiro: Pistas
    raizPistas = inserirPista(raizPistas, "Chave dourada");
    raizPistas = inserirPista(raizPistas, "Pegada misteriosa");
    raizPistas = inserirPista(raizPistas, "Carta rasgada");

    printf("\nPistas coletadas (em ordem):\n");
    listarPistas(raizPistas);

    // 🧠 Nível Mestre: Hash de Suspeitos
    inserirHash("Chave dourada", "Sr. Black");
    inserirHash("Pegada misteriosa", "Sra. White");
    inserirHash("Carta rasgada", "Srta. Green");
    inserirHash("Pegada misteriosa", "Sr. Black");

    printf("\nAssociações pista → suspeito:\n");
    listarAssociacoes();

    printf("\nSuspeito mais citado:\n");
    suspeitoMaisCitado();

    return 0;
}

// --- Implementações ---
// Salas
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

void explorarSalas(Sala* atual) {
    char escolha;
    while (atual != NULL) {
        printf("Você está na %s. (e=esquerda, d=direita, s=sair): ", atual->nome);
        scanf(" %c", &escolha);
        if (escolha == 'e') atual = atual->esquerda;
        else if (escolha == 'd') atual = atual->direita;
        else if (escolha == 's') break;
        else printf("Opção inválida!\n");
    }
}

// Pistas (BST)
Pista* inserirPista(Pista* raiz, const char* texto) {
    if (raiz == NULL) {
        Pista* nova = (Pista*) malloc(sizeof(Pista));
        strcpy(nova->texto, texto);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(texto, raiz->texto) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else
        raiz->direita = inserirPista(raiz->direita, texto);
    return raiz;
}

void listarPistas(Pista* raiz) {
    if (raiz != NULL) {
        listarPistas(raiz->esquerda);
        printf("- %s\n", raiz->texto);
        listarPistas(raiz->direita);
    }
}

// Hash
int funcaoHash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) soma += chave[i];
    return soma % TAM_HASH;
}

void inicializarHash() {
    for (int i = 0; i < TAM_HASH; i++) tabelaHash[i] = NULL;
}

void inserirHash(const char* pista, const char* suspeito) {
    int idx = funcaoHash(pista);
    NodoHash* novo = (NodoHash*) malloc(sizeof(NodoHash));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[idx];
    tabelaHash[idx] = novo;
}

void listarAssociacoes() {
    for (int i = 0; i < TAM_HASH; i++) {
        NodoHash* atual = tabelaHash[i];
        while (atual != NULL) {
            printf("Pista: %s → Suspeito: %s\n", atual->pista, atual->suspeito);
            atual = atual->prox;
        }
    }
}

void suspeitoMaisCitado() {
    char suspeitos[20][50];
    int contadores[20] = {0};
    int qtd = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        NodoHash* atual = tabelaHash[i];
        while (atual != NULL) {
            int encontrado = -1;
            for (int j = 0; j < qtd; j++) {
                if (strcmp(suspeitos[j], atual->suspeito) == 0) {
                    encontrado = j;
                    break;
                }
            }
            if (encontrado == -1) {
                strcpy(suspeitos[qtd], atual->suspeito);
                contadores[qtd] = 1;
                qtd++;
            } else {
                contadores[encontrado]++;
            }
            atual = atual->prox;
        }
    }

    int max = 0, idx = -1;
    for (int i = 0; i < qtd; i++) {
        if (contadores[i] > max) {
            max = contadores[i];
            idx = i;
        }
    }

    if (idx != -1) {
        printf("Suspeito mais citado: %s (%d vezes)\n", suspeitos[idx], max);
    }
}
