#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/if_ether.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>
#include <iostream>

using namespace std;

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
    unsigned char value[0]; /* keeps it of sizeof() = 0 */ /*       the same as ==>>
                                                                    unsigned char *value;
                                                                    but the sizeof(pointer) has a value
                                                                    not equal zero  */
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

/* another way to -fill the buffer- to send it
   p = alloc_IAPP_msg();
   add_IAPP_network_name(p, "ssid");
   add_IAPP_radio_channel(p, 6);
   */
// malloc a buffer (using calloc for arrays)

unsigned char* alloc_IAPP_msg(int nmemb, uint8_t size)
{
    return (unsigned char*)calloc(nmemb, size);
}


void add_IAPP_Version(unsigned char** p, const char* val)
{
    fprintf(stderr, "TEST: %p\n", *p);
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

void add_IAPP_Mobile_Station_Address(unsigned char** p, const char* val)
{
    (*p)[0] = TYPE_MOBILE_STATION_ADDRESS;
    (*p)[1] = 0; // Type Option
    uint8_t length = 6;
    (*p)[2] = length;
    (*p)+=3;
    *p = (unsigned char*)memcpy(*p, val, length) + length;
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
    else
    {
        printf("Opening datagram socket...OK.\n");
    }

    /* Enable SO_REUSEADDR to allow multiple instancesof this
       application to receive copies of the multicase datagrams */
    int  reuse = 1;
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
    {
        perror("Setting SO_REUSEADDR error");
        exit(1);
    }
    else
    {
        printf("Setting SO_REUSEADDR...OK.\n");
    }
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
    else
    {
        printf("Binding datagram socket...OK. \n");
    }

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
    else
    {
        printf("Adding multicast group...OK.\n");
    }
    
        /* Send to the proper port number with the IP address
           specified as Multicast IP Address to THIS -->> 224.0.1.76 */
        memset((char *) &dest_addr, 0, sizeof(dest_addr));
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(2313);
        dest_addr.sin_addr.s_addr= inet_addr("224.0.1.76");
        
        /* Add the Data */        
        unsigned char* bufptr = NULL; // bufptr: at the end of buffer
        unsigned char* bufptr2 = NULL; // bufptr2: at the beginning of buffer
        bufptr = alloc_IAPP_msg(256, sizeof(char));
        bufptr2 = bufptr;

        /* Fill the IAPP Structure */
        fprintf(stderr, "TEST1: %p\n", bufptr);
        add_IAPP_Version(&bufptr, "\x01");
        fprintf(stderr, "TEST3: %p\n", bufptr);
        add_IAPP_Type(&bufptr, "\x00");
        fprintf(stderr, "TEST4: %p\n", bufptr);
       
        /* Fill the PDU Structure */
        unsigned char ssid[33]="SSID Space Test?0000000000003256";
        add_IAPP_SSID(&bufptr, ssid); // length 2-33
        fprintf(stderr, "TEST5: %p\n", bufptr);
        uint8_t bssid[6]={0x12,0x23,0x34,0x45,0x56,0x67};
        add_IAPP_BSSID(&bufptr, bssid); // length always 6
        // hexdump(bufptr2, 32);
        fprintf(stderr, "TEST6: %p\n", bufptr);
        add_IAPP_Old_BSSID(&bufptr, "\x00\x00\x00\x00\x00\x00"); // length always 6
        add_IAPP_Mobile_Station_Address(&bufptr, "Mobile"); // length always 6
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

        /* calculate the length of buffer */
        int buf_modified_length = 0;
        for(unsigned i=0; i< 256; i++){
            if(bufptr2[i]!='\x22'){
                ++buf_modified_length;
            } else {
                break;
            }
        }

        /* Send through the socket */
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
        else
        {
            printf("The Transmitted Data is: \n");
            hexdump(bufptr2, buf_modified_length);
            printf("\n");
        }
        printf("\n");
        free(bufptr2); 

        /* Commented */
//        // 1. Define and Initialize the IAPP Pointer to the 'original Buffer'
//        struct IAPP *IAPPPtr = (struct IAPP*)databuf;
//
//        // 2.(a) Define another databuffer (*bytep) shifted by two
//        unsigned char *bytep = databuf+2; /* skip version + type field sven */
//        // 2.(b) Define and Initialize the tlv Pointer to the 'modified Buffer'
//        struct TLV *p =(struct TLV *)bytep;
//
//
//
//        /*      Check for General Version and Type of Packet    */
//        printf("General Version: (%u)\n", IAPPPtr->general_version);
//
//        switch(IAPPPtr->general_type)
//        {
//            case ANNOUNCE_REQUEST:
//                printf("General Type: Announce Request (%u)\n", IAPPPtr->general_type);
//                break;
//
//            case ANNOUNCE_RESPONSE:
//                printf("General Type: Announce Response (%u)\n", IAPPPtr->general_type);
//
//                break;
//            case HANDOVER_REQUEST:
//                printf("General Type: Handover Request  (%u)\n", IAPPPtr->general_type);
//
//                break;
//            case HANDOVER_RESPONSE:
//                printf("General Type: Handover Response  (%u)\n", IAPPPtr->general_type);
//
//                break;
//            default:
//                printf("Unknown Data Packet!\n");
//
//        }
 
//        printf("Protocol Data Units: \n\n");
//        uint8_t dataframeslength=0;
//        uint8_t no_of_elements = 0;
//
//        for(int i=0; (i< datalen) && (p->length != 0) ;)
//        {
//            p = (struct TLV *)bytep; /* sven */
//            /*      Shifting the address for p to next PD Member    */
//            bytep += 3 + p->length;
//
//            /*      Print the Members of struct     */
//            /*      1.Check for every standard Type
//             *      2.Print its name
//             *      3.Send the specified data frame */
//
//            switch(p->type)
//            {
//                case TYPE_NETWORK_NAME:
//                    if(p->length == 0)
//                    {
//                        printf("-- End of Data Packet --\n");
//                        break;
//                    }
//                    printf("- Network Name : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : %s - ", p->value);
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_BSSID:
//                    printf("- BSSID : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : ");
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_OLD_BSSID:
//                    printf("- Old BSSID : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : ");
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_MOBILE_STATION_ADDRESS:
//                    printf("- Mobile Station Address : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : ");
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_MESSAGE_ID:
//                    printf("- Message ID : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : ");
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_CAPABILITIES:
//                    printf("- Capabilities : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : ");
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_ANNOUNCE_INTERVAL:
//                    printf("- Announce Interval : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : (%u) seconds - ", ntohs(*(uint16_t *)(p->value)));
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_HANDOVER_TIMEOUT:
//                    printf("- Handover Timeout : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : (%u) Kus - ", ntohs(*(uint16_t *)(p->value)));
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_PHY_TYPE:
//                    printf("- Phy Type : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : ");
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_REGULATORY_DOMAIN:
//                    printf("- Regulatory Domain : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : ");
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_RADIO_CHANNEL:
//                    printf("- Radio Channel : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : ");
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_BEACON_INTERVAL:
//                    printf("- Beacon Interval : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : (%u) Kus - ", ntohs(*(uint16_t *)(p->value)));
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                case TYPE_OUI_IDENTIFIER:
//                    printf("- OUI Identifier : (%u) \n", p->type);
//                    printf("- Length is : %u \n", p->length);
//                    printf("- Value : ");
//                    printhexvalue(p->value, p->length);
//                    printf("\n");
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//                    break;
//
//                default:
//                    printf("Unknown PDU Type : (%u)\n", p->type);
//                    dataframeslength += p->length;
//                    no_of_elements +=3;
//            }
//            /*      Shifting the address for i      */
//            i += sizeof(TLV) + (p->length); //next beginning
//
//            printf("\n");
//        }


    return 0;
}
