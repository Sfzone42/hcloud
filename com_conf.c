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

