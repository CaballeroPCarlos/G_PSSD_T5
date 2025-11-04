// Incluimos las librerías necesarias:

#include <stdio.h>     // Permite usar funciones de entrada/salida, como printf() y fgets().
#include <stdlib.h>    // Se usa para funciones del sistema como exit(), que termina el programa en caso de error.
#include <string.h>    // Sirve para manipular cadenas de texto (strlen(), strcspn()).
#include <unistd.h>    // Contiene funciones del sistema operativo como close(), usada para cerrar el socket.
#include <arpa/inet.h> // Define las estructuras y funciones necesarias para manejar conexiones TCP/IP (como socket(), connect(), inet_addr()).

int main()
{
    int sockfd;                     // Este entero guardará el "descriptor" del socket, que identifica la conexión en el sistema.
    struct sockaddr_in server_addr; // Esta estructura guarda toda la información del servidor: familia, IP y puerto.
    char buffer[1024];              // Un arreglo de caracteres que servirá para enviar y recibir mensajes de texto.

    // === CREACIÓN DEL SOCKET ===
    // Un "socket" es como un punto de conexión para comunicarse entre computadoras.
    // Aquí se crea un socket TCP (SOCK_STREAM) sobre IPv4 (AF_INET).
    // El último parámetro (0) elige el protocolo por defecto (TCP en este caso).
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Verificamos si la creación del socket falló.
    // Si socket() devuelve un número negativo, algo salió mal (por ejemplo, falta de permisos o recursos).
    if (sockfd < 0)
    {
        perror("Error al crear el socket"); // Muestra un mensaje explicando el error del sistema.
        exit(1);                            // Termina el programa con un código de error (1 = error).
    }

    // === CONFIGURACIÓN DE LA DIRECCIÓN DEL SERVIDOR ===
    // Estas líneas preparan la estructura con la dirección a la que el cliente quiere conectarse.
    // Si no se configura correctamente, el socket no sabrá a qué servidor dirigirse.

    server_addr.sin_family = AF_INET;                     // Especifica que se usará la familia de direcciones IPv4.
    server_addr.sin_port = htons(8080);                   // Indica el puerto del servidor (8080).
                                                          // La función htons() convierte el número al formato de red (big-endian).
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Convierte la dirección "127.0.0.1" (localhost) en formato binario.
                                                          // Esto indica que el cliente se conectará al servidor local.

    // === CONEXIÓN AL SERVIDOR ===
    // Intentamos establecer la conexión TCP con el servidor en la dirección configurada.
    // Si el servidor no está disponible o hay un error de red, connect() devuelve -1.
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error al conectarse al servidor"); // Muestra la razón del error (por ejemplo, "Connection refused").
        exit(1);                                   // Sale del programa porque sin conexión no se puede continuar.
    }

    // Si llegamos aquí, la conexión se logró correctamente.
    printf("Conectando al servidor\n");

    // === ENVÍO DE MENSAJE ===
    // Pedimos al usuario que escriba un texto para enviarlo al servidor.
    printf("Escribe un mensaje: ");

    // fgets() lee una línea completa desde el teclado (stdin) y la guarda en buffer.
    // Se detiene cuando se presiona Enter o se llena el búfer.
    fgets(buffer, sizeof(buffer), stdin);

    // fgets() guarda también el salto de línea '\n' al final del texto, lo cual no queremos enviar.
    // Esta línea lo elimina reemplazando el '\n' con un carácter nulo '\0' (fin de cadena).
    buffer[strcspn(buffer, "\n")] = '\0';

    // Ahora enviamos el mensaje al servidor.
    // send() toma el socket, el mensaje, su tamaño (strlen(buffer)+1 para incluir el '\0'), y las banderas (0 por defecto).
    // El '\0' final asegura que el servidor reciba una cadena completa terminada correctamente.
    send(sockfd, buffer, strlen(buffer) + 1, 0);

    // === RECEPCIÓN DE RESPUESTA ===
    // recv() espera una respuesta del servidor.
    // Se queda bloqueado (esperando) hasta que el servidor envía algo.
    // El mensaje recibido se guarda en el mismo buffer.
    recv(sockfd, buffer, sizeof(buffer), 0);

    // Mostramos en pantalla la respuesta que el servidor haya enviado.
    printf("Respuesta del servidor: %s\n", buffer);

    // === CIERRE DE CONEXIÓN ===
    // Una vez que terminamos la comunicación, cerramos el socket.
    // Esto libera el recurso del sistema y finaliza correctamente la conexión TCP.
    close(sockfd);

    // Indicamos que el programa terminó sin errores.
    return 0;
}
