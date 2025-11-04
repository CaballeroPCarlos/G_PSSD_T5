#include <stdio.h>

int main(int argc, char const *argv[])
{
    int voto;
    printf("Ingresa tu voto (1 = commit, 0 = abort): ");
    scanf("%d", &voto);

    if (voto == 1)
        printf("Has votado COMMIT\n");
    else
        printf("Has votado ABORT\n");
    return 0;
}
