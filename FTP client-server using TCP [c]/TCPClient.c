/* 
 * TCP client for FTP
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

/* no. of bytes per part */
#define SIZE 1000

int main(int argc, char**argv) {
    FILE *downFile;
    struct sockaddr_in servaddr;
    char downFileName[255];
    char buffer[SIZE];
    int sockfd, n, serves, i;

    if (argc != 3) {
        printf("usage: ./%s <ip_address> <file_name>\n", argv[0]);
        return -1;
    }

    /* socket to connect */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* IP address information of the server to connect to */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(32000);

    /* connect to the server. */
    connect(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

    /* send request */
    printf("request file: %s\n", argv[2]);
    sendto(sockfd, argv[2], strlen(argv[2]), 0, (struct sockaddr *) &servaddr, sizeof (servaddr));

    /* get number of parts */
    n = recvfrom(sockfd, buffer, sizeof (int), 0, NULL, NULL);
    buffer[n] = 0;
    sscanf(buffer, "%d", &serves);
    printf("getting %s as %d serves.\n", argv[2], serves);

    /* open file to append */
    sprintf(downFileName, "_%s", argv[2]);
    downFile = fopen(downFileName, "a");
    if (downFile == NULL) {
        printf("error opening file.\n");
    }
    /* download as parts */
    for (i = 1; i <= serves; i++) {
        //printf("getting part %d\n", i);
        n = recvfrom(sockfd, buffer, SIZE, 0, NULL, NULL);
        buffer[n] = 0;
        fprintf(downFile, "%s", buffer);
        //printf("received: %s\n", buffer);
    }
    /* finish downloading */
    fclose(downFile);
    printf("%s file successfully downloaded. :) \n", argv[2]);

    return 0;
}