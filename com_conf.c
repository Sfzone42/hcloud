/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   com_conf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhabacuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:36:26 by lhabacuc          #+#    #+#             */
/*   Updated: 2024/08/30 15:36:28 by lhabacuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hcloud.h"

int	client[BUFFER_CLI];
int	client_exists;

void	send_data(int client_socket, const char *data)
{
	if (write(client_socket, data, strlen(data)) == -1)
	{
		perror("Erro ao enviar dados");
		close(client_socket);
		exit(EXIT_FAILURE);
	}
}

void	receive_data(int client_socket, char *buffer, size_t buffer_size)
{
	ssize_t bytes_read;

	bytes_read = read(client_socket, buffer, buffer_size - 1);
	if (bytes_read == -1)
	{
		perror("Erro ao receber dados");
		close(client_socket);
		exit(EXIT_FAILURE);
	}
	else if (bytes_read == 0)
	{
		printf("Conexão fechada\n");
		close(client_socket);
		exit(EXIT_SUCCESS);
	}
	buffer[bytes_read] = '\0';
}

void	receive_file(int client_socket, const char *file_path)
{
	FILE *file = fopen(file_path, "wb");
	char buffer[BUFFER_SIZE];
	ssize_t bytes_read;

	if (file == NULL)
	{
		perror("Erro ao criar arquivo para recebimento");
		close(client_socket);
		exit(EXIT_FAILURE);
	}

	while ((bytes_read = read(client_socket, buffer, sizeof(buffer))) > 0)
		fwrite(buffer, 1, bytes_read, file);

	if (bytes_read == -1)
		perror("Erro ao receber arquivo");
	fclose(file);
	printf("Arquivo recebido com sucesso\n");
}

void	send_file(int client_socket, const char *file_path)
{
	FILE *file = fopen(file_path, "rb");
	char buffer[BUFFER_SIZE];
	size_t bytes_read;

	if (file == NULL)
	{
		perror("Erro ao abrir arquivo para envio");
		close(client_socket);
		exit(EXIT_FAILURE);
	}

	while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
	{
		if (write(client_socket, buffer, bytes_read) == -1)
		{
			perror("Erro ao enviar arquivo");
			fclose(file);
			close(client_socket);
			exit(EXIT_FAILURE);
		}
	}

	fclose(file);
	printf("Arquivo enviado com sucesso\n");
}

void	send_request(t_client *client, const char *data)
{
	struct sockaddr_in server_addr;
	char	request[BUFFER_SIZE * 2];
	char	buffer[BUFFER_SIZE];
	int		bytes_read;

	client->client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client->client_socket == -1)
	{
		perror("Erro ao criar socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTP);
	server_addr.sin_addr.s_addr = inet_addr(client->ip);

	if (connect(client->client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("Erro ao conectar");
		close(client->client_socket);
		exit(EXIT_FAILURE);
	}

	if (strcmp(client->method, "POST") == 0 || strcmp(client->method, "PUT") == 0 || strcmp(client->method, "CMD") == 0)
	{
		snprintf(request, sizeof(request), "%s %s HTTP/1.1\r\n"
			"Host: %s\r\n"
			"Content-Length: %lu\r\n"
			"\r\n"
			"%s", client->method, client->path, client->ip, strlen(data), data);
	}
	else 
	{
		snprintf(request, sizeof(request), "%s %s HTTP/1.1\r\n"
			"Host: %s\r\n"
			"\r\n", client->method, client->path, client->ip);
	}

	write(client->client_socket, request, strlen(request));
	while ((bytes_read = read(client->client_socket, buffer, BUFFER_SIZE - 1)) > 0)
	{
		buffer[bytes_read] = '\0';
		printf("%s", buffer);
	}
	close(client->client_socket);
}


void	apply_each_client(void (*func)(int))
{
	int	i;

	i = 0;
	while (i < client_exists)
	{
		if (client[i] != 0)
			func(client[i]);
		i++;
	}
}

void	add_client(int cl)
{
	int	i;
	int	is_present;

	i = 0;
	is_present = 0;
	while (i < BUFFER_CLI)
	{
		if (client[i] == cl)
		{
			is_present = 1;
			break ;
		}
		if (client[i] == 0)
			break ;
		i++;
	}
	if (!is_present)
	{
		client[i] = cl;
		client_exists++;
	}
}

void	serve_directory(int client_socket, const char *directory, const char *request_path)
{
	char filepath[BUFFER_SIZE];
	char buffer[BUFFER_SIZE];
	int file_fd;
	ssize_t bytes_read;

	snprintf(filepath, sizeof(filepath), "%s%s", directory, request_path);

	struct stat file_stat;
	if (stat(filepath, &file_stat) == -1 || !S_ISREG(file_stat.st_mode))
	{
		const char *not_found_response = "HTTP/1.1 404 Not Found\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"<html><body><h1>404 Not Found</h1></body></html>";
		write(client_socket, not_found_response, strlen(not_found_response));
		return;
	}

	file_fd = open(filepath, O_RDONLY);
	if (file_fd == -1)
	{
		const char *internal_error_response = "HTTP/1.1 500 Internal Server Error\r\n"
		"Content-Type: text/html\r\n"
		"\r\n"
		"<html><body><h1>500 Internal Server Error</h1></body></html>";
		write(client_socket, internal_error_response, strlen(internal_error_response));
		return;
	}

	const char *ok_response = "HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
		"\r\n";
	write(client_socket, ok_response, strlen(ok_response));

	while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0)
		write(client_socket, buffer, bytes_read);
	close(file_fd);
}

