#include <stdio.h>     // Entrada/salida estándar
#include <stdlib.h>    // Funciones del sistema (exit)
#include <string.h>    // Manipulación de cadenas
#include <unistd.h>    // Cierre de sockets
#include <arpa/inet.h> // Funciones y estructuras para TCP/IP

int main(int argc, char const *argv[])
{
    int sockfd, new_sock;                        // sockfd: socket principal; new_sock: socket del cliente aceptado
    struct sockaddr_in server_addr, client_addr; // Estructuras para almacenar las direcciones del servidor y del cliente
    socklen_t addr_size;                         // Tipo especial para guardar el tamaño de la estructura de dirección
    char buffer[1024];                           // Búfer para enviar y recibir datos

    // === CREACIÓN DEL SOCKET ===
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error al crear el socket");
        exit(1);
    }

    // === CONFIGURACIÓN DE LA DIRECCIÓN DEL SERVIDOR ===
    server_addr.sin_family = AF_INET;         // IPv4
    server_addr.sin_port = htons(8080);       // Puerto de escucha (8080)
    server_addr.sin_addr.s_addr = INADDR_ANY; // Acepta conexiones desde cualquier IP local (no solo localhost)

    // === ENLACE (bind) ===
    // "Asocia" el socket con una dirección (IP y puerto) específicos del sistema.
    // Es necesario para que el servidor sepa en qué puerto escuchar.
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error en bind"); // Ocurre si el puerto está ocupado o no hay permisos.
        exit(1);
    }

    // === ESCUCHAR CONEXIONES ===
    // Pone el socket en modo "pasivo", listo para aceptar conexiones entrantes.
    // El número 5 es la cantidad máxima de conexiones en cola antes de rechazar nuevas.
    if (listen(sockfd, 5) == 0)
        printf("Servidor escuchando en el puerto 8080...\n");
    else
    {
        perror("Error en listen");
        exit(1);
    }

    // === ACEPTAR CONEXIÓN ===
    // Espera (bloquea) hasta que un cliente intente conectarse.
    // Cuando llega una conexión, crea un nuevo socket (new_sock) para comunicarse solo con ese cliente.
    addr_size = sizeof(client_addr);
    new_sock = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);

    // === RECIBIR DATOS DEL CLIENTE ===
    // Lee el mensaje enviado por el cliente y lo guarda en buffer.
    recv(new_sock, buffer, sizeof(buffer), 0);
    printf("Mensaje recibido: %s\n", buffer);

    // === ENVIAR RESPUESTA AL CLIENTE ===
    // Copiamos una respuesta en el buffer y la enviamos de vuelta.
    // strcpy() copia el texto; send() lo transmite al cliente.
    strcpy(buffer, "Mensaje recibido con éxito!");
    send(new_sock, buffer, strlen(buffer) + 1, 0);

    // === CIERRE DE CONEXIONES ===
    // Cerramos primero el socket del cliente (ya atendido),
    // luego el socket principal del servidor.
    close(new_sock);
    close(sockfd);

    return 0;
}
