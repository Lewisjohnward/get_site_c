#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK_SIZE 512

//C program that makes a get request to example.com
//Response is received but recv hangs because it doesn't know when end of message has arrived

int receive_basic(int socket_desc)
{
	int size_recv , total_size= 0;
	char chunk[CHUNK_SIZE];

	//loop
	while(1)
	{
		memset(chunk ,0 , CHUNK_SIZE);	//clear the variable
		if((size_recv = recv(socket_desc , chunk , CHUNK_SIZE , 0)) == 0)
		{
			break;
		}
		else
		{
			total_size += size_recv;
			printf("\n-----CHUNK INFO-----\n");
            printf("bytes received: %d\n", size_recv);
			printf("\n-----START OF CHUNK-----\n");
			printf("%s" , chunk);
			printf("\n-----END OF CHUNK-----\n");
		}
	}

	return total_size;
}

int main(void)
{
    int socket_desc;
    struct sockaddr_in server;
    char *message, server_reply[5000];

    //create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    //add error checking

    //IP of www.msn.com done by pinging
    server.sin_addr.s_addr = inet_addr("93.184.216.34");
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    //Connect to remote server
    connect(socket_desc, (struct sockaddr *)&server, sizeof(server));


    puts("Connected\n");

    //Send data
    message = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
    printf("message: %s\n", message);

    send(socket_desc, message, strlen(message), 0);
    //Error checking

    puts("Data sent\n");

    //Receive a reply from the server
	//recv(socket_desc, server_reply , 6000 , 0);

    receive_basic(socket_desc);

	puts("Reply received\n");

	puts(server_reply);

    return 0;
}
