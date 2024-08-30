/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ser_conf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhabacuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:36:09 by lhabacuc          #+#    #+#             */
/*   Updated: 2024/08/30 15:36:11 by lhabacuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hcloud.h"

void	ceate_server_or(t_server *server)
{
	struct sockaddr_in server_addr;

	server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server->server_socket == -1)
	{
		perror("Erro ao criar socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTP);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server->server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("Erro ao vincular socket");
		close(server->server_socket);
		exit(EXIT_FAILURE);
	}

	if (listen(server->server_socket, 5) == -1)
	{
		perror("Erro ao ouvir");
		close(server->server_socket);
		exit(EXIT_FAILURE);
	}
}

void	ceate_server(t_server *server, const char *ip)
{
	struct sockaddr_in server_addr;

	server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server->server_socket == -1)
	{
		perror("Erro ao criar socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTP);
	if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
	{
		perror("Endereço IP inválido");
		close(server->server_socket);
		exit(EXIT_FAILURE);
	}

	if (bind(server->server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("Erro ao vincular socket");
		close(server->server_socket);
		exit(EXIT_FAILURE);
	}

	if (listen(server->server_socket, 5) == -1)
	{
		perror("Erro ao ouvir");
		close(server->server_socket);
		exit(EXIT_FAILURE);
	}
}

int	acess_client(t_server *server)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_socket;

	client_socket = accept(server->server_socket, (struct sockaddr *)&client_addr, &client_len);
	if (client_socket == -1)
	{
		perror("Erro ao aceitar conexão");
		close(server->server_socket);
		exit(EXIT_FAILURE);
	}

	printf("Cliente conectado: %s\n", inet_ntoa(client_addr.sin_addr));
	return (client_socket);
}

void	close_server(t_server *server)
{
	if (close(server->server_socket) == -1)
	{
		perror("Erro ao fechar socket");
		exit(EXIT_FAILURE);
	}
	printf("Socket do servidor fechado com sucesso\n");
}

/* func exmple : */
void	process_message(int client_socket, const char *username, const char *message)
{
	FILE *fp = fopen("messages.txt", "a");
	if (fp == NULL)
	{
		perror("Erro ao abrir arquivo de mensagens");
		send_data(client_socket, "500 Internal Server Error\n");
		return ;
	}

	fprintf(fp, "%s: %s\n", username, message);
	fclose(fp);
	send_data(client_socket, "200 OK: Mensagem recebida\n");
}

void	process_request(int client_socket, const char *request)
{
	char method[10];
	char path[BUFFER_SIZE];
	char data[BUFFER_SIZE];
	int ret;

	ret = sscanf(request, "%9[^:]:%1023[^:]:%1023[^\n]", method, path, data);
	if (ret < 2)
	{
		printf("Formato de requisição inválido\n");
		send_data(client_socket, "400 Bad Request\n");
		return;
	}

	if (strcmp(method, "GET") == 0)
		send_data(client_socket, "200 OK: GET request received\n");
	else if (strcmp(method, "POST") == 0)
		send_data(client_socket, "200 OK: POST request received\n");
	else if (strcmp(method, "DWN") == 0)
		send_data(client_socket, "200 OK: Download request received\n");
	else if (strcmp(method, "UPT") == 0)
	{
		FILE *fp = fopen(path, "wb");
		if (fp != NULL)
		{
		fwrite(data, 1, strlen(data), fp);
		fclose(fp);
		send_data(client_socket, "200 OK: File uploaded\n");
		}
		else
			send_data(client_socket, "500 Internal Server Error\n");
	}
	else
		send_data(client_socket, "405 Method Not Allowed\n");
}

