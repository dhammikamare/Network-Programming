/* 
 * TCP server for FTP
 * @author: Department of Computer Engineering, University of Peradeniya
 * @modified-by: Dhammika Marasinghe | https://github.com/dhammika-marasinghe
 * @task: A server similar to an FTP server which sends a file over a network using a TCP connection along with its client.
 *      The client and the server follow the following protocol:
 *      The client connects and sends 'request' string, requesting the file '<file_mame>' from the server.
 *      Server responds with the size of the file.
 *      Server then sits in a loop, sending the file, 1000 bytes at a time. 
 *      Upon receiving each 1000 bytes, the client appends the received part to a file called '<file_mame>'. 
 *      The client disconnects upon receiving the full file. 
 */

#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* no. of bytes per part */
#define SIZE 1000

int main(int argc, char**argv) {
    FILE *reqFile;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    char buffer[SIZE];
    int listenfd, connfd, n, index = 1, i = 0, chr;

    /* one socket is dedicated to listening */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    /* initialize a sockaddr_in struct with our own address information for
    binding the socket */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(32000);

    /* binding */
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

    /* listen for any incoming connections. */
    listen(listenfd, 0);
    clilen = sizeof (cliaddr);

    /* accept the client with a different socket. */
    connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen); // the uninitialized cliaddr variable is filled in with the

    /* getting client request */
    n = recvfrom(connfd, buffer, SIZE, 0, (struct sockaddr *) &cliaddr, &clilen); // information of the client by recvfrom function
    buffer[n] = 0;
    printf("requested:%s\n", buffer);

    /* open requested file */
    reqFile = fopen(buffer, "r");
    if (reqFile == NULL) {
        char* errorMsg = "error opening file.\n";
        printf("%s", errorMsg);
        // sendto(connfd, errorMsg, strlen(errorMsg), 0, (struct sockaddr *) &cliaddr, sizeof (cliaddr));
    }

    /* find file length */
    fseek(reqFile, 0, 2);
    long int size = ftell(reqFile);
    printf("%s size: %ld\n", buffer, size);

    /* calculate no. of serves and send it to client */
    int serves = (size / SIZE) + 1;
    printf("serves: %d\n", serves);
    sprintf(buffer, "%d\n", serves);
    sendto(connfd, buffer, sizeof (int), 0, (struct sockaddr *) &cliaddr, sizeof (cliaddr));

    rewind(reqFile);

    /* sending file to client */
    while (1) {
        chr = fgetc(reqFile);
        buffer[i++] = chr;
        if (chr == EOF) { // last part of file
            buffer[--i] = 0;
            //printf("sending: %s\n", buffer);
            sendto(connfd, buffer, strlen(buffer), 0, (struct sockaddr *) &cliaddr, sizeof (cliaddr));
            break;
        }
        if (ftell(reqFile) == (index * SIZE)) { // send by part
            //printf("sending: %s\n", buffer);
            sendto(connfd, buffer, strlen(buffer), 0, (struct sockaddr *) &cliaddr, sizeof (cliaddr));
            i = 0;
        }
    }
    fclose(reqFile);

    return 0;
}