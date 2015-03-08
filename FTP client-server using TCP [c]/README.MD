# FTP Client-Server using TCP

A server similar to an FTP server which sends a file over a network using a TCP connection along with its client.

The client and the server follow the following protocol:

The client connects and sends 'request' string, requesting the file '<file_mame>' from the server.

Server responds with the size of the file.

Server then sits in a loop, sending the file, 1000 bytes at a time. 

Upon receiving each 1000 bytes, the client appends the received part to a file called '<file_mame>'. 

The client disconnects upon receiving the full file. 