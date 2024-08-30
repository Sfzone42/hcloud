#include "hcloud.h"

int main(void)
{
    t_server server;
    t_client client;
    const char *server_ip = "127.0.0.1";
    const char *test_message = "Hello from client!";
    const char *test_file_path = "test_file.txt";

    printf("Configuring server...\n");
    ceate_server(&server, server_ip);
    
    printf("Configuring client...\n");
    configure_client(&client, server_ip);
    
    printf("Sending message to server...\n");
    process_command(&client, test_message);

    printf("Sending file to server...\n");
    send_file(client.client_socket, test_file_path);

    char buffer[BUFFER_SIZE];
    printf("Receiving data from server...\n");
    receive_data(client.client_socket, buffer, sizeof(buffer));
    printf("Received data: %s\n", buffer);

    printf("Receiving file from server...\n");
    receive_file(client.client_socket, "received_file.txt");

    printf("Processing message on server...\n");
    process_message(client.client_socket, client.username, test_message);

    close(client.client_socket);
    close(server.server_socket);

    return 0;
}

