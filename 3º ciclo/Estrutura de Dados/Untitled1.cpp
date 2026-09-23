#include <stdio.h>
#include <string.h>

#define MAX_PRODUTOS 100

typedef struct {
    int codigo;
    char nome[50];
    float valor;
    int estoque;
} Produto;

typedef struct {
    int codigo;
    int quantidade;
    float subtotal;
    char nome[50];
} ItemVenda;

void exibirProdutos(Produto produtos[], int n) {
    printf("\n%-10s | %-15s | %-10s | %-10s\n", "Código", "Produto", "Valor", "Estoque");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-10d | %-15s | %-10.2f | %-10d\n", 
               produtos[i].codigo, produtos[i].nome, produtos[i].valor, produtos[i].estoque);
    }
    printf("\n");
}

int buscarEExibirProduto(Produto produtos[], int n, int codigo) {
    for (int i = 0; i < n; i++) {
        if (produtos[i].codigo == codigo) {
            printf("\nProduto Encontrado:\n");
            printf("Código: %d\n", produtos[i].codigo);
            printf("Nome: %s\n", produtos[i].nome);
            printf("Valor: R$ %.2f\n", produtos[i].valor);
            printf("Estoque: %d\n", produtos[i].estoque);
            return i; 
        }
    }
    return -1; }

void realizarVendas(Produto produtos[], int n) {
    ItemVenda carrinho[MAX_PRODUTOS];
    int totalItensComprados = 0;
    char opcao;

    do {
        int codigo, qtdDesejada;
        printf("\nDigite o código do produto que deseja comprar: ");
        scanf("%d", &codigo);

        int indice = -1;
        for (int i = 0; i < n; i++) {
            if (produtos[i].codigo == codigo) {
                indice = i;
                break;
            }
        }

        if (indice == -1) {
            printf("Código de produto não cadastrado!\n");
        } else {
            printf("Digite a quantidade de compra: ");
            scanf("%d", &qtdDesejada);

            if (qtdDesejada > produtos[indice].estoque) {
                printf("A quantidade solicitada não existe em estoque! (Disponível: %d)\n", produtos[indice].estoque);
            } else {
                produtos[indice].estoque -= qtdDesejada;

                carrinho[totalItensComprados].codigo = codigo;
                carrinho[totalItensComprados].quantidade = qtdDesejada;
                carrinho[totalItensComprados].subtotal = qtdDesejada * produtos[indice].valor;
                strcpy(carrinho[totalItensComprados].nome, produtos[indice].nome);
                
                totalItensComprados++;
                printf("Produto adicionado ao carrinho com sucesso!\n");
            }
        }

        printf("Deseja inserir mais produtos? (S/N): ");
        scanf(" %c", &opcao);

    } while (opcao == 'S' || opcao == 's');

    if (totalItensComprados > 0) {
        float totalCompra = 0.0;
        printf("\n================ RESUMO DA COMPRA ================\n");
        printf("%-15s | %-5s | %-10s\n", "Produto", "Qtd", "Sub Total");
        printf("--------------------------------------------------\n");
        
        for (int i = 0; i < totalItensComprados; i++) {
            printf("%-15s | %-5d | %-10.2f\n", 
                   carrinho[i].nome, carrinho[i].quantidade, carrinho[i].subtotal);
            totalCompra += carrinho[i].subtotal;
        }
        
        printf("--------------------------------------------------\n");
        printf("Total da compra: R$ %.2f\n", totalCompra);
        printf("==================================================\n");
    } else {
        printf("\nNenhuma compra foi realizada.\n");
    }
}

int main() {
    int n;

    printf("Quantos produtos deseja cadastrar? ");
    scanf("%d", &n);

    Produto produtos[n];

    for (int i = 0; i < n; i++) {
        printf("\n--- Produto %d ---\n", i + 1);
        printf("Código: ");
        scanf("%d", &produtos[i].codigo);
        printf("Nome: ");
        scanf(" %[^\n]", produtos[i].nome);         	printf("Valor: ");
        scanf("%f", &produtos[i].valor);
        printf("Quantidade em Estoque: ");
        scanf("%d", &produtos[i].estoque);
    }

    exibirProdutos(produtos, n);

    int codConsulta;
    printf("Digite um código de produto para consultar: ");
    scanf("%d", &codConsulta);
    if (buscarEExibirProduto(produtos, n, codConsulta) == -1) {
        printf("Produto não cadastrado!\n");
    }

    realizarVendas(produtos, n);

    return 0;
}
