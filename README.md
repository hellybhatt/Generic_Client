Generic_Client
TCP and UDP Generic Client Code in C Language(Linux). To run this program, we will need following four parameters in order:

-m (message) - Text message which is to be send to server

-h (IP Address/ DNS server name) - Ip address or domain name of the server i.e host name

-s (service name) - Service name to fetch the port number 

-p (protocol) - This option will only contain two parameters, TCP or UDP

Here we need to use getservbyname() to get port number from service name and the protocol.
Also, we will use gethostbyname() to get convert host name into IP address(32 Bit accepted format)
