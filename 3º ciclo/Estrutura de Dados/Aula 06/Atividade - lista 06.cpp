#include <stdio.h>
#include <stdlib.h>

int posicao, continua;

struct TbFicha {
    int codigo;
    char nome[40];
    char bairro[40];
    char cidade[60];
    char telefone[20];
    char cpf[15];
    char rg[20];
};

struct TbProduto {
    int codproduto;
    char nomeproduto[10];
    float valor;
    int qtd;
};

struct TbFicha ficha[10];
struct TbProduto produto[10];

int clienteCadastrado[10] = {0};
int produtoCadastrado[10] = {0};

void insereCliente() {
    printf("Informe o codigo do cliente: ");
    scanf("%d", &ficha[posicao].codigo);
    printf("Informe o nome do cliente: ");
    fflush(stdin);
    fgets(ficha[posicao].nome, 40, stdin);
    printf("Informe o bairro: ");
    fflush(stdin);
    fgets(ficha[posicao].bairro, 40, stdin);
    printf("Informe a cidade: ");
    fflush(stdin);
    fgets(ficha[posicao].cidade, 60, stdin);
    printf("Informe o telefone: ");
    scanf("%19s", ficha[posicao].telefone);
    printf("Informe o CPF: ");
    scanf("%14s", ficha[posicao].cpf);
    printf("Informe o RG: ");
    scanf("%19s", ficha[posicao].rg);
    clienteCadastrado[posicao] = 1;
}

void exibeCliente() {
    printf("\n Dados do cadastro \n");
    printf("Codigo: %d\n", ficha[posicao].codigo);
    printf("Nome: %s", ficha[posicao].nome);
    printf("Bairro: %s", ficha[posicao].bairro);
    printf("Cidade: %s", ficha[posicao].cidade);
    printf("Telefone: %s\n", ficha[posicao].telefone);
    printf("CPF: %s\n", ficha[posicao].cpf);
    printf("RG: %s\n", ficha[posicao].rg);
}

void insereProduto() {
    printf("Informe o codigo do produto: ");
    scanf("%d", &produto[posicao].codproduto);
    printf("Informe o nome do produto: ");
    fflush(stdin);
    fgets(produto[posicao].nomeproduto, 10, stdin);
    printf("Informe o valor do produto: ");
    scanf("%f", &produto[posicao].valor);
    printf("Informe a quantidade: ");
    scanf("%d", &produto[posicao].qtd);
    produtoCadastrado[posicao] = 1;
}

void exibeProduto() {
    printf("\n Dados do Produto \n");
    printf("Codigo: %d\n", produto[posicao].codproduto);
    printf("Nome: %s", produto[posicao].nomeproduto);
    printf("Valor: R$ %.2f\n", produto[posicao].valor);
    printf("Quantidade: %d\n", produto[posicao].qtd);
}

int main() {
    int opcao;

    do {
        printf("\n Menu Principal \n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Consultar cliente\n");
        printf("3 - Cadastrar produto\n");
        printf("4 - Consultar produto\n");
        printf("0 - Sair\n");

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao >= 1 && opcao <= 4) {
            printf("Informe uma posicao de 0 a 9: ");
            scanf("%d", &posicao);

            if (posicao < 0 || posicao >= 10) {
                printf("Posicao invalida!\n");
                continue;
            }
        }

        switch (opcao) {
            case 1:
                insereCliente();
                printf("\nCliente cadastrado!\n");
                break;

            case 2:
                if (clienteCadastrado[posicao] == 1) {
                    exibeCliente();
                } else {
                    printf("Cliente nao cadastrado!\n");
                }
                break;

            case 3:
                insereProduto();
                printf("\nProduto cadastrado!\n");
                break;

            case 4:
                if (produtoCadastrado[posicao] == 1) {
                    exibeProduto();
                } else {
                    printf("Produto nao cadastrado!\n");
                }
                break;

            case 0:
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
    return 0;
}

