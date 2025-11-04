#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 3

int main()
{
    int server_fd, new_socket[MAX_CLIENTS];
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Crear socket TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error al crear socket");
        exit(EXIT_FAILURE);
    }

    // Configurar socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("Error de setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Enlazar socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }

    printf("Coordinador iniciado en puerto %d.\nEsperando participantes...\n", PORT);

    // Aceptar conexiones de los participantes
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if ((new_socket[i] = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Error al aceptar conexión");
            exit(EXIT_FAILURE);
        }
        printf("Participante %d conectado\n", i + 1);
    }

    // === FASE 1: PREPARE ===
    printf("\n=== FASE 1: PREPARE ===\n");
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        send(new_socket[i], "PREPARE", strlen("PREPARE") + 1, 0);
    }

    int votos_commit = 0;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        memset(buffer, 0, sizeof(buffer));
        read(new_socket[i], buffer, sizeof(buffer));
        printf("Participante %d votó: %s\n", i + 1, buffer);
        if (strcmp(buffer, "COMMIT") == 0)
            votos_commit++;
    }

    // === FASE 2: DECISIÓN ===
    printf("\n=== FASE 2: DECISIÓN ===\n");
    const char *decision;
    if (votos_commit == MAX_CLIENTS)
    {
        decision = "GLOBAL_COMMIT";
    }
    else
    {
        decision = "GLOBAL_ABORT";
    }

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        send(new_socket[i], decision, strlen(decision) + 1, 0);
    }

    printf("Decisión global: %s\n", decision);

    // Cerrar conexiones
    for (int i = 0; i < MAX_CLIENTS; i++)
        close(new_socket[i]);
    close(server_fd);

    return 0;
}
