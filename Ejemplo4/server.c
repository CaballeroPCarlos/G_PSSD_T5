#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    int sockfd, new_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];

    // === CREAR SOCKET PRINCIPAL ===
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error al crear el socket");
        exit(1);
    }

    // === CONFIGURAR DIRECCIÓN DEL SERVIDOR ===
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // === ENLACE (bind) ===
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error en bind");
        exit(1);
    }

    // === ESCUCHAR CONEXIONES ENTRANTES ===
    if (listen(sockfd, 5) == 0)
        printf("Servidor escuchando en el puerto 8080...\n");
    else
    {
        perror("Error en listen");
        exit(1);
    }

    // === BUCLE PRINCIPAL PARA ACEPTAR CLIENTES ===
    while (1)
    {
        addr_size = sizeof(client_addr);
        new_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
        if (new_sock < 0)
        {
            perror("Error en accept");
            continue;
        }

        if (fork() == 0) // Proceso hijo
        {
            close(sockfd); // El hijo no necesita socket de escucha

            // === BUCLE PARA RECIBIR MÚLTIPLES MENSAJES ===
            while (1)
            {
                memset(buffer, 0, sizeof(buffer)); // Limpiar buffer antes de cada recv()
                int bytes = recv(new_sock, buffer, sizeof(buffer), 0);

                if (bytes <= 0) // Cliente se desconectó o hubo error
                    break;

                printf("Mensaje recibido: %s\n", buffer);

                // Si el cliente escribió "salir", terminamos la conexión
                if (strcmp(buffer, "salir") == 0)
                    break;

                // Enviar respuesta
                strcpy(buffer, "Mensaje recibido con éxito!");
                send(new_sock, buffer, strlen(buffer) + 1, 0);
            }

            close(new_sock);
            exit(0); // Termina el proceso hijo
        }
        else
        {
            close(new_sock); // El padre no usa este socket
        }
    }

    close(sockfd);
    return 0;
}
