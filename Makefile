PKGFLAGS=-ledit -lm
CFLAGS= -std=c99 -Wall

tests:
	./parsing

parsing parsing.c mpc.c:
	$(CC) $(CFLAGS) parsing.c mpc.c $(PKGFLAGS) -o parsing
