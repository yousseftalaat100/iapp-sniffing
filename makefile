all:	mcmclients.c mcclientsender.c
	gcc -g mcmclients.c -Wall -O1 -o mcmclients
	gcc -g mcclientsender.c -Wall -O1 -o mcclientsender

sniffer:	mcmclients.c
	gcc -g mcmclients.c -Wall -O1 -o mcmclients && ./mcmclients

sender:	mcclientsender.c
	gcc -g mcclientsender.c -Wall -O1 -o mcclientsender && ./mcclientsender $(type) $(traffic) $(dest)

clean:
	rm mcmclients mcclientsender
