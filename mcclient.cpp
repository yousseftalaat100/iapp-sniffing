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

#define ANNOUNCE_RESPONSE 1
#define HANDOVER_REQUEST 2

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

struct tag_len_val
{
	uint8_t type;
	uint8_t type_option=0x00;
	uint8_t length;
	unsigned char value[0]; /* keeps it of sizeof() = 0 */ /*	the same as ==>>
							       unsigned	char *value;
							but the sizeof(pointer) has a value
								not equal zero	*/
}__attribute__((packed));

//struct protocol_data
//{
//	char *Network_Name;
//	char *BSSID;
//	char *Capabilities;
//	char *Announce_Interval;
//	char *Handover_Timeout;
//	char *PHY_Type;
//	char *Regulatory_Domain;
//	char *Radio_Channel;
//	char *Beacon_Interval;
//	char *OUI_Identifier;
//
//	struct tag_len_val tlv;
//	
//}__attribute__((packed));

struct iapp
{
	uint8_t general_version;
	uint8_t general_type;
	
	struct tag_len_val tlv;
//	struct protocol_data prodata;
	
}__attribute__((packed));
// Function to convert hexadecimal to decimal 
int hexadecimalToDecimal(void* hexval) 
{   const char *hexVal = (const char*)hexval;
    int len = strlen(hexVal); 
      
    // Initializing base value to 1, i.e 16^0 
    int base = 1; 
      
    int dec_val = 0; 
      
    // Extracting characters as digits from last character 
    for (int i=len-1; i>=0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        } 
  
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
          
            // incrementing base by power 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
}

/*	Stringbuffer(p->value) to  Decimal 	*/
int power(int base, int exp)
{
	int ret=1;
	int i;
	for(i=0;i<exp;i++){
    		ret*=base;
	}
	return ret;
}
int stringToInt(unsigned char* nrStr)
{
	int nrChars=0;
	while(nrStr[nrChars]!='\0'){
    		nrChars++;
	}


	int result=0;
	int i=0;
	while(nrStr[i]!='\0')
	{
	    	//while you dont get to the end of the string
	        int digit=nrStr[i]-48;//48 is zero ascii code
	        int exp=nrChars-i-1;
	        int add=digit*power(10,exp);
	        result+=add;
		i++;
	}
	return result;
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
		if(i==buflen-1)
		{
		printf("%02d",buf[i]);
		}
		else
		{
		printf("%02d ", buf[i]);
		}
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

struct ip_mreq group;
int sd;
int datalen;
unsigned char databuf[128];

struct tag_len_val tlv;
//struct tag_len_val *p = (tag_len_val*)malloc(10*sizeof(struct tag_len_val));

//struct iapp info;
//struct iapp *infoPtr;
//infoPtr = &info; /*so to access the sructures' levels: */
			/*infoPtr->version & infoPtr->general_type*/
			/*infoPtr->proto->Network_Name*/

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
	   interface. Note that this IP_ADD_MEMBERSHIP option must be 
	   called for each local interface over which the multicast
	   datagrams are to be received.*/
	group.imr_multiaddr.s_addr = inet_addr("224.0.1.76");
	group.imr_interface.s_addr = inet_addr("10.93.0.64");
	if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
	{
		perror("Adding multicast group error");
		close(sd);
		exit(1);
	}else
	{
		printf("Adding multicast group...OK.\n");
	}
	
	/* Read from the socket */
	datalen = sizeof(databuf);
	int readOut = read(sd, databuf, datalen);
	if(readOut < 0)
	{
		perror("Reading datagram message error");
		close(sd);
		exit(1);
	}
	else
	{ 
		/* HERE BEGINS THE MANIPULATING OF DATA... */
		
		printf("Reading datagram message...OK.\n");
		printf("\nThe message from multicast server is: \n");
		hexdump(databuf, 128); // This is to print out the Data Package
		printf("\n");
		printf("\n");

		// Declare and Initialize the iapp Pointer to the 'original Buffer'
		struct iapp *iappPtr = (struct iapp*)databuf;

		/*     	Check for General Version and Type of Packet	*/
		printf("General Version: (%u)\n", iappPtr->general_version);
		switch(iappPtr->general_type)
		{
			case ANNOUNCE_RESPONSE:
	                printf("General Type: Announce Request (%u)\n", iappPtr->general_type);
			break;
			case HANDOVER_REQUEST:
			printf("General Type: Handover Response  (%u)\n", iappPtr->general_type);
			break;
			default:
			printf("Unknown Data Packet!\n");

		}
		printf("\n");
		/*	IDEA	*/
		/*	remove the first two bytes from the databuf and assign it to a new variable,
		 *	then use the new variable to be pointed at.	*/

//		char databuf_modified[128];
//		memcpy(databuf_modified, databuf+2, datalen-2);
		
//		struct tag_len_val *p = (struct tag_len_val*)databuf_modified;
		
		/*	   Let's See the databuf_modified  	*/
		printf("Protocol Data Units: \n");
		printf("\n");	
		/*Loop through the buffer -beginning from the third Index of buffer-*/
		//int varlen;
		unsigned char *bytep = databuf+2; /* skip version + type field sven */
		hexdump(bytep, 128);
		printf("\n");
		struct tag_len_val *p =(struct tag_len_val *)bytep;
		for(int i=0; (i< datalen) && (p->length != 0) ;)
		{
			p = (struct tag_len_val *)bytep; /* sven */
			bytep += 3 + p->length;

			/*	Modify the new Buffer	*/
			//memcpy(databuf_modified, databuf+2+i, datalen-2-i);
        	        //struct tag_len_val *p = (struct tag_len_val*)databuf_modified;

			/*	Print the Members of struct	*/
			/*	1.Check for every standard Type 
			 *	2.Print its name	*/
			
			switch(p->type)
			{
				case TYPE_NETWORK_NAME:
				if(p->length == 0)
				{
				printf("the Rest is unknown\n");
				break;
				}
				printf("- Network Name : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : %s - ", p->value);
				printhexvalue(p->value, p->length);
				printf("\n");
					break;

				case TYPE_BSSID:
				printf("- BSSID : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
					break;

				case TYPE_OLD_BSSID:
				printf("- Old BSSID : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
					break;

				case TYPE_MOBILE_STATION_ADDRESS:
				printf("- Mobile Station Address : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
					break;
				
				case TYPE_MESSAGE_ID:
				printf("- Message ID : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
					break;
			
				case TYPE_CAPABILITIES:
                                printf("- Capabilities : (%u) \n", p->type);
                                printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
                                        break;

                                case TYPE_ANNOUNCE_INTERVAL:
                                printf("- Announce Interval : (%u) \n", p->type);
                                printf("- Length is : %u \n", p->length);
				printf("- Value : (%u) seconds - ", ntohs(*(uint16_t *)(p->value)));
				printhexvalue(p->value, p->length);
				printf("\n");
				        break;

                                case TYPE_HANDOVER_TIMEOUT:
                                printf("- Handover Timeout : (%u) \n", p->type);
                                printf("- Length is : %u \n", p->length);
				printf("- Value : (%u) Kus - ", ntohs(*(uint16_t *)(p->value)));
				printhexvalue(p->value, p->length);
				printf("\n");
                                        break;

				
				case TYPE_PHY_TYPE:
				printf("- Phy Type : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
					break;

				case TYPE_REGULATORY_DOMAIN:
				printf("- Regulatory Domain : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
					break;

				case TYPE_RADIO_CHANNEL:
				printf("- Radio Channel : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
					break;

				case TYPE_BEACON_INTERVAL:
				printf("- Beacon Interval : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : (%u) Kus - ", ntohs(*(uint16_t *)(p->value)));
				printhexvalue(p->value, p->length);
				printf("\n");
					break;

				case TYPE_OUI_IDENTIFIER:
				printf("- OUI Identifier : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
					break;
				default:
					printf("Unknown PDU Type : (%u)\n", p->type);

			}
				/*	Shifting the address for i	*/
				i += sizeof(tag_len_val) + (p->length); //next beginning
							
				printf("\n");

				/*	Shifting the address for p	*/
		}
	
	}
	return 0;
}
