/*
Author: Jhean C Toledo
CaseID: jct95
Filename: proj4.c
Date created: 11/5/22
Description: This code is to give infomation about a trace file
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include "next.h"
#include <fcntl.h>
#include <arpa/inet.h>

#define TABLE_SIZE 30000
#define midBUF 256
#define smallBUF 16
#define ROW2BYTES 4

bool tPresent = false;
bool sPresent = false;
bool lPresent = false;
bool pPresent = false;
bool mPresent = false;
bool oPresent = false;
bool goodArg = false;
char *outputFile;
char *traceFile;
int validCMDS = 0;
int invalidCMDS = 0;

// Make hashtable stuff here
typedef struct entry_t{
    char *key;            // Port pair (Source port | Dest Port)
    int value;            // Traffic vol
    struct entry_t *next; // Pointer to the next entry in the list
} entry_t;

typedef struct {
    entry_t **entries; // List of entries
} ht_t;

// Calculate the hash value for a given key
unsigned int hash(const char *key) {
    unsigned long int index = 0; // Dont want a negative index
    int i = 0;
    int key_len = strlen(key);
    int prime = 37; // A good prime number to multiply by for hashing
    
    for (; i < key_len; i++) {
        index = index * prime + key[i];
    }

    index = index % TABLE_SIZE;
    return index;
}

// Allocates memory for the key value pair.
entry_t *ht_pair(const char *key, int value) {

    // Allocate memory for the entry
    entry_t *entry = malloc(sizeof(entry_t));
    entry->key = malloc(strlen(key) + 1);

    // Copy the key and value in place
    strcpy(entry->key, key);
    entry->value = value;

    // Make the entry next null so it can be set later
    entry->next = NULL;

    return entry;
}

// Create a hashtable
ht_t *ht_create() {
    // Allocate memory for table
    ht_t *hashtable = malloc(sizeof(ht_t) * 1);

    // Allocate memory for table entries
    hashtable->entries = malloc(sizeof(entry_t*) * TABLE_SIZE);

    // Set every entry to null for now
    int i = 0;
    for (; i < TABLE_SIZE; ++i) {
        hashtable->entries[i] = NULL;
    }

    return hashtable;
}

// Put a key value pair into a given hashtable
void ht_put(ht_t *hashtable, const char *key, int value) {
    unsigned int slot = hash(key);

    // Try to look up an entry set
    entry_t *entry = hashtable->entries[slot];

    // No entry means slot empty, insert immediately
    if (entry == NULL) {
        hashtable->entries[slot] = ht_pair(key, value);
        return;
    } else {
    // Walk through each entry until end is found
        while (entry != NULL) { 
            // Check if the key 
            if(strcmp(entry->key,key) == 0){
                // If the key already exists, update its value
                entry->value = entry->value + value;
                return;
            } else if(entry->next == NULL) {
                // Add it to the list
                entry->next = ht_pair(key,value);
                return;
            } 
            entry = entry->next;
        }
    }
}


// Print the table
void ht_print(ht_t *hashtable) {
    // Run through everything in the table
    for (int i = 0; i < TABLE_SIZE; ++i) {
        entry_t *entry = hashtable->entries[i];
       // If the entry in the table exists, print everything in the bin 
       while(entry != NULL){
            printf("%s %d\n", entry->key, entry->value);
            entry = entry->next;
        }
    }
}


// mMode. Keeps track of source and destination addresses with the amount of data
void mMode(){
    int file = open(traceFile,O_RDONLY);
    if (file < 0)
        errexit("Error: Could not open file");

    struct pkt_info pinfo;
    struct in_addr ip_addr;
    char* source = malloc(midBUF); 
    char* dest = malloc(midBUF);   
    char *key = malloc(midBUF);
    ht_t *portPairs = ht_create();
    int traffic_volume = 0; // Data we need to keep track of. 

    while(next_packet(file,&pinfo) == 1){
        
        // Has to have a TCP header
        if(pinfo.tcph != NULL){
            // Set the traffic volume for this individual packet
            traffic_volume = pinfo.iph->tot_len - (pinfo.tcph->th_off + pinfo.iph->ihl) * ROW2BYTES;
            // Set the ports correctly
            ip_addr.s_addr = pinfo.iph->saddr;
            strcpy(source,inet_ntoa(ip_addr));
            ip_addr.s_addr = pinfo.iph->daddr;
            strcpy(dest,inet_ntoa(ip_addr));
            strcpy(key,source);
            strcat(key," ");
            strcat(key,dest);
            // Put the key in the table with its volume
            ht_put(portPairs,key,traffic_volume);
        }
    }
    ht_print(portPairs);
}







// pMode. Packet printing, prints things relating to TCP packets 
void pMode(){

    struct in_addr ip_addr;
    int numWithAck = 0;

    int file = open(traceFile,O_RDONLY);
    if (file < 0)
        errexit("Error: Could not open file");

    struct pkt_info pinfo;

    while(next_packet(file,&pinfo) == 1){

        // Is a TCP packet and its fully present
        if(pinfo.ethh != NULL && pinfo.iph != NULL && pinfo.iph->protocol == IPPROTO_TCP){
            printf("%0.6f",pinfo.now);

            ip_addr.s_addr = pinfo.iph->saddr;
            printf(" %s", inet_ntoa(ip_addr));

            ip_addr.s_addr = pinfo.iph->daddr;
            printf(" %s",inet_ntoa(ip_addr));

            printf(" %d",pinfo.iph->ttl);       // Time to live 
            printf(" %d",pinfo.tcph->th_sport); // TCP source port 
            printf(" %d",pinfo.tcph->th_dport); // TCP dest port
            printf(" %d",pinfo.tcph->th_win);   // TCP window size
            printf(" %u",pinfo.tcph->th_seq);   // TCP seq number
            if(pinfo.tcph->ack == 1){
                numWithAck++;
                printf(" %u\n",pinfo.tcph->th_ack); // TCP ack number
            } else {
                printf(" -\n");
            }
        }
    }
    // printf("Number of TCP with ACKs: %d",numWithAck);

}

//lMode. Length mode, prints things relating to the type of protocol
void lMode(){
    int updHeaderSize = 8;
    
    // Used these values for extra credit 
    // int defaultSize = 66; 
    // int finCount = 0; 
    // int synCount = 0;
    // int numOfTCP = 0;
    // int numOfUDP = 0;
    // int numOfUnknown = 0;
    int file = open(traceFile,O_RDONLY);
    if (file < 0)
        errexit("Error: Could not open file");

    struct pkt_info pinfo;
    while(next_packet(file,&pinfo) == 1){
        if( pinfo.ethh != NULL && pinfo.iph != NULL){
            printf("%0.6f",pinfo.now);
            printf(" %d",pinfo.caplen);
            printf(" %d",pinfo.iph->tot_len);
            printf(" %d",pinfo.iph->ihl * ROW2BYTES);
            if(pinfo.iph->protocol == IPPROTO_TCP){
                printf(" T");
              //  numOfTCP++;
                if(pinfo.tcph != NULL && pinfo.tcph->th_off != 0){
                    printf(" %d",pinfo.tcph->th_off * ROW2BYTES);
                    printf(" %d\n", pinfo.iph->tot_len - (pinfo.tcph->th_off + pinfo.iph->ihl) * ROW2BYTES);
                }else{
                    printf(" - -\n");
                }
                // if(pinfo.tcph->fin == 1){
                //     finCount++;
                // }

                // if(pinfo.tcph->syn == 1){
                //     synCount++;
                // }
            }else if(pinfo.iph->protocol == IPPROTO_UDP){
                printf(" U");
               // numOfUDP++;
                if(pinfo.udph != NULL && pinfo.udph->uh_ulen != 0){
                    printf(" %d",updHeaderSize); // Always 8 cause of UDP
                    printf(" %d\n", pinfo.udph->uh_ulen - updHeaderSize);
                }else{
                    printf(" - -\n");
                }
            } else {
               // numOfUnknown++;
                printf(" ? ? ?\n");
            } 
        } else if(pinfo.ethh !=NULL && pinfo.ethh->ether_type == ETHERTYPE_IP && pinfo.iph == NULL){
            printf("%0.6f %d - - - - -\n",pinfo.now,pinfo.caplen);
        }
    }
    // printf("Number of TCP pkts: %d\n",numOfTCP);
    // printf("Number of UDP pkts: %d\n",numOfUDP);
    // printf("Number of unknown transport protocol: %d\n",numOfUnknown);
    // printf("Number of IP pkts with options: %d\n",pktsWithOpt);
    // printf("Number of finishing connections: %d\n",finCount);
    // printf("Number of TCP start up connections: %d\n",synCount);
    
}

//sMode. Summary mode, prints a overall summary about the trace file
void sMode(){
    int totalPKTS = 0;
    int ipPKTS = 0;
    double first_time = 0;
    double last_time = 0;

	int file = open(traceFile,O_RDONLY);
    if (file < 0)
        errexit("Error: Could not open file");

    struct pkt_info pinfo;
    while(next_packet(file,&pinfo) == 1){
        // This is the first packet
        if (totalPKTS == 0){
            first_time = pinfo.now;
        }
        totalPKTS++;
        if (pinfo.iph != NULL){
            ipPKTS++;
        } 
        last_time = pinfo.now;  
    }

    printf("FIRST PKT: %0.6f\n",first_time);
    printf("LAST PKT: %0.6f\n",last_time);
    printf("TOTAL PACKETS: %d\n",totalPKTS);
    printf("IP PACKETS: %d\n",ipPKTS);
}

void errexit (char *msg){
    fprintf (stdout,"%s\n",msg);
    exit (1);
}


// next_packet. Taken from the class webpage. Takes in a file and a struct to put the info about the file into. 
// Returns 1 when a packet was read, and 0 when we hit the end of the file
unsigned short next_packet (int fd, struct pkt_info *pinfo){
    struct meta_info meta;
    int bytes_read;
    double secondsSince;
    double microSeconds;
    double convert = 1000000.00000; // Conversion factor to change mirco seconds to seconds
    memset (pinfo,0x0,sizeof (struct pkt_info));
    memset (&meta,0x0,sizeof (struct meta_info));

    /* read the meta information */
    bytes_read = read (fd,&meta,sizeof (meta));
    if (bytes_read == 0)
        return (0);
    if (bytes_read < sizeof (meta))
        errexit ("cannot read meta information");
    pinfo->caplen = ntohs (meta.caplen);
    /* set pinfo->now based on meta.secs & meta.usecs */

    // Take the meta info that has the seconds and covert them to proper units.
    secondsSince = ntohl(meta.secs);
    microSeconds = ntohl(meta.usecs);
    microSeconds = microSeconds / convert;
    pinfo->now = (double) secondsSince + microSeconds;

    if (pinfo->caplen == 0)
        return (1);
    if (pinfo->caplen > MAX_PKT_SIZE)
        errexit ("packet too big");
    /* read the packet contents */
    bytes_read = read (fd,pinfo->pkt,pinfo->caplen);
    if (bytes_read < 0)
        errexit ("error reading packet");
    if (bytes_read < pinfo->caplen)
        errexit ("unexpected end of file encountered");
    if (bytes_read < sizeof (struct ether_header))
        return (1);
    pinfo->ethh = (struct ether_header *)pinfo->pkt;
    pinfo->ethh->ether_type = ntohs (pinfo->ethh->ether_type);
    if (pinfo->ethh->ether_type != ETHERTYPE_IP)
        /* nothing more to do with non-IP packets */
        return (1);
    if (pinfo->caplen == sizeof (struct ether_header))
        /* we don't have anything beyond the ethernet header to process */
        return (1);
    /* set pinfo->iph to start of IP header */
    pinfo->iph = (struct iphdr *) (pinfo->pkt + sizeof(struct ether_header));
    pinfo->iph->tot_len = ntohs(pinfo->iph->tot_len);  
    if(pinfo->iph->protocol == IPPROTO_TCP){
        pinfo->iph->protocol = IPPROTO_TCP;
        pinfo->tcph = (struct tcphdr *) (pinfo->pkt + (pinfo->iph->ihl * ROW2BYTES) + sizeof(struct ether_header));
        //set up values in pinfo->tcph as needed here
        pinfo->tcph->th_sport = ntohs(pinfo->tcph->th_sport);
        pinfo->tcph->th_dport = ntohs(pinfo->tcph->th_dport);
        pinfo->tcph->th_win = ntohs(pinfo->tcph->th_win);
        pinfo->tcph->th_seq = ntohl(pinfo->tcph->th_seq);
        pinfo->tcph->th_ack = ntohl(pinfo->tcph->th_ack);
        if (pinfo -> caplen == sizeof(struct ether_header) + sizeof(struct iphdr))
            return(1);
    } else if(pinfo->iph->protocol == IPPROTO_UDP){
        pinfo->iph->protocol = IPPROTO_UDP;
        pinfo->udph = (struct udphdr *) (pinfo->pkt + sizeof(struct ether_header) + sizeof(struct iphdr));
        pinfo->udph->uh_ulen = ntohs(pinfo->udph->uh_ulen);
        pinfo->udph->uh_sum = ntohs(pinfo->udph->uh_sum);
         if (pinfo -> caplen == sizeof(struct ether_header) + sizeof(struct iphdr))
            return(1);
    }
    return (1);
}

int main(int argc, char* argv[]){

    if(argc == 1){
        printf("Not enough commands passed. Must include -t [trace file] with either -s, -l, -p, or -m\n");
    }

    for (int i = 0; i < argc; i++){
        if(!strcmp(argv[i],"-t")){
            traceFile = argv[i+1];
            tPresent = true;
            validCMDS++;
        } else if(!strcmp(argv[i],"-s")){
            sPresent = true;
            validCMDS++;
        } else if(!strcmp(argv[i],"-l")){
            lPresent = true;
            validCMDS++;
        } else if(!strcmp(argv[i],"-p")){
            pPresent = true;
            validCMDS++;
        } else if(!strcmp(argv[i],"-m")){
            mPresent = true;
            validCMDS++;
        } else if(!strcmp(argv[i], "-o")) {
            outputFile = argv[i+1];
            oPresent = true;
            validCMDS++;
        } else {
            invalidCMDS++;
        }

    }

    if(invalidCMDS > 2){
        printf("Unrecongized command has been found. Valid commands are -t [trace file] with either -s -l -p or -m\n");
    }
    if(validCMDS > 2){
        printf("Can only run one option at a time, valid options are -s -l -p or -m\n");
    }
    if(validCMDS == 1){
        printf("Must include both a trace file and a valid command\n");
    }
    if(validCMDS == 2 && invalidCMDS == 2) {
        goodArg = true;
    }

    if(!tPresent){
        printf("Must include a trace file\n");
    }else if(sPresent && tPresent && goodArg ){
        sMode();
    } else if(lPresent && tPresent && goodArg){
        lMode();
    }else if(pPresent && tPresent && goodArg){
        pMode();
    }else if(mPresent && tPresent && goodArg){
        mMode();
    }
}
