all: server.run client.run
server.run:	server.c
		gcc server.c -lpthread -o server.run
client.run:	client.c
		gcc client.c -o client.run
clean:
		rm *.o *.run
