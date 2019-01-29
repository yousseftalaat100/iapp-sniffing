all:	mcmclients.c mcclientsender.c
	g++ -g mcmclients.c -Wall -O1 -o mcmclients
	g++ -g mcclientsender.c -Wall -O1 -o mcclientsender

sniffer:	mcmclients.c
	g++ -g mcmclients.c -Wall -O1 -o mcmclients && ./mcmclients

sender:	mcclientsender.c
	g++ -g mcclientsender.c -Wall -O1 -o mcclientsender && ./mcclientsender $(type) $(traffic)

clean:
	rm mcmclients mcclientsender
