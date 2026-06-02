#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_b.h"

int main() {
    //armazena a ordem da árvore e a opção do menu
    int d;
    int opcao;

    //Reservando espaço para o nome do pokemon
    char nome[100];

    printf("Digite a ordem da arvore B: ");
    scanf("%d", &d);

    //Criando a árvore B
    ArvoreB *arvore = criarArvore(d);
    
    //Carregando os pokemons do arquivo txt para a árvore
    carregarPokemon(arvore);

    printf("Pokemons carregados com sucesso!\n");

    //Menu
    do {
    printf("\n// ----- // ----- // ARVORE B // ----- // ----- //\n");
    printf("[1] - Buscar\n");
    printf("[2] - Inserir\n");
    printf("[9] - Finalizar\n");
    printf("------------------------------------------------------------\n");
    printf("Entre com a sua opcao: ");

    scanf("%d", &opcao);
    
    //Estrutura Switch para lidar com as opções do menu
    switch(opcao) {

        //Buscar Pokemon
        case 1:
        {
            //armazena o nome digitado
            char nome[100];

            //resultado da busca
            int encontrado;
            int posicao;

            //ponteiro para a página onde o pokemon foi encontrado
            NoB *pagina;

            printf("Nome do Pokemon: ");

            //removendo o \n deixado pelo scanf
            getchar();

            //lendo o nome do pokemon
            fgets(nome, sizeof(nome), stdin);

            //removendo o \n deixado pelo fgets
            nome[strcspn(nome, "\n")] = '\0';

            //buscando o pokemon na árvore
            pagina = buscar( arvore->raiz, nome, &encontrado, &posicao);

            //resultado da busca
            if(encontrado){
                printf("\nPokemon encontrado!\n");
                printf("Posicao na pagina: %d\n", posicao);
            }else{
                printf("\nPokemon nao encontrado.\n");
            }
            break;
        }

        case 2:
        {
            //armazena o nome digitado
            char nome[100];

            printf("Nome do Pokemon: ");

            getchar();

            fgets(nome, sizeof(nome), stdin);

            nome[strcspn(nome, "\n")] = '\0';

            //Realizando a inserção do pokemon na árvore
            inserir(arvore, nome);

            printf("Pokemon inserido com sucesso!\n");

            break;
        }

        case 9:
        {
            printf("Encerrando programa...\n");
            break;
        }

        default:
        {
            printf("Opcao invalida!\n");
        }
    }

    } while(opcao != 9);

    return 0;
}