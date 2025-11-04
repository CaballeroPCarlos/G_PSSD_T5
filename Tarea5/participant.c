#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080

// Función para verificar si la cadena es un número válido (solo dígitos)
int es_numero_valido(const char *str)
{
    for (int i = 0; str[i] != '\0' && str[i] != '\n'; i++)
    {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char input[10];

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error al crear socket\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Dirección inválida\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Error al conectar con el coordinador\n");
        return -1;
    }

    // Esperar mensaje PREPARE
    read(sock, buffer, sizeof(buffer));
    printf("Coordinador dice: %s\n", buffer);

    // Pedir voto al usuario con validación
    int voto_num;
    do
    {
        printf("Ingresa tu voto (1 = COMMIT, 0 = ABORT): ");
        fgets(input, sizeof(input), stdin);

        // Verificar si es número
        if (!es_numero_valido(input))
        {
            printf("Entrada inválida, debes ingresar un número 0 o 1.\n");
            continue;
        }

        voto_num = atoi(input);

        if (voto_num != 0 && voto_num != 1)
        {
            printf("Número inválido, solo 0 o 1.\n");
        }

    } while (voto_num != 0 && voto_num != 1);

    const char *voto = (voto_num == 1) ? "COMMIT" : "ABORT";
    printf("Mi voto: %s\n", voto);
    send(sock, voto, strlen(voto) + 1, 0);

    // Recibir decisión final
    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, sizeof(buffer));
    printf("Decisión final del coordinador: %s\n", buffer);

    close(sock);
    return 0;
}
