
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK_SIZE 512

//C program that makes a get request to example.com
//Response is sent in chunks recv returns before full response arrives 

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
	recv(socket_desc, server_reply , 6000 , 0);

	puts("Reply received\n");

	puts(server_reply);

    return 0;
}
