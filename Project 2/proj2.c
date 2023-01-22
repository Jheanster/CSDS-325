/*
Author: Jhean C Toledo
CaseID: jct95
Filename: proj2.c
Date created: 9/24/22
Description: This code is to represent a simple web client. It request data from a server and is 
able to download the contents into a specified folder.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define ERROR 1
#define REQUIRED_ARGC 3
#define HOST_POS 1
#define PORT_POS 2
#define PROTOCOL "tcp"
#define BUFLEN 1024

 	bool uPresent = false;  // Keeps track of the u command
    bool iPresent = false;  // Keeps track of the i command
    bool oPresent = false;  // Keeps track of the o command
    bool cPresent = false;  // Keeps track of the c command
    bool sPresent = false;  // Keeps track of the s command
	bool filePresent = false; //Keeps track of a file after the -o command
	bool valid = false;     // Keeps track if the input is valid
	char *url;              // String to store the URL
	char *filename;         // String to store the filename
	char *hostname;         // String to store the host name 
	char *pieces[20];       // String array to store the pieces of the URL
	char *headerpieces[20]; // String array to store the pieces of the header
	char *fullPath;         // String to store the full path of a URL
	int invalidCMDS = 0;    // Keeps track of invalid commands (commands not implemented)



// ParseURL method. Takes in a URL and parses it at every "/" in order to get the different parts of the URL
void parseURL(char *string){
	char* piece = malloc(10);
	piece = strtok(string,"/");
	
	int i = 0;
	// Loop through the URL and break it into pieces. Store individual pieces into an array called "pieces"
	while(piece != NULL){
		pieces[i] = piece;
		i++;
		piece = strtok(NULL, "/");
	} 

	fullPath = malloc(20);
		
	i = 0;
	char *temp = malloc(10);
	// Print out the entire path
	while(pieces[2+i] != NULL){
			
		strcpy(temp, pieces[2+i]);
		if ( i == 0) {
			strcat(temp,"/");
			strcpy(fullPath,temp);
		} else {
			strcat(fullPath, temp);
		}
				
		i++;
	}
}


// Info method. Returns the hostname of the URL, the web filename, and the output filename.
void INFO(){
   

   //Print out the web name
   printf("INF: hostname = %s\n",pieces[1]);

	// Checks to see if there even is a path to print
	if(!(pieces[2] == NULL)){
		printf("INF: web_filename = /%s\n", fullPath);
	} else {
		printf("INF: web_filename = /\n");
	}

	// Print out the name of the file you wanna save to
	printf("INF: output_filename = %s\n",filename);
}

// Error method provided on the class website
int errexit (char *format, char *arg)
{
    fprintf (stderr,format,arg);
    fprintf (stderr,"\n");
    exit (ERROR);
}


// Socket method provided on the class website
void createSocket(char *string)
{
    struct sockaddr_in sin;
    struct hostent *hinfo;
    struct protoent *protoinfo;
    char buffer [BUFLEN];
    int sd, ret;

    /* lookup the hostname */
    hinfo = gethostbyname (string);
    if (hinfo == NULL)
        errexit ("cannot find name: %s", string);

    /* set endpoint information */
    memset ((char *)&sin, 0x0, sizeof (sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons (atoi ("80"));
    memcpy ((char *)&sin.sin_addr,hinfo->h_addr,hinfo->h_length);

    if ((protoinfo = getprotobyname (PROTOCOL)) == NULL)
        errexit ("cannot find protocol information for %s", PROTOCOL);

    /* allocate a socket */
    /*   would be SOCK_DGRAM for UDP */
    sd = socket(PF_INET, SOCK_STREAM, protoinfo->p_proto);
    if (sd < 0)
        errexit("cannot create socket",NULL);

    /* connect the socket */
    if (connect (sd, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		 errexit ("cannot connect", NULL);
	}
       
	// Allocate space for a request
	char request[sizeof(pieces)];
	
	// Format the request and write that request to the socket
	snprintf(request,sizeof(request),"GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: CWRU CSDS 325 Client 1.0\r\n\r\n",fullPath,pieces[1]);
	int checkWrite = write(sd,request,strlen(request));
	if(checkWrite < 0){
		errexit("write failed",NULL);
	}
	

	// Begin reading from the header given from the request
	char *headerString = malloc(BUFLEN + 1);
	FILE *readFile = fdopen(sd,"r");
	char *status = fgets(headerString,BUFLEN,readFile); // Keep track of status which is the first line in the header. Use for -f
	
	char *tmpStatus = malloc(10); // Temporary string variable to hold the status to be used in strtok as strtok messes up the string.
	strcpy(tmpStatus,status);

	char *headerpiece = malloc(10);
	headerpiece = strtok(tmpStatus," ");

	 int j = 0;
	// Loop through the first headerstring and break it into pieces. Store individual pieces into an array called "headerpieces"
	// This allows us to find where the status number is as it is always the 1 index in this array.
	while(headerpiece != NULL){
		headerpieces[j] = headerpiece;
	  	j++;
	  	headerpiece = strtok(NULL, " ");
	} 

	// Read what is in the header.
	while(strcmp(headerString,"\r\n")){
		// Prints out the header if -s was an input
		if( (strcmp(headerString,"")) && sPresent)
			printf("RSP: %s", headerString);
		fgets(headerString,BUFLEN,readFile);
	}

	if(strcmp(headerpieces[1],"200") == 0){

		// Begin writing whatever data was read from the request to the output file. This is for the -o command
		FILE *fp = fopen(filename,"w");
		int size = fread(headerString,1, BUFLEN, readFile);
		while(size > 0){
			fwrite(headerString,size,1,fp);
			memset (headerString,0,BUFLEN);
			size = fread(headerString,1, BUFLEN, readFile);
		}

	}else if(strcmp(headerpieces[1],"301") == 0){
		/* 
		Wouldn't -f just go in here...? So read the header and find the location, parse the URL and go to the new webpage...?
		*/
		printf("-f would be applied here. Not yet implemented");
	} else if(strcmp(headerpieces[1],"200") != 0){
		errexit("ERROR: non-200 response code\n",NULL);
	}

	

    /* snarf whatever server provides and print it */
    memset (buffer,0x0,BUFLEN);
    ret = read (sd,buffer,BUFLEN - 1);
    if (ret < 0)
        errexit ("reading error",NULL);
    fprintf (stdout,"%s\n",buffer);
            
    /* close & exit */
    close (sd);
    exit (0);
}

int main(int argc, char* argv[]){
   

	if(argc == 1){
        // Not enough args
		printf("Not enough arguments have been passed. Must include at least -u [URL] and -o [filename]\n");
	}

	if(argc >= 2){

	    //Valid number of args
		for(int i = 0; i< argc; i++){
			if(!strcmp(argv[i],"-u")){
                //Check if -u is in the arg
				uPresent = true;
				url = argv[i+1];
			} else if(!strcmp(argv[i],"-i")){
			// Check if -i is in the arg
				iPresent = true;
			} else if(!strcmp(argv[i],"-o")){
                //Check if -o is in the arg
				oPresent = true;
				filename = argv[i+1];
				filePresent = true;
			} else if(!strcmp(argv[i],"-c")){
            //Check if -c is in the arg
				cPresent = true;
			} else if(!strcmp(argv[i],"-s")){
                // Check if -s is in the arg
				sPresent = true;
			} else {
				invalidCMDS++;
			}
		}
		if(invalidCMDS > 3){
			printf("Unrecognized command. Valid commands are -u [url], -i, -c, -o [filename],and -s\n");
			printf("Number of unrecognized commands: %d\n",invalidCMDS - 3);
		}
		
	}

	parseURL(url);	

	// for(int i = 0; i < sizeof(pieces); i++){
	// 	printf("%d spot in pieces is: %s\n",i,pieces[i]);
	// }

	// char *str = *pieces[0];
	// printf("Before lower: %s",str);
	// for(int i = 0; strlen(str); i++){
	//  	str[i] = tolower((unsigned char)  str[i]);
	//  }
	//  printf("After lower: %s",str);


	// Checks to see if the protocol is valid, does not need to be case sensitive
	char str[sizeof(pieces) + 1];
	strcpy(str,pieces[0]);
	for(int i = 0; i < sizeof(pieces) + 1; i++){
		str[i] = tolower(str[i]);
	}

	if(strcmp(str,"http:") != 0){
		printf("This program only supports http. Please type in a URL beginning with http. Not case sensitive\n");
	} else {
		valid = true;
	}

	if(!uPresent){
			printf("Must include -u command\n");
	} else if(!oPresent || !filePresent){
		printf("Must include -o command with a filename\n");
	}  
	

	if(oPresent && uPresent && valid && (invalidCMDS == 3)){
		if(iPresent){	
			INFO();
		}

		if(cPresent){
			printf("REQ: GET /%s HTTP/1.0\r\n",fullPath);
			printf("REQ: Host: %s\r\n",pieces[1]);
			printf("REQ: User-Agent: CWRU CSDS 325 Client 1.0\r\n");
		}
		
		createSocket(pieces[1]);

		printf("Is iPresent? %d\n",iPresent);
		
	}

	
	

	
}

	

