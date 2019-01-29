
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

#define GENERAL_VERSION             1

#define ANNOUNCE_REQUEST            0 
#define ANNOUNCE_RESPONSE           1 
#define HANDOVER_REQUEST            2 
#define HANDOVER_RESPONSE           3
#define SEENSTATION_REQUEST         4
#define SEENSTATION_RESPONSE        5
#define SEENNEIGHBORS_INFO          6
#define IAPP_MAXTYPE                6

#define TYPE_NETWORK_NAME           0
#define TYPE_BSSID                  1
#define TYPE_OLD_BSSID              2
#define TYPE_MOBILE_STATION_ADDRESS 3
#define TYPE_CAPABILITIES           4
#define TYPE_ANNOUNCE_INTERVAL      5
#define TYPE_HANDOVER_TIMEOUT       6
#define TYPE_MESSAGE_ID             7
#define TYPE_PHY_TYPE               16
#define TYPE_REGULATORY_DOMAIN      17
#define TYPE_RADIO_CHANNEL          18
#define TYPE_BEACON_INTERVAL        19
#define TYPE_OUI_IDENTIFIER         128
#define TYPE_NEIGHBOR_INFO          133
#define TYPE_LOAD_INFO              134
#define TYPE_SEEN_STATIONS          135
#define TYPE_SEEN_NEIGHBORS         136

#define BUFFER_SIZE                 256

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
    unsigned char *Neighbor_Info;
    unsigned char *Load_Info;
    unsigned char *Seen_Stations;
    unsigned char *Seen_Neighbors;

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
    //printf("\n");
}

void printdecvalue(void *ptr, int buflen)
{
    unsigned char *buf = (unsigned char*)ptr;
    for(int i=0; i<buflen; i++)
    {
        printf("%02d ", buf[i]);
    }
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
unsigned char databuf[BUFFER_SIZE] = {};

int main(int argc, char *argv[]){
    
    /* Select() related */
    int activity;
    struct timeval tv;
    /* Set of socket descriptors */
    fd_set readfds;

    /* Create a datagram socket on which to receive */
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0)
    {
        perror("Opening datagram socket error");
        exit(1);
    }
    
    printf("Opening datagram socket...OK.\n");

    // clear the set ahead of time
    FD_ZERO(&readfds);

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

    /* Length of Buffer */
    datalen = sizeof(databuf);
   
    /* Preparation for select() */
    while(1)
    {
        
        tv.tv_sec = 2;
        tv.tv_usec = 500000;
        // add file descriptor to the set
        FD_SET(sd, &readfds);
        //FD_SET(sd, &writefds);
        activity = select(sd+1, &readfds, NULL, NULL, &tv);
        
        if((activity < 0))
        {
            perror("Select error");
        }
        else if(activity == 0)
        {
            printf("Timeout! No data in 2,5 seconds\n");
        }
        else
        {
            /* Here is what happens to every socket "reading" */
            if(FD_ISSET(sd, &readfds))
            {
                /* define a structure member to get source IP Address */
                struct sockaddr_in from;
                socklen_t fromLen = sizeof(from);
  
                /* Receive the data packet */
                memset(databuf, 0, sizeof(databuf));
                int readoutlen = recvfrom(sd, databuf, sizeof(databuf), 0, (struct sockaddr*)&from, &fromLen);
                printf("-- Message Length : %d\n", readoutlen);
  
                /* Get source IP Address */
                char ip[16];
                inet_ntop(AF_INET, &from.sin_addr, ip, sizeof(ip));
                printf("-- Source IP Address: %s -- Port: %i\n", ip, ntohs(from.sin_port));
  
                
                /* Check Receiving */
                if(readoutlen < 0)
                {
                perror("Reading datagram message error");
                close(sd);
                exit(1);
                }
                /* Send to the proper port number with the IP address
                specified as Multicast IP Address to THIS -->> 224.0.1.76 */
                memset((char *) &dest_addr, 0, sizeof(dest_addr));
                dest_addr.sin_family = AF_INET;
                dest_addr.sin_port = htons(2313);
                dest_addr.sin_addr.s_addr= inet_addr("224.0.1.76");
                printf("-- Destination IP Address: %s -- Port: %i\n", inet_ntoa(dest_addr.sin_addr), ntohs(dest_addr.sin_port));
  
                /* Read from the socket */
                printf("Reading datagram message...OK.\n");
                printf("\nThe message from multicast server is: \n");
                hexdump(databuf, readoutlen); // This is to print out the Data Package
                printf("\n\n");
  
                // 1. Define and Initialize the IAPP Pointer to the 'original Buffer'
                struct IAPP *IAPPPtr = (struct IAPP*)databuf;
            
                // 2.(a) Define another databuffer (*bytep) shifted by two
                unsigned char *bytep = databuf+2; /* skip version + type field sven */
                // 2.(b) Define and Initialize the tlv Pointer to the 'modified Buffer'
                struct TLV *p =(struct TLV *)bytep;
            
                /*      Check for General Version and Type of Packet    */
                printf("General Version: (%u)\n", IAPPPtr->general_version);
            
                switch(IAPPPtr->general_type)
                {
                    case ANNOUNCE_REQUEST:
                        printf("General Type: Announce Request (%u)\n", IAPPPtr->general_type);
                        break;
            
                    case ANNOUNCE_RESPONSE:
                        printf("General Type: Announce Response (%u)\n", IAPPPtr->general_type);
                        break;
            
                    case HANDOVER_REQUEST:
                        printf("General Type: Handover Request  (%u)\n", IAPPPtr->general_type);
                        break;
            
                    case HANDOVER_RESPONSE:
                        printf("General Type: Handover Response  (%u)\n", IAPPPtr->general_type);
                        break;
            
                    case SEENSTATION_REQUEST:
                        printf("General Type: SeenStations Request (%u)\n", IAPPPtr->general_type);
                        break;
            
                    case SEENSTATION_RESPONSE:
                        printf("General Type: SeenStations Response (%u)\n", IAPPPtr->general_type);
                        break;
            
                    case SEENNEIGHBORS_INFO:
                        printf("General Type: SeenNeighbors Info (%u)\n", IAPPPtr->general_type);
                        break;
            
                    default:
                        printf("General Type: Unknown Data Packet!\n");
            
                }
                printf("Protocol Data Units: \n\n");
                
                /*Loop through the buffer -beginning from the third Index of buffer-*/
                for(int i=0; (i< datalen) && (p->length != 0) ;)
                {
                    p = (struct TLV *)bytep; /* sven */
                    /*      Shifting the address for p to next PD Member    */
                    bytep += 3 + p->length;
            
                    /*      Print the Members of struct     */
                    /*      1.Check for every standard Type
                     *      2.Print its name
                     *      3.Send the specified data frame */
            
                    switch(p->type)
                    {
                        case TYPE_NETWORK_NAME:
                            if(p->length == 0)
                            {
                                printf("-- End of Data Packet --\n");
                                break;
                            }
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "NETWORK NAME (0)", "________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "SSID", "LENGTH");
                            printf("\t%-30.*s\t\t|\t\t%-30d\n", p->length, p->value, p->length);
                            break;

                        case TYPE_BSSID:
       
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "MAC ADDRESS (1)", "_________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "BSSID", "LENGTH");
                            printf("\t");
                            printhexvalue(p->value, p->length);
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_OLD_BSSID:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "OLD MAC ADDRESS (2)", "______________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "OLD BSSID", "LENGTH");
                            printf("\t");
                            printhexvalue(p->value, p->length);
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_MOBILE_STATION_ADDRESS:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "MOB.STATION ADDRESS(3)", "_________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "MS ADDRESS", "LENGTH");
                            printf("\t");
                            printhexvalue(p->value, p->length);
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_MESSAGE_ID:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "MESSAGE ID (7)", "_________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "MESSAGE ID", "LENGTH");
                            printf("\t");
                            printf("(%u) \t\t", ntohs(*(uint16_t *)(p->value)));
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_CAPABILITIES:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "CAPABILITIES (4)", "________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "CAPABILITIES (HEX)", "LENGTH");
                            printf("\t");
                            printhexvalue(p->value, p->length);
                            printf("\t\t\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_ANNOUNCE_INTERVAL:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "ANNOUNCE INTERVAL (5)", "____________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "ANNOUNCE INT", "LENGTH");
                            printf("\t");
                            printf("(%u) seconds\t", ntohs(*(uint16_t *)(p->value)));
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_HANDOVER_TIMEOUT:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "HANDOVER TIMEOUT (6)", "____________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "HANDOVER TIMEOUT", "LENGTH");
                            printf("\t");
                            printf("(%u) Kus\t", ntohs(*(uint16_t *)(p->value)));
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_PHY_TYPE:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "PHYSICAL TYPE (16)", "____________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "PHY TYPE", "LENGTH");
                            printf("\t(");
                            printdecvalue(p->value, p->length);
                            printf("\b)\t\t");
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_REGULATORY_DOMAIN:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "REGULATORY DOMAIN (17)", "____________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "REG. DOMAIN", "LENGTH");
                            printf("\t(");
                            printdecvalue(p->value, p->length);
                            printf("\b)\t\t");
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_RADIO_CHANNEL:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "RADIO CHANNEL (18)", "____________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "RADIO CHANNEL", "LENGTH");
                            printf("\t(");
                            printdecvalue(p->value, p->length);
                            printf("\b)\t\t");
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_BEACON_INTERVAL:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "BEACON INTERVAL (19)", "_________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "BEACON INTERVAL", "LENGTH");
                            printf("\t");
                            printf("(%u) \t\t", ntohs(*(uint16_t *)(p->value)));
                            printf("\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_OUI_IDENTIFIER:
                            printf("\t\t\t\t\t%-40s\n\t\t\t\t\t%-s\n", "OUI IDENTIFIER (128)", "_________________");

                            printf("\t%-30s\t\t|\t\t%-30s\n", "OUI_IDENT.", "LENGTH");
                            printf("\t");
                            printhexvalue(p->value, p->length);
                            printf("\t\t\t\t|\t\t%-30d\n", p->length);
                            break;
            
                        case TYPE_NEIGHBOR_INFO:
                            if(IAPPPtr->general_type == SEENNEIGHBORS_INFO)
                            {
                                printf("\t\t\t%-40s\n\t\t\t%-s\n", "NEIGHBOR INFO (133)", "___________________");
                                printf("\t%-10s\t\t|\t\t%-10s\n", "MAC-Address", "Strength");
                                printf("\t");
                                printhexvalue(p->value, (p->length)-1);
                                printf("\t|\t\t%-10d\n", p->value[6]);
                                break;
                            }
                            else 
                            {
                                printf("- Neighbor INFO : (%u)\n", p->type);
                                printf("- Length is : %u \n", p->length);
                                printf("- Value : ");
                                printhexvalue(p->value, p->length);
                                break;
                            }
  
                        case TYPE_LOAD_INFO:
                            printf("- Load INFO : (%u)\n", p->type);
                            printf("- Length is : %u \n", p->length);
                            printf("- Value : ");
                            printhexvalue(p->value, p->length);
                            printf("\n");
                            break;
  
                        case TYPE_SEEN_STATIONS:
                            printf("- Seen Stations : (%u)\n", p->type);
                            printf("- Length is : %u \n", p->length);
                            printf("- Value : ");
                            printhexvalue(p->value, p->length);
                            printf("\n");
                            break;
  
                        case TYPE_SEEN_NEIGHBORS:
                            printf("- Seen Neighbors : (%u)\n", p->type);
                            printf("- Length is : %u \n", p->length);
                            printf("- Value : ");
                            printhexvalue(p->value, p->length);
                            printf("\n");
                            break;
  
                        default:
                            printf("- Unknown PDU Type : (%u)\n", p->type);
                            printf("- Length is : %u \n", p->length);
                            printf("- Value : %.*s - ", p->length, p->value);
                            printhexvalue(p->value, p->length);
                            printf("\n");
                    }

                    /*      Shifting the address for i      */
                    i += sizeof(TLV) + (p->length); //next beginning
                    printf("\n");
                }
            }
        }
  
    }
    return 0;
}
