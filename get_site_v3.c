#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define CHUNK_SIZE 512

//C program that makes a get request to example.com
//Response is received but recv hangs because it doesn't know when end of message has arrived

int recv_timeout(int socket_desc, int timeout)
{
	int size_recv , total_size= 0;
    struct timeval begin, now;
	char chunk[CHUNK_SIZE];
    double timediff;

    //make socket non blocking
    fcntl(socket_desc, F_SETFL, O_NONBLOCK);

    //begin time
    gettimeofday(&begin, NULL);

	//loop
	while(1)
	{
        gettimeofday(&now, NULL);

        //time elapsed in seconds
        timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
		
        //if got data break after timeout
        if(total_size > 0 && timediff > timeout)
        {
            break;
        }
        //if got no data, wait more
        else if (timediff > timeout * 2)
        {
            break;
        }




        memset(chunk ,0 , CHUNK_SIZE);	//clear the variable
        if((size_recv = recv(socket_desc , chunk , CHUNK_SIZE , 0)) < 0)
        {
            usleep(100000);
        }
        else
        {
            total_size += size_recv;
            printf("\n-----CHUNK INFO-----\n");
            printf("bytes received: %d\n", size_recv);
            printf("\n-----START OF CHUNK-----\n");
            printf("%s" , chunk);
            printf("\n-----END OF CHUNK-----\n");
            gettimeofday(&begin, NULL);
        }
    }

    return total_size;
}

int main(void)
{
    int socket_desc, total_recv;
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

    total_recv = recv_timeout(socket_desc, 1);

    puts("Reply received\n");

    puts(server_reply);

    return 0;
}
