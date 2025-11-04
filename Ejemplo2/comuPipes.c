#include <stdio.h>
#include <unistd.h> // Librería con funciones del sistema UNIX (fork, pipe, read, write, close)
#include <string.h>

int main(int argc, char const *argv[])
{
    int fd[2];
    pipe(fd);
    // El sistema operativo crea un canal de comunicación interno (una “tubería” o pipe).
    // Le asigna dos descriptores de archivo (file descriptors), es decir, dos enteros:
    // - fd[0] se usa para leer datos del pipe.
    // - fd[1] se usa para escribir datos en el pipe.

    if (fork() == 0)
    // fork() crea un nuevo proceso.
    // Si el valor devuelto es 0, significa que este bloque lo ejecuta el proceso hijo.
    {
        close(fd[1]);
        // El hijo cierra el extremo de escritura, porque solo necesita leer.

        char msg[20];
        read(fd[0], msg, sizeof(msg));
        // Lee datos desde el extremo de lectura del pipe (fd[0]) y los guarda en msg.
        // sizeof(msg) indica el número máximo de bytes a leer (20 en este caso).
        // read() se bloquea hasta que haya algo que leer o el extremo de escritura se cierre.

        printf("Hijo recibió: %s\n", msg);
        // Imprime el mensaje recibido desde el proceso padre.
    }
    else
    {
        close(fd[0]);
        // El padre cierra el extremo de lectura, porque solo escribirá.

        char *mensaje = "PREPARE";
        write(fd[1], mensaje, strlen(mensaje) + 1);
        // Escribe el contenido de 'mensaje' en el pipe.
        // strlen(mensaje) + 1 incluye el carácter nulo '\0' para que la cadena se interprete correctamente.
    }

    return 0;
}
