/*
 *  This code is used for sending data packets to the AP
 *
 *  WITHOUT the use of select() function for writing
 *
 * */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <sys/select.h>
#include <netinet/if_ether.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>


#define GENERAL_VERSION 1

#define ANNOUNCE_REQUEST 0 
#define ANNOUNCE_RESPONSE 1
#define HANDOVER_REQUEST 2 
#define HANDOVER_RESPONSE 3 

#define TYPE_NETWORK_NAME 0
#define TYPE_BSSID 1
#define TYPE_OLD_BSSID 2
#define TYPE_MOBILE_STATION_ADDRESS 3
#define TYPE_CAPABILITIES 4
#define TYPE_ANNOUNCE_INTERVAL 5
#define TYPE_HANDOVER_TIMEOUT 6
#define TYPE_MESSAGE_ID 7
#define TYPE_PHY_TYPE 16
#define TYPE_REGULATORY_DOMAIN 17
#define TYPE_RADIO_CHANNEL 18
#define TYPE_BEACON_INTERVAL 19
#define TYPE_OUI_IDENTIFIER 128

#define BUFFER_SIZE 256

struct TLV
{
    uint8_t type;
    uint8_t type_option=0x00;
    uint8_t length;
    unsigned char value[0]; 
                         
}__attribute__((packed));

struct PDU
{

    unsigned char *SSID;
    unsigned char *BSSID;
    unsigned char *Old_BSSID;
    unsigned char *Mobile_Station_Address;
    unsigned char *Capabilities;
    unsigned char *Announce_Interval;
    unsigned char *Handover_Timeout;
    unsigned char *Message_ID;
    unsigned char *PHY_Type;
    unsigned char *Regulatory_Domain;
    unsigned char *Radio_Channel;
    unsigned char *Beacon_Interval;
    unsigned char *OUI_Identifier;

    struct TLV *tlv;

}__attribute__((packed));

struct IAPP
{
    uint8_t general_version;
    uint8_t general_type;

    struct PDU *pdu;

}__attribute__((packed));

unsigned char* alloc_IAPP_msg(int nmemb, uint8_t size)
{
    return (unsigned char*)calloc(nmemb, size);
}


void add_IAPP_Version(unsigned char** p, const char* val)
{
    //fprintf(stderr, "TEST: %p\n", *p);
    *p = (unsigned char*)memcpy(*p, val, 1) + 1;
}

void add_IAPP_Type(unsigned char** p, const char* val)
{
    *p = (unsigned char*)memcpy(*p, val, 1) + 1;
}

void add_IAPP_SSID(unsigned char** p, unsigned char* val)
{
    (*p)[0] = TYPE_NETWORK_NAME;
    (*p)[1] = 0; // Type Option
    uint8_t length = 0;
    for(unsigned i=0; i<33; i++){
        if(val[i]!='\0'){
            length++;
        } else {
            break;
        }
    }
    (*p)[2] = length;
    (*p)+=3;
    (*p) = (unsigned char*)memcpy(*p, (const char*)val, length) + length;
}

void add_IAPP_BSSID(unsigned char** p, uint8_t* val)
{
    (*p)[0] = TYPE_BSSID;
    (*p)[1] = 0;
    uint8_t length=6;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, (const char*)val, length) + length;
}

void add_IAPP_Old_BSSID(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_OLD_BSSID;
    (*p)[1] = 0;
    uint8_t length=6;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_IAPP_Mobile_Station_Address(unsigned char** p, unsigned char* val)
{
    (*p)[0] = TYPE_MOBILE_STATION_ADDRESS;
    (*p)[1] = 0; // Type Option
    uint8_t length = 6;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, (const char*)val, length) + length;
}

void add_IAPP_Capabilities(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_CAPABILITIES;
    (*p)[1] = 0; // Type Option
    uint8_t length = 1;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_IAPP_Announce_Interval(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_ANNOUNCE_INTERVAL;
    (*p)[1] = 0;
    uint8_t length=2;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_IAPP_Handover_Timeout(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_HANDOVER_TIMEOUT;
    (*p)[1] = 0; // Type Option
    uint8_t length = 2;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_IAPP_Message_ID(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_MESSAGE_ID;
    (*p)[1] = 0; // Type Option
    uint8_t length = 2;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_IAPP_Phy_Type(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_PHY_TYPE;
    (*p)[1] = 0; // Type Option
    uint8_t length = 1;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_IAPP_Regulatory_Domain(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_REGULATORY_DOMAIN;
    (*p)[1] = 0; // Type Option
    uint8_t length = 1;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_IAPP_Radio_Channel(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_RADIO_CHANNEL;
    (*p)[1] = 0; // Type Option
    uint8_t length = 1;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_IAPP_Beacon_Interval(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_BEACON_INTERVAL;
    (*p)[1] = 0; // Type Option
    uint8_t length = 2;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_IAPP_OUI_Identifer(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_OUI_IDENTIFIER;
    (*p)[1] = 0; // Type Option
    uint8_t length = 3;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
}

void add_Terminator(unsigned char** p)
{
    /* Terminate with insertion of ' " ' */
    const char* val = "\x22";
    *p = (unsigned char*)memcpy(*p, val, 1);
}

void printhexvalue(void *ptr, int buflen)
{
    unsigned char *buf = (unsigned char*)ptr;
    for(int i=0; i<buflen; i++)
    {
        if(i==buflen-1)
        {
            printf("%02x",buf[i]);
        }
        else
        {
            printf("%02x ", buf[i]);
        }
    }
    printf("\n");
}

void printdecvalue(void *ptr, int buflen)
{
    unsigned char *buf = (unsigned char*)ptr;
    for(int i=0; i<buflen; i++)
    {
        printf("%02d ", buf[i]);
    }
    printf("\n");
}

void hexdump(void *ptr, int buflen)
{
    unsigned char *buf = (unsigned char*)ptr;
    int i, j;
    for(i=0; i<buflen; i+=16)
    {
        printf("%06x: ", i); // printing 00000i
        for(j=0; j<16; j++)
        {
            if(i+j < buflen)
            {
                printf("%02x ", buf[i+j]);
            }else{
                printf("   ");
            }

        }printf(" ");
        for(j=0; j<16; j++)
        {
            if(i+j < buflen)
            {
                printf("%c", isprint(buf[i+j]) ? buf[i+j] : '.');
            }
        }printf("\n");
    }
}

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdef0123456789";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* rand_string_alloc(size_t size)
{
     char *s = (char*)malloc(size + 1);
     if (s) {
         rand_string(s, size);
     }
     return s;
}

struct sockaddr_in localSock;
struct sockaddr_in dest_addr;
struct ip_mreq group;
int sd;
int datalen;
unsigned char databuf[BUFFER_SIZE];

int main(int argc, char *argv[]){

    /*Create a datagram socket on which to receive*/
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0)
    {
        perror("Opening datagram socket error");
        exit(1);
    }
    printf("Opening datagram socket...OK.\n");

    /* Enable SO_REUSEADDR to allow multiple instancesof this
       application to receive copies of the multicase datagrams */
    int  reuse = 1;
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
    {
        perror("Setting SO_REUSEADDR error");
        exit(1);
    }
    printf("Setting SO_REUSEADDR...OK.\n");
    /* Bind to the proper port number with the IP address
       specified as INADDR_ANY */
    memset((char *) &localSock, 0, sizeof(localSock));
    localSock.sin_family = AF_INET;
    localSock.sin_port = htons(2313);
    localSock.sin_addr.s_addr= INADDR_ANY;
    if(bind(sd, (struct sockaddr*)&localSock, sizeof(localSock)))
    {
        perror("Binding datagram socket error");
        close(sd);
        exit(1);
    }
    printf("Binding datagram socket...OK. \n");

    /* Join the multicast group 224.0.1.76 on the local 10.93.0.64
     * or INADDR_ANY interface. Note that this IP_ADD_MEMBERSHIP option
     * must be called for each local interface over which the multicast
     datagrams are to be received.*/
    group.imr_multiaddr.s_addr = inet_addr("224.0.1.76");
    group.imr_interface.s_addr = htonl(INADDR_ANY);
    if(group.imr_multiaddr.s_addr < 0)
    {
        perror("224.0.1.76 is not a valid Multicast-Address\n");
        close(sd);
        exit(1);
    }
    if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
    {
        perror("Adding multicast group error");
        close(sd);
        exit(1);
    }
    printf("Adding multicast group...OK.\n");
   
    /* create random variable for SSID & BSSID and Mobile_Address */
    unsigned char ssid[33]={};
    uint8_t bssid[6]={};
    unsigned char mobile_address[6]={};
    
    int max_traffic = 2;
    int size_array = 100;
    unsigned char ssidholder[33][size_array];
    uint8_t bssidholder[6][size_array];
    unsigned char mobile_addressholder[6][size_array];
/* 1. generate the random array in a loop of 100 elements
 * 2. store the array in a 2D array of 100 elemets
 * 3. extract the 1D arrays from the 2D array
 * 4. add those 1D arrays to the buffer
 * 5. send the buffer
 * */

    /* 1. Generate and 2. Store arrays */
    for(int i=0; i<size_array; i++)
    {
        // 1.1 generate a random ssid
        snprintf((char*)ssid, sizeof(ssid), "RANDOMSSID-%d", 0 + rand() % 99 );
        // 1.2 generate a random bssid
        for(int j=0; j<6; j++)
        {
            bssid[j] = rand() % 255;
            if(bssid[j] == '\x22')
                bssid[j]++;
        }
        // 1.3 generate a random mobile station
        snprintf((char*)mobile_address, sizeof(mobile_address), "Mob%c%u", 65+ (rand() % (90-66)), rand() % (99));
    
        // 2. store the array in a 2D array of 100 elements
        // 2.1 store 'ssid' in 'ssidholder' 
        snprintf((char*)ssidholder[i], sizeof(ssid), "RANDOMSSID-%d", 0 + rand() % 99 );
        //memcpy(ssidholder[i], ssid, sizeof(ssid));
        // 2.2 store 'bssid' in 'bssidholder'
        for(int j=0; j<6; j++)
        {
            bssidholder[j][i]=bssid[j];
        }
        // 2.3 store 'mobile_address' in 'mobile_addressholder'
        snprintf((char*)mobile_addressholder[i], sizeof(mobile_address), "Mob%c%u", 65+ (rand() % (90-66)), rand() % (99) );
        
    }

    /* 3. extract the 1D array from the 2D array */
        unsigned char ssid2[33]={};
        uint8_t bssid2[6]={};
        unsigned char mobile_address2[6]={};

    printf("max_traffic: %d\nsize_array: %d\n", max_traffic, size_array);
    for(int i=0; i<max_traffic; i++)
    {
        /* Send to the proper port number with the IP address
           specified as Multicast IP Address to THIS -->> 224.0.1.76 */
        memset((char *) &dest_addr, 0, sizeof(dest_addr));
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(2313);
        dest_addr.sin_addr.s_addr= inet_addr("224.0.1.76");
        
        /* Add the Data */        
        unsigned char* bufptr = NULL; // bufptr: at the end of buffer (adding)
        unsigned char* bufptr2 = NULL; // bufptr2: at the beginning of buffer (printing)
        bufptr = alloc_IAPP_msg(256, sizeof(char));
        bufptr2 = bufptr;

        /* Fill the IAPP Structure */
        add_IAPP_Version(&bufptr, "\x01");
        add_IAPP_Type(&bufptr, "\x01");
       
        /* Fill the PDU Structure */
        /* 3. Extract and 4. Add to the buffer */
        // 3.1 extract 'ssid2' from 'ssidholder'
        memcpy(ssid2, ssidholder[i], sizeof(ssid));
        // 4.1 add 'ssid2' to the buffer
        add_IAPP_SSID(&bufptr, ssid2); // length 2-33
        
        // 3.2 extract 'bssid2' from 'bssidholder'
        for(int j=0; j<6; j++)
        {
            bssid2[j] = bssidholder[j][i];
        }
        // 4.2 add 'bssid2' to the buffer
        add_IAPP_BSSID(&bufptr, bssid2); // length always 6
        
        add_IAPP_Old_BSSID(&bufptr, "\xff\xff\xff\xff\xff\xff"); // length always 6
       
        // 3.3 extract 'mobile_address2' from 'mobile_addressholder'
        memcpy(mobile_address2, mobile_addressholder[i], sizeof(mobile_address2));
        //for(int k=0; k<6; k++)
        //{
        //    mobile_address2[k] = mobile_addressholder[i][k];
        //}
        // 4.3 add 'mobile_address2' to the buffer
        add_IAPP_Mobile_Station_Address(&bufptr, mobile_address2); // length always 6

        add_IAPP_Capabilities(&bufptr, "\x20"); // length always 1
        add_IAPP_Announce_Interval(&bufptr, "\x00\x78"); // length always 2
        add_IAPP_Handover_Timeout(&bufptr, "\x03\xe8"); // length always 2
        add_IAPP_Message_ID(&bufptr, "\x00\x50"); // length always 2
        add_IAPP_Phy_Type(&bufptr, "\x07"); // length always 1
        add_IAPP_Regulatory_Domain(&bufptr, "\x00"); // length always 1
        add_IAPP_Radio_Channel(&bufptr, "\x06"); // length always 1
        add_IAPP_Beacon_Interval(&bufptr, "\x00\x64"); // length always 2
        add_IAPP_OUI_Identifer(&bufptr, "\x10\x56\x57"); // length always 3
        add_Terminator(&bufptr); // Terminator to determine the End Of Buffer

        /* calculate the length of buffer & PRINT BUFFER */
        int buf_modified_length = 0;
        for(unsigned i=0; i< BUFFER_SIZE; i++){
            if(bufptr2[i]!='\x22'){
                ++buf_modified_length;
            } else {
                break;
            }
        }

        /* 5. send the buffer to the access point */
        /* Send through the socket and print what's been sent */
        printf("Sending datagram message...OK.\n");
        printf("\nThe message to multicast server is: \n");
        printf("New Buffer Length: %i\n",buf_modified_length);
        /*  Send whole data buffer to the Server (Source 'AP') with MULTICAST IP 224.0.1.76  */
        if(sendto(sd, bufptr2, buf_modified_length, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr))< 0)
        {
            perror("Sending Error");
            close(sd);
            exit(1);
        }
        printf("The Transmitted Data is: \n");
        hexdump(bufptr2, buf_modified_length);
        printf("\n");
        free(bufptr2); 
   
        if(i==(max_traffic-1))
        {
            i=-1;
            sleep(30);
        }
    }
    
    return 0;
}
