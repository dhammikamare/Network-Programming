/* 
 * UDP client 
 * @author: Department of Computer Engineering, University of Peradeniya
 * @modified-by: Dhammika Marasinghe | https://github.com/dhammika-marasinghe
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char**argv) {
    int sockfd, n;
    struct sockaddr_in servaddr;
    char sendline[] = "Hello UDP server! This is UDP client";
    char recvline[1000];
    if (argc != 2) {
        printf("usage:%s <IP address>\n", argv[0]);
        return -1;
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof (servaddr));

    // Initialize a sockaddr_in struct with the information of the server intended to connect to
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(32000);

    // send and receive messages
    sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr*) &servaddr, sizeof (servaddr));

    n = recvfrom(sockfd, recvline, 10000, 0, NULL, NULL);
    recvline[n] = 0;
    printf("Received: %s\n", recvline);

    return 0;
}