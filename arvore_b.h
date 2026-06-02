#ifndef ARVORE_B_H
#define ARVORE_B_H

typedef struct NoB {
    int n; // Número de chaves atualmente armazenadas no nó
    int folha; // 1 se for folha, 0 caso contrário

    char **chaves; //Vetor de strings
    struct NoB **filhos;
    struct NoB *pai; //guardar o pai para facilitar a inserção e divisão de páginas

} NoB;

typedef struct {
    NoB *raiz; // Ponteiro para a raiz da árvore
    int d; // Ordem da árvore
} ArvoreB;

//criação
NoB *criarNo(int d, int folha);
ArvoreB *criarArvore(int d);

//busca
NoB *buscar(NoB *raiz, char *chave, int *encontrado, int *posicao);

//inserção
void inserirNaPagina(NoB *pagina, char *chave, NoB *filhoDireita);

void dividirPagina(ArvoreB *arvore, NoB *pagina, char **chavePromovida, NoB **novoNo);

void tratarCisao(ArvoreB *arvore, NoB *pagina);

void inserir(ArvoreB *arvore, char *chave);

//carregamento do arquivo
void carregarPokemon(ArvoreB *arvore);

#endif