Aqui está um pequeno exemplo para testar o servidor e o cliente utilizando as funções que desenvolvemos. Esse exemplo inclui uma função `serve_directory` no servidor e uma função `http_send` no cliente para solicitar um arquivo específico do servidor.

### Arquivos:

1. **`server.c`**: Implementa o servidor.
2. **`client.c`**: Implementa o cliente.
3. **`Makefile`**: Para compilar ambos os programas.

### `server.c`

```c
#include "hcloud.h"

void start_server()
{
    t_server server;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket;

    create_server(&server, "127.0.0.1");
    printf("Servidor iniciado em 127.0.0.1 na porta %d\n", PORTP);

    while (1)
    {
        client_socket = accept(server.server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1)
        {
            perror("Erro ao aceitar conexão");
            continue;
        }

        char request[BUFFER_SIZE];
        int received = recv(client_socket, request, BUFFER_SIZE, 0);
        if (received > 0)
        {
            request[received] = '\0';
            http_request(client_socket, request);
        }
        else
        {
            close(client_socket);
        }
    }
}

int main()
{
    start_server();
    return 0;
}
```

### `client.c`

```c
#include "hcloud.h"

int main()
{
    t_client client;
    configure_client(&client, "127.0.0.1");

    const char *request = "GET /index.html HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";
    http_send(&client, request);

    return 0;
}
```

### `Makefile`

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I.
LIBS = -lhcloud -lreadline

SERVER_OBJS = server.o
CLIENT_OBJS = client.o

all: server client

server: $(SERVER_OBJS)
	$(CC) -o server $(SERVER_OBJS) $(INCLUDES) $(LIBS)

client: $(CLIENT_OBJS)
	$(CC) -o client $(CLIENT_OBJS) $(INCLUDES) $(LIBS)

clean:
	rm -f server client $(SERVER_OBJS) $(CLIENT_OBJS)
```

### Estrutura de Diretórios

Certifique-se de que a estrutura de diretórios seja a seguinte:

```
/projeto
|-- www
|   |-- index.html
|-- server.c
|-- client.c
|-- Makefile
|-- libhcloud.a (ou os arquivos necessários para compilar a lib)
|-- hcloud.h
```

### Compilação e Execução:

1. **Compilar**:
   ```bash
   make
   ```

2. **Executar o Servidor**:
   ```bash
   ./server
   ```

3. **Executar o Cliente**:
   ```bash
   ./client
   ```

### Teste:

- Coloque um arquivo `index.html` no diretório `www`.
- Quando você executar o cliente, ele enviará uma solicitação GET para o servidor.
- O servidor responderá com o conteúdo de `index.html`.

### Explicação:

- **Servidor (`server.c`)**:
  - Escuta conexões na porta definida e aceita conexões dos clientes.
  - Lê a requisição, processa e retorna o conteúdo do arquivo solicitado se existir.
  - Fecha a conexão após enviar a resposta.

- **Cliente (`client.c`)**:
  - Conecta ao servidor e envia uma solicitação HTTP GET para o arquivo `index.html`.
  - Recebe e imprime a resposta do servidor.

- **Makefile**:
  - Compila tanto o servidor quanto o cliente.
  - Você pode adicionar mais funcionalidades à medida que o projeto evolui.
