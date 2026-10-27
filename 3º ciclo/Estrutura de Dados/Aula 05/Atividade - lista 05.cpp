
#include <stdio.h>
#include <string.h>

int main() {
    char times[12][50], nomeAux[50];
    int pontos[12], saldo[12];
    int aux;

    for (int i = 0; i < 12; i++) {
        printf("\nTime %d\n", i + 1);

        printf("Nome do time: ");
        scanf(" %49[^\n]", times[i]);

        printf("Pontos ganhos: ");
        scanf("%d", &pontos[i]);

        printf("Saldo de gols: ");
        scanf("%d", &saldo[i]);
    }

    printf("\n Times cadastrados \n");

    for (int i = 0; i < 12; i++) {
        printf("%s - Pontos: %d - Saldo: %d\n",
               times[i], pontos[i], saldo[i]);
    }

    for (int i = 0; i < 11; i++) {
        for (int j = i + 1; j < 12; j++) {

            if (pontos[i] < pontos[j] ||
               (pontos[i] == pontos[j] && saldo[i] < saldo[j])) {

                strcpy(nomeAux, times[i]);
                strcpy(times[i], times[j]);
                strcpy(times[j], nomeAux);

                aux = pontos[i];
                pontos[i] = pontos[j];
                pontos[j] = aux;

                aux = saldo[i];
                saldo[i] = saldo[j];
                saldo[j] = aux;
            }
        }
    }

    printf("\n Classificacao do Campeonato \n");

    printf("Posicao | Time | Pontos | Saldo de Gols\n");

    for (int i = 0; i < 12; i++) {
        printf("%d | %s | %d | %d\n",
               i + 1, times[i], pontos[i], saldo[i]);
    }

    return 0;
}

