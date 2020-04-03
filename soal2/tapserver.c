//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux 
#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
	
#define TRUE 1 
#define FALSE 0 
#define PORT 8080

char p1[512] = "", p2[512] = "";

char* auth(char* str){
    char* token = strtok(str, "\n");
    char username[512], password[512], method[512];


    // init user pass
    strcpy(method, token);
    token = strtok(NULL, "\n");
    strcpy(username, token);
    token = strtok(NULL, "\n");
    strcpy(password, token);

    printf("New Login : \nUsername : %s\nPassword : %s\n", username, password );

    char buff[1024];
    int status = 0;
    char upass[1024] = "";

    strcat(upass, username);
    strcat(upass, "\n");
    strcat(upass, password);
    strcat(upass, "\n");

    printf("%s\n", method);

    if(strcmp(method, "login") == 0){
        // login, cek file
        FILE* filePtr = NULL;
        filePtr = fopen("akun.txt", "r");
        fread(buff, 1024, 1, filePtr);
        if(strstr(buff, upass) != NULL){
            status = 1;
        }
        fclose(filePtr);
    }else if(strcmp(method, "daftar") == 0){
        // daftar, tambahkan file
        FILE* filePtr = NULL;
        filePtr = fopen("akun.txt", "a");
        fwrite(upass, strlen(upass), 1, filePtr);
        status = 1;
        fclose(filePtr);
    }

    if(status == 1){
        printf("Sukses!\n");
        return "1";
    }else{
        printf("Gagal...\n");
        return "0";
    }
} 

int main(int argc , char *argv[]) 
{ 
	int opt = TRUE; 
	int master_socket , addrlen , new_socket , client_socket[30] , 
		max_clients = 30 , activity, i , valread , sd; 
	int max_sd; 
	struct sockaddr_in address; 
		
	char buffer[1025]; //data buffer of 1K 
		
	//set of socket descriptors 
	fd_set readfds; 
		
	
	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < max_clients; i++) 
	{ 
		client_socket[i] = 0; 
	} 
		
	//create a master socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	//set master socket to allow multiple connections , 
	//this is just a good habit, it will work without this 
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	
	//type of socket created 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
		
	//bind the socket to localhost port 8888 
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Listener on port %d \n", PORT); 
		
	//try to specify maximum of 3 pending connections for the master socket 
	if (listen(master_socket, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
		
	//accept the incoming connection 
	addrlen = sizeof(address); 
	puts("Waiting for connections ..."); 
		
	while(TRUE) 
	{ 
		//clear the socket set 
		FD_ZERO(&readfds); 
	
		//add master socket to set 
		FD_SET(master_socket, &readfds); 
		max_sd = master_socket; 
			
		//add child sockets to set 
		for ( i = 0 ; i < max_clients ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &readfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
	
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL); 
	
		if ((activity < 0) && (errno!=EINTR)) 
		{ 
			printf("select error"); 
		} 
			
		//If something happened on the master socket , 
		//then its an incoming connection 
		if (FD_ISSET(master_socket, &readfds)) 
		{ 
			if ((new_socket = accept(master_socket, 
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			
			//inform user of socket number - used in send and receive commands 
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs 
				(address.sin_port)); 
				
			puts("Welcome message sent successfully"); 
				
			//add new socket to array of sockets 
			for (i = 0; i < max_clients; i++) 
			{ 
				//if position is empty 
				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 
					printf("Adding to list of sockets as %d\n" , i); 
						
					break; 
				} 
			} 
		} 
			
		//else its some IO operation on some other socket 
		for (i = 0; i < max_clients; i++) 
		{ 
			sd = client_socket[i]; 
				
			if (FD_ISSET( sd , &readfds)) 
			{ 
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((valread = read( sd , buffer, 1024)) == 0) 
				{ 
					//Somebody disconnected , get his details and print 
					getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen); 
					printf("Host disconnected , ip %s , port %d \n" , 
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
						
					//Close the socket and mark as 0 in list for reuse 
					close( sd ); 
					client_socket[i] = 0; 
				} 
					
				//Echo back the message that came in 
				else
				{ 
					// Release the octOwOpus 
					buffer[valread] = '\0';
					
			        printf("Request : %s\n", buffer);
					char status_login[100];

					// PseudoAPI
					char raw_input[1024]; strcpy(raw_input, buffer);
					char response[1024];
					char *method = strtok(buffer, "\n");


					if(strcmp(method, "login") == 0){
						strcpy(response, auth(raw_input));
						send(sd , response , strlen(response) , 0 );
					}else if(strcmp(method, "daftar") == 0){
						strcpy(response, auth(raw_input));
						send(sd , response , strlen(response) , 0 );
					}else if(strcmp(method, "ready") == 0){
						// Posisikan player dalam kondisi ready 
						char 	res_ready[1024], 
								stat_true[] = "1", 
								stat_false[] = "0";
						
						char username[512], method[512];
						char* token = strtok(raw_input, "\n");
					    strcpy(method, token);
					    token = strtok(NULL, "\n");
					    strcpy(username, token);
					    printf("%s\n", token);
					    if(strcmp(p1, "") == 0){
					    	strcpy(p1, username);
					    }else{
					    	strcpy(p2, username);
					    }

						printf("P1 : %s; P2 : %s\n", p1, p2);
						if(strcmp(p1, "") != 0 && strcmp(p2, "") != 0){
					    	strcpy(res_ready, "1\0");
						}else{
							strcpy(res_ready, "0\0");
						}
						res_ready[2] = '\0';
						sleep(1);
						send(sd , res_ready , strlen(res_ready) , 0 );
					}else if(strcmp(method, "wait") == 0){
						if(strcmp(p1, "") != 0 && strcmp(p2, "") != 0){
					    	strcpy(response, "1\0");
						}else{
							strcpy(response, "0\0");
						}

						response[2] = '\0';

						send(sd , response , strlen(response) , 0 );
					}else if(strcmp(method, "attack") == 0){
						char raw_input[1024]; strcpy(raw_input, buffer);
						char method[512], username[512];
					    char* token = strtok(NULL, "\n");
					    strcpy(username, token);
					    printf("di strcpy\n");

					    if(strcmp(p1, username) == 0){
					    	printf("p1 attack p2\n");
					    	strcpy(response, p2);

					    }else{
					    	printf("p2 attack p1\n");
					    	strcpy(response, p1);
					    }

					    strcat(response, "\n");

					    for (int iter = 0; iter < max_clients; ++iter)
					    {
							send(client_socket[iter] , response , strlen(response) , 0 );
					    }
					}else if(strcmp(method, "lose") == 0){
						strcpy(p1, "");
						strcpy(p2, "");
						printf("Game selesai!\n");
						char winning_flag[] = "aerosolismic_pwd_flag_for_winning\n";
						strcpy(response, winning_flag);

					    for (int iter = 0; iter < max_clients; ++iter)
					    {
							send(client_socket[iter] , response , strlen(response) , 0 );
					    }
					}
				} 
			} 
		} 
	} 
		
	return 0; 
} 
