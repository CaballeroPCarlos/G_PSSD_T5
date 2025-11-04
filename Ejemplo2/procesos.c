#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    for (int i = 0; i < 3; i++)
    {
        pid_t pid = fork();
        // Se duplica el proceso.
        // El cual se considera un hijo.
        // Y este tendrá como pid = 0.

        if (pid == 0)
        {
            printf("Soy el participante %d (PID=%d)\n", i + 1, getpid());
            return 0;
            // Evita que los hijos también se dupliquen.
        }
    }

    return 0;
}
