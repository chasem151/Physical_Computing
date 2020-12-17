all: webserv my-histogram authentication

webserv: webserv.c
		gcc -o webserv webserv.c

my-histogram: my-histogram.c
		gcc -o my-histogram my-histogram.c

authentication: authentication.c
		gcc -o authentication authentication.c

clean:
		rm -rf *~ webserv my-histogram authentication
