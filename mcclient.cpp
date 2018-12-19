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

#define BUFFER_SIZE 256

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

struct protocol_data
{
	char *SSID;
	char *BSSID;
	char *Capabilities;
	char *Announce_Interval;
	char *Handover_Timeout;
	char *PHY_Type;
	char *Regulatory_Domain;
	char *Radio_Channel;
	char *Beacon_Interval;
	char *OUI_Identifier;

	struct tag_len_val *tlv;
	
}__attribute__((packed));

struct iapp
{
	uint8_t general_version;
	uint8_t general_type;
	
//	char *SSID;
//	char *BSSID;
//	char *Capabilities;
//	char *Announce_Interval;
//	char *Handover_Timeout;
//	char *PHY_Type;
//	char *Regulatory_Domain;
//	char *Radio_Channel;
//	char *Beacon_Interval;
//	char *OUI_Identifier;
	
	struct tag_len_val *tlv;
	struct protocol_data *dataframe;
	
}__attribute__((packed));

/* another way to -fill the buffer- to send it
	p = alloc_iapp_msg();
	add_iapp_network_name(p, "ssid");
	add_iapp_radio_channel(p, 6);
*/
// malloc a buffer (using calloc for arrays)
unsigned char* alloc_iapp_msg(unsigned char *buffer, uint8_t bufferlen)
{
	buffer = (unsigned char*)calloc(bufferlen, sizeof(unsigned char));
	return buffer;
}
void add_iapp_ver_type(struct iapp* iappPtr, unsigned char* sent_buffer)
{
	if(iappPtr->general_version == GENERAL_VERSION)
		sent_buffer[0] = iappPtr->general_version;
	if(iappPtr->general_type == ANNOUNCE_RESPONSE)
                sent_buffer[1] = iappPtr->general_type;
	if(iappPtr->general_type == HANDOVER_REQUEST)
		sent_buffer[1] = iappPtr->general_type;
}
void* add_iapp_SSID(struct protocol_data* dataframe, struct tag_len_val* tlvPtr, unsigned char* sent_buffer)
{
	if(tlvPtr->type == TYPE_NETWORK_NAME)
	{

	/* assign the tlv pointer to the members of iapp structure*/
	dataframe->SSID[0] = tlvPtr->type;
	dataframe->SSID[1] = tlvPtr->type_option;
	dataframe->SSID[2] = tlvPtr->length;
	unsigned char* destPtr= (unsigned char*) memcpy((unsigned char*)dataframe->SSID[3], (const unsigned char*)tlvPtr->value, tlvPtr->length);
	
	int dataframelen = tlvPtr->length + 3;

		for(int i=0; i<dataframelen; i++)
		{
			sent_buffer[i] = dataframe->SSID[i];
		}
	destPtr++;
	sent_buffer = destPtr;
	return sent_buffer;
	}else
	{return NULL;}

}
void* add_iapp_BSSID(struct protocol_data* dataframe, struct tag_len_val* tlvPtr, unsigned char* sent_buffer)
{
	if(tlvPtr->type == TYPE_BSSID)
        {
        /* assign the tlv pointer to the members of iapp structure*/

        dataframe->BSSID[0] = tlvPtr->type;
        dataframe->BSSID[1] = tlvPtr->type_option;
        dataframe->BSSID[2] = tlvPtr->length;
        memcpy((void*)dataframe->BSSID[3], tlvPtr->value, tlvPtr->length);
	
        int dataframelen = tlvPtr->length + 3;

	        for(int i=0; i<dataframelen; i++)
        	{
                	sent_buffer[i] = dataframe->BSSID[i];
	        }
	return sent_buffer;
        }else
	{return NULL;}
}
void* add_iapp_Capabilities(struct protocol_data* dataframe, struct tag_len_val* tlvPtr, unsigned char* sent_buffer)
{
        if(tlvPtr->type == TYPE_CAPABILITIES)
        {
        /* assign the tlv pointer to the members of iapp structure*/

        dataframe->Capabilities[0] = tlvPtr->type;
        dataframe->Capabilities[1] = tlvPtr->type_option;
        dataframe->Capabilities[2] = tlvPtr->length;
        memcpy((void*)dataframe->Capabilities[3], tlvPtr->value, tlvPtr->length);
        int dataframelen = tlvPtr->length + 3;

        	for(int i=0; i<dataframelen; i++)
        	{
                	sent_buffer[i] = dataframe->Capabilities[i];
	        }
	return sent_buffer;
        }else
	{return NULL;}
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
	}else
	{
		printf("Adding multicast group...OK.\n");
	}
	
	/* Read from the socket */
	datalen = sizeof(databuf);
	//datalen = sizeof(databuf); // with normal unsigned char buffer of 128 bytes
	int readoutlen = read(sd, databuf, datalen);
	printf("-- Message Length : %d\n", readoutlen);
	if(readoutlen < 0)
	{
		perror("Reading datagram message error");
		close(sd);
		exit(1);
	}
	else
	{
	       /**********************************************/	
		/* HERE BEGINS THE MANIPULATING OF DATA... */
		
		/* Send to the proper port number with the IP address
		specified as Multicast IP Address to THIS -->> 224.0.1.76 */
		memset((char *) &dest_addr, 0, sizeof(dest_addr));
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(2313);
		dest_addr.sin_addr.s_addr= inet_addr("224.0.1.76");
		
		printf("Reading datagram message...OK.\n");
		//printf("Got Data Packet from %s\n", inet_ntoa(dest_addr));
		printf("\nThe message from multicast server is: \n");
		hexdump(databuf, readoutlen); // This is to print out the Data Package
		printf("\n\n");

		
		// 1. Define and Initialize the iapp Pointer to the 'original Buffer'
		struct iapp *iappPtr = (struct iapp*)databuf;
		
		// 2.(a) Define another databuffer (*bytep) shifted by two
		unsigned char *bytep = databuf+2; /* skip version + type field sven */
		// 2.(b) Define and Initialize the tlv Pointer to the 'modified Buffer'
		struct tag_len_val *p =(struct tag_len_val *)bytep;	
		


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
			/*	skip the first two bytes from the databuf   */

		
		/*	   Let's See the databuf_modified  	*/
		printf("Protocol Data Units: \n");
		printf("\n");	
		
		/*Loop through the buffer -beginning from the third Index of buffer-*/
		//unsigned char *bytep = databuf+2; /* skip version + type field sven |MOVED UP| */ 
		//hexdump(bytep, datalen);
		//printf("\n");

		//alloc_iapp_msg();

		uint8_t dataframeslength=0;
		uint8_t no_of_elements = 0;

		for(int i=0; (i< datalen) && (p->length != 0) ;)
		{
			p = (struct tag_len_val *)bytep; /* sven */
			/*	Shifting the address for p to next PD Member	*/
			bytep += 3 + p->length;

			/*	Print the Members of struct	*/
			/*	1.Check for every standard Type 
			 *	2.Print its name	
			 *	3.Send the specified data frame */
			
			switch(p->type)
			{
				case TYPE_NETWORK_NAME:
				if(p->length == 0)
				{
				printf("-- End of Data Packet --\n");
				break;
				}
				printf("- Network Name : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : %s - ", p->value);
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;

				case TYPE_BSSID:
				printf("- BSSID : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;

				case TYPE_OLD_BSSID:
				printf("- Old BSSID : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;

				case TYPE_MOBILE_STATION_ADDRESS:
				printf("- Mobile Station Address : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;
				
				case TYPE_MESSAGE_ID:
				printf("- Message ID : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;
			
				case TYPE_CAPABILITIES:
                                printf("- Capabilities : (%u) \n", p->type);
                                printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
                                        break;

                                case TYPE_ANNOUNCE_INTERVAL:
                                printf("- Announce Interval : (%u) \n", p->type);
                                printf("- Length is : %u \n", p->length);
				printf("- Value : (%u) seconds - ", ntohs(*(uint16_t *)(p->value)));
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
				        break;

                                case TYPE_HANDOVER_TIMEOUT:
                                printf("- Handover Timeout : (%u) \n", p->type);
                                printf("- Length is : %u \n", p->length);
				printf("- Value : (%u) Kus - ", ntohs(*(uint16_t *)(p->value)));
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
                                        break;

				
				case TYPE_PHY_TYPE:
				printf("- Phy Type : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;

				case TYPE_REGULATORY_DOMAIN:
				printf("- Regulatory Domain : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;

				case TYPE_RADIO_CHANNEL:
				printf("- Radio Channel : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;

				case TYPE_BEACON_INTERVAL:
				printf("- Beacon Interval : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : (%u) Kus - ", ntohs(*(uint16_t *)(p->value)));
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;

				case TYPE_OUI_IDENTIFIER:
				printf("- OUI Identifier : (%u) \n", p->type);
				printf("- Length is : %u \n", p->length);
				printf("- Value : ");
				printhexvalue(p->value, p->length);
				printf("\n");
				dataframeslength += p->length;
				no_of_elements +=3;
					break;
				default:
					printf("Unknown PDU Type : (%u)\n", p->type);
					dataframeslength += p->length;
					no_of_elements +=3;

			}
				/*	Shifting the address for i	*/
				i += sizeof(tag_len_val) + (p->length); //next beginning
							
				printf("\n");

		}
		
		/* Manipulate the databuffer to get it sending exactly what you want */
		/* Now I have the total length of values [in bytes],
		/  but how many [bytes are in the whole Packet]*/
		
	//	uint8_t packetlength = dataframeslength + no_of_elements +2;
		
		/* allocate buffer->add bytes in buffer->
		 * remember the buffer name?->got the new buffer length?->
		 * ---pass it to sendto()--- */

		//p = alloc_iapp_msg();
	        //add_iapp_network_name(p, "Test1");
        	//add_iapp_radio_channel(p, 6);
	
		/*  Send whole data buffer to the Server (Source 'AP') with MULTICAST IP 224.0.1.76  */
		if(sendto(sd, databuf, readoutlen, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr))< 0)

		{
		perror("Sending Error");
		close(sd);
		exit(1);
		}
		else
		{
		printf("The Transmitted Data is: \n");
		hexdump(databuf, readoutlen);	
		printf("\n");
		}
	}
	return 0;
}
