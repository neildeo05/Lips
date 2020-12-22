PKGFLAGS=-ledit -lm
CFLAGS= -std=c99 -Wall
#gcc -std=c99 -Wall main.c -ledit -o main
parsing parsing.c mpc.c:
	$(CC) $(CFLAGS) parsing.c mpc.c $(PKGFLAGS) -o parsing
