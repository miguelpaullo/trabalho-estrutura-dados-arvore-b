#include <stdio.h>
#include <stdlib.h>
#include <string.h> //pois vamos comparar strings
#include "arvore_b.h"

NoB *criarNo(int d, int folha) {
    //Recece a ordem da árvore e se o nó é folha ou não
    //retorna um ponteiro para o nó criado

    NoB *novo = (NoB *) malloc(sizeof(NoB)); //Aloca memória para o novo nó

    if (novo == NULL) //Caso não consiga alocar memória
        return NULL;

    novo->n = 0; //inicializa a quantidade de chaves como 0
    novo->folha = folha;
    novo->pai = NULL;

    //Criando vetor de chaves e permitindo uma página temporariamente cheia (2d chaves) para facilitar a divisão de páginas
    novo->chaves = (char **) malloc((2 * d + 1) * sizeof(char *)); 
    novo->filhos = (NoB **) malloc((2 * d + 2) * sizeof(NoB *)); //Criando vetor de filhos
    
    //Inicializando os ponteiros das chaves
    for (int i = 0; i < (2 * d + 1); i++) {
        novo->chaves[i] = NULL;
    }

    //Inicializando os ponteiros dos filhos
    for (int i = 0; i < (2 * d + 2); i++) {
        novo->filhos[i] = NULL;
    }

    return novo;
}

ArvoreB *criarArvore(int d) {
    //Recece a ordem da árvore e retorna um ponteiro para a árvore criada

    ArvoreB *arvore = (ArvoreB *) malloc(sizeof(ArvoreB)); //Aloca memória para a árvore

    if (arvore == NULL) //Caso não consiga alocar memória
        return NULL;

    arvore->d = d; //Guarda a ordem da árvore
    arvore->raiz = NULL; //Inicia a raiz como NULL

    return arvore; 
}

NoB *buscar(NoB *raiz, char *chave, int *encontrou, int *posicao) {
    NoB *p = raiz;

    *encontrou = 0;

    //Enquanto ainda existir uma página
    while (p != NULL) {
        int i = 0;

        //Procurando a posição dentro da página
        while (i < p->n && strcmp(chave, p->chaves[i]) > 0) {
            i++;
        }

        //Verificando se a chave foi encontrada
        if (i < p->n && strcmp(chave, p->chaves[i]) == 0) {
            *encontrou = 1;
            *posicao = i;

            return p;
        }
        
        //quando a chave não é encontrada, a posição i indica onde ela deveria estar
        *posicao = i;

        //retornamos a folha onde a chave deveria estar, quando chegamos ao final e não encontramos
        if (p->folha) {
            return p;
        }
        
        //Seguindo para o filho adequado, percorrendo os outros níveis
        p = p->filhos[i];
    }

    return NULL;
}


void inserirNaPagina(NoB *pagina, char *chave, NoB *filhoDireita) {
    //O filho à esquerda da chave promovida permanece na estrutura
    //Apenas o novo filho à direita precisa ser inserido

    //começando pelo ultimo elemento da pagina
    int i = pagina->n - 1;

    //Enquanto a nova chave for menor que a chave atual da página, vamos deslocar as chaves e os filhos para a direita
    while (i >= 0 && strcmp(chave, pagina->chaves[i]) < 0) {
        pagina->chaves[i + 1] = pagina->chaves[i];
        pagina->filhos[i + 2] = pagina->filhos[i + 1];

        //voltando para o elemento anterior da página
        i--;
    }

    //Quando encontramos o lugar certo
    pagina->chaves[i + 1] = chave;
    pagina->filhos[i + 2] = filhoDireita;

    if(filhoDireita != NULL) {
        //Atualizando o pai do filho direito
        filhoDireita->pai = pagina;
    }

    //atualizando o número de chaves na página
    pagina->n++;
}

void dividirPagina( ArvoreB *arvore, NoB *pagina, char **chavePromovida, NoB **novoNo) {
    //Dividir a página em duas
    //Essa função apenas divide a página, não lida com a promoção da chave para o pai

    //guadar o d
    int d = arvore->d;
    int i;

    //criando o novo nó e verificando se é folha ou não
    *novoNo = criarNo(d, pagina->folha);

    //Pegando a chave central
    *chavePromovida = pagina->chaves[d];

    //Copiando a metade direita das chaves para o novo nó
    for(i = 0; i < d; i++) {
        (*novoNo)->chaves[i] = pagina->chaves[d + 1 + i];
    }

    //Se não for folha -> copiamos os filhos
    if(!pagina->folha)
    {
        for(i = 0; i <= d; i++) {
            (*novoNo)->filhos[i] = pagina->filhos[d + 1 + i];

            if((*novoNo)->filhos[i] != NULL) {
                //Atualizando o pai dos filhos do novo nó
                (*novoNo)->filhos[i]->pai = *novoNo;
            }
        }
    }

    //Ajustando o número de chaves em cada página
    pagina->n = d;
    (*novoNo)->n = d;

    //Atualizando o pai do novo nó
    (*novoNo)->pai = pagina->pai;
}

void tratarSplit(ArvoreB *arvore, NoB *pagina) {
    //Tratando a divisão de páginas e a promoção de chaves para o pai

    //Variáveis auxiliares
    char *chavePromovida;
    NoB *novoNo;
    NoB *pai;

    //Enquanto a página estiver cheia (mais de 2d chaves)
    while (pagina != NULL && pagina->n > 2 * arvore->d) {
        //Dividindo a página
        dividirPagina(arvore, pagina, &chavePromovida, &novoNo);

        // caso especial: página é a raiz
        if (pagina->pai == NULL) {

            //Criando uma nova raiz
            NoB *novaRaiz = criarNo(arvore->d, 0);

            //Promovendo a chave para a nova raiz
            novaRaiz->chaves[0] = chavePromovida;

            //Ajustando os filhos da nova raiz
            novaRaiz->filhos[0] = pagina;
            novaRaiz->filhos[1] = novoNo;

            novaRaiz->n = 1;

            pagina->pai = novaRaiz;
            novoNo->pai = novaRaiz;

            //Atualizando a raiz da árvore
            arvore->raiz = novaRaiz;

            return;
        }

        //caso normal: página tem pai
        pai = pagina->pai;

        //inserindo a chave promovida
        inserirNaPagina( pai, chavePromovida, novoNo);

        //continua a propagação
        pagina = pai;
    }
}

void inserir(ArvoreB *arvore, char *chave) {
    //Inserindo uma chave na árvore, lidando com a divisão de páginas e promoção de chaves para o pai
    NoB *pagina;

    int encontrado;
    int posicao;

    // árvore vazia
    if(arvore->raiz == NULL) {
        arvore->raiz = criarNo(arvore->d, 1);

        char *novaChave = malloc(strlen(chave) + 1);

        strcpy(novaChave, chave);

        arvore->raiz->chaves[0] = novaChave;

        arvore->raiz->n = 1;

        return;
    }

    //buscando a página onde a chave deve ser inserida
    pagina = buscar( arvore->raiz, chave, &encontrado, &posicao);

    //Nesse cenário não permitiremos chaves repetidas
    if(encontrado) {
        printf("Chave ja existe.\n");
        return;
    }

    char *novaChave = malloc(strlen(chave) + 1);

    strcpy(novaChave, chave);

    //Inserindo...como estamos inserindo numa folha, o filho direito é NULL
    inserirNaPagina(pagina, novaChave, NULL);

    //Verificando se a página ficou cheia e precisa ser dividida
    tratarSplit(arvore, pagina);
}

void carregarPokemon(ArvoreB *arvore) {
    //Função para carregar os nomes dos pokemons do arquivo e inserir na árvore

    //Declarando o arquivo
    FILE *arquivo;

    //para leitura do nome do pokemon
    char nome[100];

    //abrindo o arquivo para leitura
    arquivo = fopen("pokemon_names.txt", "r");

    //caso de erro ao abrir o arquivo
    if (arquivo == NULL) {
        printf("Erro ao abrir pokemon_names.txt\n");
        return;
    }

    //lendo linha por linha
    while (fgets(nome, sizeof(nome), arquivo) != NULL) {
        nome[strcspn(nome, "\n")] = '\0';

        //inserindo
        inserir(arvore, nome);
    }

    //fechando o arquivo
    fclose(arquivo);
}