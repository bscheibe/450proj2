/* sender.c */ 
/* Programmed by Adarsh Sethi */
/* Sept. 13, 2018 */

#include <message.h>

int main(void) {

   int sock_client;  /* Socket used by client */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned short client_port;  /* Port number used by client (local port) */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   char sentence[STRING_SIZE];  /* send message */
   char modifiedSentence[STRING_SIZE]; /* receive message */
   short msg_len;  /* length of message */
   short ack_num; /* Ack number of our returned message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */

   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("Client: can't open datagram socket\n");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information
            unless you want to specify a specific local port.
            The local address initialization and binding is done automatically
            when the sendto function is called later, if the socket has not
            already been bound. 
            The code below illustrates how to initialize and bind to a
            specific local port, if that is desired. */

   /* initialize client address information */

   client_port = 0;   /* This allows choice of any available local port */

   /* Uncomment the lines below if you want to specify a particular 
             local port: */
   /*
   printf("Enter port number for client: ");
   scanf("%hu", &client_port);
   */

   /* clear client address structure and initialize with client address */
   memset(&client_addr, 0, sizeof(client_addr));
   client_addr.sin_family = AF_INET;
   client_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* This allows choice of
                                        any host interface, if more than one 
                                        are present */
   client_addr.sin_port = htons(client_port);

   /* bind the socket to the local client port */

   if (bind(sock_client, (struct sockaddr *) &client_addr,
                                    sizeof (client_addr)) < 0) {
      perror("Client: can't bind to local address\n");
      close(sock_client);
      exit(1);
   }

   if ((server_hp = gethostbyname("cisc450.cis.udel.edu")) == NULL) {
      perror("Client: invalid server hostname\n");
      close(sock_client);
      exit(1);
   }
   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof("cisc450.cis.udel.edu"));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
   server_addr.sin_port = htons(65006);

   /* user interface */

   printf("Please input a sentence:\n");
   scanf("%s", sentence);
   msg_len = strlen(sentence) + 1;

   /* send message */

   line = (message *) malloc(sizeof(message));
   message->count = msg_len;
   message->seqNum = 1;
   strcpy(message->data, sentence);
   bytes_sent = sendto(sock_client, line, sizeof(line), 0,
            (struct sockaddr *) &server_addr, sizeof (server_addr));

   /* get response from server */
  
   printf("Waiting for response from server...\n");
   bytes_recd = recvfrom(sock_client, ack_num, sizeof(short), 0,
                (struct sockaddr *) 0, (int *) 0);
   printf("\nThe response from server is:\n");
 //  printf("%s\n\n", modifiedSentence);

   /* close the socket */

   close (sock_client);
}
