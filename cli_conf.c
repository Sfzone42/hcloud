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

void	process_command(t_client *client, const char *command)
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


