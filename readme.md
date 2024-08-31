
# HCLOUD

HCLOUD é uma biblioteca em C para facilitar a criação de um cliente e servidor de rede básico, com suporte para operações de envio e recebimento de arquivos, processamento de comandos e mensagens, e gerenciamento de conexões de clientes.

## Recursos

- **Cliente**: Configuração e conexão com um servidor, envio de comandos e arquivos, recebimento de dados e arquivos.
- **Servidor**: Criação e configuração do servidor, processamento de requisições, envio e recebimento de arquivos e mensagens, gerenciamento de clientes.
- **Comum**: Funções utilitárias para manipulação de arquivos e dados.

## Estrutura do Projeto

- **`cli_conf.c`**: Implementações das funções do cliente.
- **`com_conf.c`**: Implementações das funções comuns e gerenciamento de clientes.
- **`ser_conf.c`**: Implementações das funções do servidor.
- **`hcloud.h`**: Cabeçalhos da biblioteca.
- **`Makefile`**: Arquivo para compilar a biblioteca.

## Instalação

1. **Clone o repositório:**

   ```sh
   git clone https://github.com/lhabacuc/hcloud.git
   cd hcloud
   ```

2. **Compile a biblioteca:**

   ```sh
   make
   ```

   Isso irá gerar o arquivo `libhcloud.a` na pasta do projeto.

## Uso

Para usar a biblioteca, inclua o cabeçalho `hcloud.h` e link a biblioteca estática `libhcloud.a` ao compilar seu programa.

### Exemplo de Código

#### Cliente

```c
#include "hcloud.h"

int main(void)
{
    t_client client;
    configure_client(&client, "127.0.0.1");

    // Envie um comando para o servidor
    process_command(&client, "GET /index.html");

    return 0;
}
```

#### Servidor

```c
#include "hcloud.h"

int main(void)
{
    t_server server;
    ceate_server(&server, "127.0.0.1");

    // Processa requisições de clientes
    while (1)
    {
        int client_socket = accept(server.server_socket, NULL, NULL);
        if (client_socket >= 0)
        {
            process_request(client_socket, "request");
            close(client_socket);
        }
    }

    return 0;
}
```

## Funções Principais

### Cliente

- **`void configure_client(t_client *client, const char *server_ip);`**  
  Configura o cliente com o IP do servidor.

- **`void process_command(t_client *client, const char *command);`**  
  Processa um comando e envia para o servidor.

### Servidor

- **`void ceate_server(t_server *server, const char *ip);`**  
  Cria e configura o servidor com o IP especificado.

- **`void process_request(int client_socket, const char *request);`**  
  Processa uma requisição recebida do cliente.

## Contribuição

Contribuições são bem-vindas! Para contribuir, por favor, siga as etapas abaixo:

1. Faça um fork do repositório.
2. Crie uma branch para sua feature (`git checkout -b feature-nome`).
3. Faça commit das suas mudanças (`git commit -am 'Add new feature'`).
4. Envie a branch para o repositório (`git push origin feature-nome`).
5. Crie um Pull Request.

## Licença

Distribuído sob a Licença MIT. Veja `LICENSE` para mais informações.

## Contato

Seu Nome - [liedsonnhabacuc@gmail.com](em:lisvaldosf@gmail.com)

Link do Repositório: [https://github.com/lhabacuc/hcloud](https://github.com/lhabacuc/hcloud)
```
    -I. -L. -lhcloud -lreadline
