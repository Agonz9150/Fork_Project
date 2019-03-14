#Alexis Gonzalez
CC = gcc
Program = SharedProcessMain.c SharedProcessFunctions.c -o SharedProcess

Build:
	$(CC) $(Program)
