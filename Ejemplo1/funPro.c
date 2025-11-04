#include <stdio.h>

void participante(int id)
{
    printf("Participante %d listo para votar\n", id);
}

int main(int argc, char const *argv[])
{
    for (int i = 1; i <= 3; i++)
    {
        participante(i);
    }

    return 0;
}
