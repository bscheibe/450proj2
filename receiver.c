/* receiver.c */
/* Programmed by Adarsh Sethi */
/* Nov. 13, 2018 */

#include "message.h"

int main(void) {

   int sock_server;

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   char sentence[STRING_SIZE];  /* receive message */
   short ack; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */

   /* open a socket */

   if ((sock_server = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("Server: can't open datagram socket\n");
      exit(1);
   }

   /* initialize server address information */

   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */
   server_port = SERV_UDP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address\n");
      close(sock_server);
      exit(1);
   }

   /* wait for incoming messages in an indefinite loop */

   printf("Waiting for incoming messages on port %hu\n\n", 
                           server_port);

   client_addr_len = sizeof (client_addr);

   struct message * msg = malloc(sizeof(struct message));
   char * line = NULL;
   FILE *fp;
   fp = fopen("output.txt", "w");
   if (!fp) {
    puts("open error");
   }

   for (;;) {

      bytes_recd = recvfrom(sock_server, msg, sizeof(*msg), 0,
                     (struct sockaddr *) &client_addr, &client_addr_len);
      printf("Packet %d received with %d data bytes\n\n",
                        msg->seqNum, bytes_recd);
      if (!msg->count) {
        if (fclose(fp) < 0)
          puts("close error");
        break;
      }
      msg->data[msg->count] = '\0';
      fputs(msg->data, fp);

      /* send message */
      short shsize = sizeof(short);
      short seq = msg->seqNum;

      bytes_sent = sendto(sock_server, &seq, sizeof(short), 0,
               (struct sockaddr*) &client_addr, client_addr_len);
   }
}
