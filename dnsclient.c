#include<stdio.h>
#include<string.h>      //strlen
#include<sys/socket.h>
#include<arpa/inet.h>   //inet_addr
#include<unistd.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	int socket_desc;
	struct sockaddr_in server;
	char client_domain[1000], server_ip[1000];
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		puts("Could not create socket");
	}
		
	server.sin_addr.s_addr = inet_addr("192.168.56.103");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	puts("Connected to server :) \n");

	//Ask client for domain name
	
	puts("DNS IP ADDRESS FINDER \n");
	puts("Enter domain name (example: google.com) :  ");
    scanf("%s",client_domain);
	
	
	//Send data to server
	if( send(socket_desc , client_domain , strlen(client_domain) , 0) < 0)
	{
		puts("Failed to send data :( \n ");
		return 1;
	}
	puts("Data Sent to DNS server\n");
    
	//Receive a reply from server
	if(recv(socket_desc, server_ip , 2000 , 0) < 0)
	{
		puts("Failed to receive data from DNS server \n");
	}
	puts("IP address: ");
	puts(server_ip);

	return 0;
}
