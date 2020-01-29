#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

char *my_itoa(int num, char *str) //перевод числа в строку
{
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}

// функция для общения клиента и сервера 
void func(int sockfd) 
{ 
	char buff[MAX]; 
	for (;;) //бесконечный цикл для чата
	{ 
		bzero(buff, MAX); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From client: %s\n", buff);
		system(buff);
		if ((strncmp("exit", buff, 4) == 0)) //выход из чата
		{ 
			printf("Server Exit...\n"); 
			write(sockfd, buff, sizeof(buff));
			break; 
		}
		else
		{ 		
		FILE* cmd=popen(buff, "r");
		while(fgets(buff, sizeof(buff), cmd))
		{
		write(sockfd, buff, strlen(buff));
		} 		 			 
		int ret=pclose(cmd);
		bzero(buff,MAX);
		my_itoa(ret, buff);
		write(sockfd, buff, strlen(buff));
		}		      
	} 
} 

int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; //sockaddr_in - {in - домен для интернета}
	sockfd = socket(AF_INET, SOCK_STREAM, 0); //Дескриптор сокета (Домен AF_INET, тип SOCK_STREAM {TCP}, протокол)
	if (sockfd == -1) 
	{ 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); //обнулить структуру сокета, сделав 0.0.0.0
	servaddr.sin_family = AF_INET; //установка семейства адресов интернет
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //htonl - host to network long {преобразование числа из порядка хоста в сетевой} INADDR_ANY {любой интерфейс}
	servaddr.sin_port = htons(PORT); //htonl - host to network short {преобразование числа из порядка хоста в сетевой}
	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) //Связывание сокета sockfd с адресом servaddr размером структуры servaddr
	{  
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); //перевод сокета в режим ожидания, где 1 - дескриптор сокета, а 2 - размер очереди
	if ((listen(sockfd, 5)) != 0) 
	{ 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 
	connfd = accept(sockfd, (SA*)&cli, &len); // создание сокета общения с клиентом и возврат дескриптора, где 1 - декриптор сокета, 2- адрес сокета клиента, 3- размер
	if (connfd < 0) 
	{ 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 
	func(connfd); 
	close(sockfd); 
} 

