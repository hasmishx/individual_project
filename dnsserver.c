#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h> 	//inet_addr
#include<unistd.h>	//write
#include<netdb.h>	//hostent
#include<arpa/inet.h>

int main(int argc , char *argv[])
{
	int socket_desc , new_socket , c, i;
	struct sockaddr_in server , client;
	char client_domain[1000] , server_ip[1000];
	struct hostent *he;
    struct in_addr **addr_list;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		//receive domain name from client
		//recv(new_socket,client_domain,strlen(client_domain));
		
		if(recv(new_socket,client_domain,sizeof(client_domain),0) < 0)
		{
			puts("recv failed");
		}
		puts("Reply received\n");


        if ( (he = gethostbyname(client_domain) ) == NULL)
        {
                //gethostbyname failed
                herror("gethostbyname");
                return 1;
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;

        for(i = 0; addr_list[i] != NULL; i++)
        {
                //Return the first one;
                strcpy(server_ip , inet_ntoa(*addr_list[i]) );
        }
		//Reply to the client
		write(new_socket , server_ip , strlen(server_ip));



	}
	
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}
