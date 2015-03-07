/* 
 * UDP echo server 
 * @author: Department of Computer Engineering, University of Peradeniya
 * @modified-by: Dhammika Marasinghe | https://github.com/dhammika-marasinghe
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>

int main(int argc, char**argv) {
    int sockfd, n;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char mesg[1000];

    // create a socket with appropriate options for UDP.
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // initialize a sockaddr_in struct with appropriate options for our server address.
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(32000);

    // bind the socket with the created sockaddr_in struct.
    bind(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));

    // indefinitely, replying for all clients until it is forced to terminate by the host computer.
    while (1) {
        // send and receive messages from clients that sockaddr_in struct is configured to use.
        len = sizeof (cliaddr);
        n = recvfrom(sockfd, mesg, 1000, 0, (struct sockaddr *) &cliaddr, &len);
        mesg[n] = 0;
        printf("S: Received: %s\n", mesg);

        sendto(sockfd, mesg, n, 0, (struct sockaddr *) &cliaddr, sizeof (cliaddr));
    }
    return 0;
}
