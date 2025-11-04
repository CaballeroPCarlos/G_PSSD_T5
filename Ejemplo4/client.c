#include <stdio.h>     // Entrada/salida estándar
#include <stdlib.h>    // Funciones del sistema (exit)
#include <string.h>    // Manejo de cadenas
#include <unistd.h>    // close()
#include <arpa/inet.h> // Funciones y estructuras para TCP/IP

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // === CREACIÓN DEL SOCKET ===
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error al crear el socket");
        exit(1);
    }

    // === CONFIGURACIÓN DEL SERVIDOR ===
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // === CONEXIÓN AL SERVIDOR ===
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error al conectarse al servidor");
        exit(1);
    }

    printf("Conectado al servidor. Escribe 'salir' para terminar.\n");

    // === BUCLE PRINCIPAL PARA ENVIAR Y RECIBIR MENSAJES ===
    while (1)
    {
        printf("Escribe un mensaje: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Eliminar salto de línea al final
        buffer[strcspn(buffer, "\n")] = '\0';

        // Si el usuario escribe "salir", termina el bucle
        if (strcmp(buffer, "salir") == 0)
            break;

        // Enviar mensaje al servidor
        send(sockfd, buffer, strlen(buffer) + 1, 0);

        // Recibir respuesta del servidor
        recv(sockfd, buffer, sizeof(buffer), 0);
        printf("Respuesta del servidor: %s\n", buffer);
    }

    // === CIERRE DE CONEXIÓN ===
    close(sockfd);
    printf("Conexión cerrada. Adiós!\n");

    return 0;
}
