#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#define PORT 8089

int main(int argc, char const* argv[])
{
	srand(time(NULL));
    double data = 0;
	char str_data[10];

	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// criando socket
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// anexando a porta no socket
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	// família TCP
	address.sin_family = AF_INET;
	// conectar o socket em todos os IPs disponíveis.
	address.sin_addr.s_addr = INADDR_ANY;
	// porta a ser usado pelo TCP
	address.sin_port = htons(PORT);

	// conectar ao cliente
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
    {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// transforma o socket ativo em socket passivo que será usado para a comunicação
	if (listen(server_fd, 3) < 0) 
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// aceitar a conecção com novo cliente
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) 
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

    while (1) 
	{
		// gerando dados para serem transmitidos
		if((rand() % 5) < 2)
		{
			data = - rand() % 2000;
		}
        else
		{
			data = rand() % 2000;
		}

		// string com os dados formatada para envio
		sprintf(str_data, "%+09.3f", data);

		// mostrar os dados
        printf("Num = %+09.3f\n", data);
        printf("Str = %s\n",str_data);
        printf("Len = %d\n\n", strlen(str_data));

		// envia os dados caso exista conecção
       	send(new_socket, str_data, strlen(str_data), 0);

		// pause
		sleep(1);
    }

	// encerrando a conecção com o cliente
	close(new_socket);

    // desativando o socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}