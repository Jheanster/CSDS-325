/*
Author: Jhean C Toledo
CaseID: jct95
Filename: proj3.c
Date created: 10/15/22
Description: This code is to represent a simple web server.
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

bool pPresent = false;
bool rPresent = false;
bool tPresent = false;
bool pValid = false;
bool requestValid = false;

int invalidCMDS = 0;
int portNumber;
char *authToken;
char *directory;
char *portString;
char *pieces[4];  

#define REQUIRED_ARGC 3
#define PORT_POS 1
#define MSG_POS 2
#define ERROR 1
#define QLEN 1
#define PROTOCOL "tcp"
#define BUFLEN 1024
#define midBUF 256
#define smallBUF 16


// parseRequest method: Takes in a string and parses the string into individual pieces. Used for the first line in the request.
void parseRequest(char *string){
    requestValid = false;
    // Check to see if the request is valid
    char* piece = malloc(smallBUF);
    piece = strtok(string," ");
	
    int i = 0;
    // Loop through the request and break it into pieces. Store individual pieces into an array called "pieces"
    while(piece != NULL){
	pieces[i] = piece;
	piece = strtok(NULL, " ");
	i++;
    }  

    // Check if the line ends with a "\r\n"
    if(i == 3 && pieces[2][strlen(pieces[2]) - 2] == '\r' && pieces[2][strlen(pieces[2]) - 1] == '\n'){
        requestValid = true;
    }
    
}

//errexit method: Taken from class webpage
int errexit (char *format, char *arg)
{
    fprintf (stderr,format,arg);
    fprintf (stderr,"\n");
    exit (ERROR);
}

// listenMethod: Takes in an int "sd" that represents which socket to listen to. Used recursively to keep listening on that socket.
void listenMethod(int sd){
    struct sockaddr addr;
    unsigned int addrlen;
    int sd2;
    char temp[midBUF];
    char method[smallBUF];
    char argument[midBUF];
    char version[smallBUF];
    char *buffer = malloc(BUFLEN);

    /* listen for incoming connections */
    if (listen (sd, QLEN) < 0)
        errexit ("cannot listen on port %s\n", portString);

    /* accept a connection */
    addrlen = sizeof (addr);
    sd2 = accept (sd,&addr,&addrlen);
    if (sd2 < 0)
        errexit ("error accepting connection", NULL);
    

    // Get whatever is requested
    FILE *file = fdopen(sd2,"rb");
    fgets(buffer,BUFLEN,file);

   // printf("Request: %s",buffer);
    parseRequest(buffer);
    strcpy(method,pieces[0]);
    strcpy(argument,pieces[1]);
    strcpy(version,pieces[2]);

    //  printf("METHOD: %s\n", method);
    //  printf("ARGUMENT: %s\n", argument);
    //  printf("HTTP/VERISON: %s\n", path);

    // Check if the request is good
    if(!requestValid){
	char* str = "HTTP/1.1 400 Malformed Request\r\n\r\n";
	if(write (sd2,str,strlen (str)) < 0)
		errexit ("error writing message: %s", str);
	close(sd2);
	listenMethod(sd);
    } else {

        buffer[0] = '\0';
        // Check the rest of the request to see if it ends with \r\n
         while(strcmp(buffer,"\r\n")){
		fgets(buffer,BUFLEN,file);
		if(buffer[strlen(buffer) - 2] != '\r' || buffer[strlen(buffer) - 1] != '\n'){
			char* str = "HTTP/1.1 400 Malformed Request\r\n\r\n";
                	if(write (sd2,str,strlen (str)) < 0)
                    		errexit ("error writing message: %s", str);
            	close(sd2);
            	listenMethod(sd);
           	}  
        }

        // printf("Method now:%s\n",method);
        // printf("Argument now:%s\n",argument);
        // printf("Path now:%s",path);


        // Check if its the right protocol, case sensitive
        if(strncmp(version,"HTTP/",5 )){
		char* str = "HTTP/1.1 501 Protocol Not Implemented\r\n\r\n";
		if(write (sd2,str,strlen(str)) < 0)
                	errexit ("error writing message: %s",str);
            	close(sd2);
            	listenMethod(sd);
        } 

        // Check if the method is supported
        if(strcmp(method,"GET") && strcmp(method,"TERMINATE")){
            	char* str = "HTTP/1.1 405 Unsupported Method\r\n\r\n";
            	if(write (sd2,str,strlen(str)) < 0)
                	errexit("error writing message %s",str);
            	close(sd2);
            	listenMethod(sd);
        } 

        // Check if the TERMINATE method was used
        if(!strcmp(method,"TERMINATE")){ 

            // Then check if the correct authToken was passed in argument
            	if(!strcmp(argument,authToken)){
                	char* str = "HTTP/1.1 200 Server Shutting Down\r\n\r\n";
                	if(write (sd2,str,strlen(str)) < 0)
                    		errexit("error writing message: %s",str);
                	close (sd);
                	close (sd2);
                	exit (0);
            	} else {
                	char* str = "HTTP/1.1 403 Operation Forbidden\r\n\r\n";
                	if(write (sd2,str,strlen(str)) < 0)
                    		errexit("error writing message: %s",str);
                	close(sd2);
                	listenMethod(sd);
            	}
        } 

        // Check if the GET method was used
        if(!strcmp(method,"GET")){

            // Then check if no "/" was given
           	 if(strncmp(argument,"/",1)){
               		char* str = "HTTP/1.1 406 Invalid Filename\r\n\r\n";
               	 if(write (sd2,str,strlen(str)) < 0 )
                 	errexit("error writing message: %s",str);
                close(sd2);
                listenMethod(sd);
        } else {            
		// Set file to /homepage.html if only / was only given
		if (!strcmp(argument,"/")) 
			strcpy(argument,"/homepage.html");
		// printf("Argument after default: %s\n",argument);
             	strcpy(temp,directory);
              	strcat(temp,argument);
              	//printf("Temp: %s\n",temp);

                    // File exists
               	if(access(temp,F_OK) == 0){
			FILE *fp = fopen(temp,"rd");
                        temp[0]  = '\0';
                        char* str = "HTTP/1.1 200 OK\r\n\r\n";
                        if(write (sd2,str,strlen(str)) < 0)
                        	errexit("error writing message: %s",str);
 
                        // Write whatever was in the file to the socket.
                        int size = fread(buffer,1, sizeof(buffer), fp);
		                while(size != 0){
			                if(write (sd2,buffer,size) < 0)
                                		errexit("error writing message: %s",buffer);
			                memset (buffer,0,BUFLEN);
			                size = fread(buffer,1, sizeof(buffer), fp);
		                }
                        memset(buffer,0x0,size);
                        fclose(fp);
                        close(sd2);
                        listenMethod(sd);

                     //File doesn't exist   
               	} else {
               		char* str = "HTTP/1.1 404 File Not Found\r\n\r\n";
                        if(write(sd2,str,strlen(str)) < 0)
                       		errexit("error writing message: %s",str);
                        close(sd2);
                        listenMethod(sd);
            	}
           } 
        }
    }
    close(sd2);
    
}
 

// createSocket method: Taken from class webpage. Creates a socket and binds to a port, then listens on that socket.
void createSocket(){
    struct sockaddr_in sin;
    struct protoent *protoinfo;
    int sd;
    
    /* determine protocol */
    if ((protoinfo = getprotobyname (PROTOCOL)) == NULL)
    	errexit ("cannot find protocol information for %s", PROTOCOL);

    /* setup endpoint info */
    memset ((char *)&sin,0x0,sizeof (sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons ((u_short) portNumber);

    /* allocate a socket */
    /*   would be SOCK_DGRAM for UDP */
    sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    if (sd < 0)
    	errexit("cannot create socket", NULL);

    /* bind the socket */
    if (bind (sd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    	errexit ("cannot bind to port %s", portString);

    listenMethod(sd);

    close(sd);
    exit(0);
}


int main(int argc, char* argv[]){
    if(argc == 1){
        	// Not enough args
		printf("Not enough arguments have been passed. Must include -p [port number] -r [document directory] and -t [auth token]\n");
	}

	//Valid number of args
    for(int i = 0; i< argc; i++){
    	if(!strcmp(argv[i],"-p")){
            //Check if -u is in the arg
            portString = argv[i+1];
            portNumber = atoi(argv[i+1]);
            pPresent = true;
        } else if(!strcmp(argv[i],"-r")){
        // Check if -i is in the arg
            directory = argv[i+1];
            rPresent = true;
        } else if(!strcmp(argv[i],"-t")){
            //Check if -o is in the arg
            tPresent = true;
            authToken = argv[i+1];
        } else {
            invalidCMDS++;
        }
		
	}

    if(invalidCMDS > 4){
			printf("Unrecognized command. Valid commands are -p [port number], -r [directory], and -t [auth token]\n");
			printf("Number of unrecognized commands: %d\n",invalidCMDS - 4);
	}

    if(!tPresent)
        printf("-t [auth token] command must be present.\n");
    if(!pPresent)
        printf("-p [port number] command must be present.\n");
    if(!rPresent)
        printf("-r [directory] command must be present.\n");

    // Validate the port number
    int min = 1025;
    int max = 65535;

    if((portNumber - min)*(portNumber - max) <= 0){
        pValid = true;
    }else{
        printf("Port number is not in the valid range. Please put a port number between [1025,65535]\n");
    }

    //  printf("Auth thingy: %s\n",authToken);
    //  printf("Directory: %s\n",directory);
    //  printf("PortNUmber: %d\n",portNumber);
   
    // Create socket
    if (pPresent && pValid && rPresent && tPresent && (invalidCMDS == 4) ){
        createSocket();
    } 

}
