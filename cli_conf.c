/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_conf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhabacuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:36:18 by lhabacuc          #+#    #+#             */
/*   Updated: 2024/08/30 15:36:19 by lhabacuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hcloud.h"

void	configure_client(t_client *client, const char *server_ip)
{
	struct sockaddr_in server_addr;

	client->client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client->client_socket == -1)
	{
		perror("Erro ao criar socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTP);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);

	if (connect(client->client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("Erro ao conectar");
		close(client->client_socket);
		exit(EXIT_FAILURE);
	}
}

void	command(t_client *client, const char *command)
{
	if (strcmp(command, "SEND") == 0)
	{
		char *file_path = readline("Digite o caminho do arquivo para enviar: ");
		send_file(client->client_socket, file_path);
		free(file_path);
	}
	else if (strcmp(command, "RECEIVE") == 0)
	{
		char *file_path = readline("Digite o caminho para salvar o arquivo recebido: ");
		receive_file(client->client_socket, file_path);
		free(file_path);
	}
	else
		send_data(client->client_socket, command);
}

void	process_command(t_client *client, const char *command)
{
	char method[10];
	char path[BUFFER_SIZE];
	char request[2048];
	int bytes_sent;

	memset(method, 0, sizeof(method));
	memset(path, 0, sizeof(path));
	memset(request, 0, sizeof(request));

	sscanf(command, "%9s %1023s", method, path);

	int len = snprintf(request, sizeof(request), "%s:%s:%s:%s\n", method, path, client->prev, client->username);
	len--;
	bytes_sent = write(client->client_socket, request, strlen(request));
	if (bytes_sent == -1)
	{
		perror("Erro ao enviar a requisição");
		close(client->client_socket);
		exit(EXIT_FAILURE);
	}

	printf("Requisição enviada: %s\n", request);

	if (strcmp(method, "GET") == 0)
	{
		char buffer[BUFFER_SIZE];
		ssize_t bytes_received;

		while ((bytes_received = read(client->client_socket, buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytes_received] = '\0';
			printf("%s", buffer);
		}

		if (bytes_received == -1)
			perror("Erro ao receber a resposta do servidor");
	}
}

void	prop_command(t_client *client, const char *command)
{
	char	buffer[BUFFER_SIZE];

	if (strncmp(command, "GET", 3) == 0)
	{
		strncpy(client->method, "GET", sizeof(client->method) - 1);
		client->method[sizeof(client->method) - 1] = '\0';
		sscanf(command, "GET %s", client->path);
		send_request(client, NULL);
	}
	else if (strncmp(command, "POST", 4) == 0)
	{
		strncpy(client->method, "POST", sizeof(client->method) - 1);
		client->method[sizeof(client->method) - 1] = '\0';
		sscanf(command, "POST %s", client->path);
		printf("Enter data to send: ");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		send_request(client, buffer);
	}
	else if (strncmp(command, "PUT", 3) == 0)
	{
		strncpy(client->method, "PUT", sizeof(client->method) - 1);
		client->method[sizeof(client->method) - 1] = '\0';
		sscanf(command, "PUT %s", client->path);
		printf("Enter data to upload: ");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		send_request(client, buffer);
	}
	else if (strncmp(command, "DELETE", 6) == 0)
	{
		strncpy(client->method, "DELETE", sizeof(client->method) - 1);
		client->method[sizeof(client->method) - 1] = '\0';
		sscanf(command, "DELETE %s", client->path);
		send_request(client, NULL);
	}
	else if (strncmp(command, "CMD", 3) == 0)
	{
		strncpy(client->method, "CMD", sizeof(client->method) - 1);
		client->method[sizeof(client->method) - 1] = '\0';
		sscanf(command, "CMD %s", client->path);
		printf("Enter shell command: ");
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")] = 0;
		send_request(client, buffer);
	}
	else
		printf("Método não suportado: %s\n", command);
}

void	http_send(t_client *client, const char *request)
{
	send(client->client_socket, request, strlen(request), 0);

	char buffer[BUFFER_SIZE];
	int bytes_received = recv(client->client_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received > 0)
	{
		buffer[bytes_received] = '\0';
		printf("Resposta do servidor:\n%s\n", buffer);
	}
	else
		printf("Erro ao receber resposta do servidor\n");
	close(client->client_socket);
}
/*
int main()
{
	t_client client;
	configure_client(&client, "127.0.0.1");

	const char *request = "GET /index.html HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";
	send_request(&client, request);

	return 0;
}
*/
