all:	mcmclients.c mcclientsender.c
	g++ -g mcmclients.c -Wall -O2 -o mcmclients
	g++ -g mcclientsender.c -Wall -O2 -o mcclientsender

sniffer:	mcmclients.c
	g++ -g mcmclients.c -Wall -O2 -o mcmclients && ./mcmclients

sender:	mcclientsender.c
	g++ -g mcclientsender.c -Wall -O2 -o mcclientsender && ./mcclientsender

clean:
	rm mcmclients mcclientsender
