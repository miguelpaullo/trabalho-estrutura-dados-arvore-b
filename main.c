#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_b.h"

int main() {
    int d;
    int opcao;

    char nome[100];

    printf("Digite a ordem da arvore B: ");
    scanf("%d", &d);

    ArvoreB *arvore = criarArvore(d);

    carregarPokemon(arvore);

    printf("Pokemons carregados com sucesso!\n");

    do {
    printf("\n// ----- // ----- // ARVORE B // ----- // ----- //\n");
    printf("[1] - Buscar\n");
    printf("[2] - Inserir\n");
    printf("[9] - Finalizar\n");
    printf("------------------------------------------------------------\n");
    printf("Entre com a sua opcao: ");

    scanf("%d", &opcao);

    switch(opcao) {
        case 1:
        {
            char nome[100];

            int encontrado;
            int posicao;

            NoB *pagina;

            printf("Nome do Pokemon: ");

            getchar();

            fgets(nome, sizeof(nome), stdin);

            nome[strcspn(nome, "\n")] = '\0';

            pagina = buscar( arvore->raiz, nome, &encontrado, &posicao);

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
            char nome[100];

            printf("Nome do Pokemon: ");

            getchar();

            fgets(nome, sizeof(nome), stdin);

            nome[strcspn(nome, "\n")] = '\0';

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