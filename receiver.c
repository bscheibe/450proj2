/* receiver.c */
/* Programmed by Adarsh Sethi */
/* Nov. 13, 2018 */

#include "message.h"

int main(int argc, char** argv) {

   int sock_server;

   short seq = 0;

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

   // Statistics
   int succ_packs = 0;
   int delivered_bytes = 0;
   int dup_packs = 0;
   int lost_packs = 0;
   int total_packs = 0;
   int succ_acks = 0;
   int lost_acks = 0;
   int total_acks = 0;

   // Set up inputs
   if (!argv[1] || !argv[2]) {
     puts("Must give two numbers between 0 and 1!");
     return(0);
   }
   if (atof(argv[1]) < 0  || atof(argv[1]) > 1 ||
	atof(argv[2]) < 0  || atof(argv[2]) > 1) {
     puts("Please enter numbers between 0 and 1: \n");
     return(0);
   }

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

      // Receive message
      bytes_recd = recvfrom(sock_server, msg, sizeof(*msg), 0,
                     (struct sockaddr *) &client_addr, &client_addr_len);

      // Simulate packet loss
      if (!packet_loss(atof(argv[1]))) {
        lost_packs++;
	printf("Packet %d lost\n\n", msg->seqNum);
        continue;
      }
      printf("Packet %d received with %d data bytes\n\n",
                        msg->seqNum, bytes_recd);
      succ_packs++;

      // Duplicate packet.
      if (msg->seqNum != seq) {
	printf("Duplicate packet %d receieved with %d data bytes\n\n", msg->seqNum, bytes_recd);
        sendto(sock_server, &msg->seqNum, sizeof(short), 0,
               (struct sockaddr*) &client_addr, client_addr_len);
        dup_packs++;
        continue;
      }

      // Handle final packet
      if (!msg->count) {
        if (fclose(fp) < 0)
          puts("close error");
        sendto(sock_server, &msg->seqNum, sizeof(short), 0,
               (struct sockaddr*) &client_addr, client_addr_len);
	printf("End of Transmission Packet with Sequence Number %d received with %d data bytes\n\n", msg->seqNum, bytes_recd );
        break;
      }

      msg->data[msg->count] = '\0';
      fputs(msg->data, fp);
      delivered_bytes += msg->count;

      // Simulate ACK loss
      if (!ack_loss(atof(argv[2]))) {
        printf("ACK %d Lost\n\n", seq);
        lost_acks++;
        seq = (seq+1)%2;
        continue;
      }
      else {
        printf ("ACK %d Transmitted\n\n", seq);
      }

      /* send message */
      short shsize = sizeof(short);
      seq = msg->seqNum;
      bytes_sent = sendto(sock_server, &seq, sizeof(short), 0,
               (struct sockaddr*) &client_addr, client_addr_len);
      seq = (seq+1)%2;
      succ_acks++;
 }
 total_packs = succ_packs + dup_packs + lost_packs;
 total_acks = succ_acks + lost_acks;

  printf("Number of data packets received successfully: %d\n", succ_packs);
  printf("Total number of data bytes received which are delivered to user: %d\n", delivered_bytes);
  printf("Total number of duplicate packets received: %d\n", dup_packs);
  printf("Number of data packets received but dropped due to loss: %d\n", lost_packs);
  printf("Total number of data packets received: %d\n", total_packs);
  printf("Number of ACKs transmitted without loss: %d\n", succ_acks);
  printf("Number of ACKs generated but dropped due to loss: %d\n", lost_acks);
  printf("Total number of ACKs generated: %d\n", total_acks);
}

int packet_loss(double packet_loss_rate)
{
  double x = rand() / (RAND_MAX +1. );
  printf("/ %G %G / \n", x, packet_loss_rate);
  if ( x < packet_loss_rate) {
    return 0;
  } else {
    return  1;
  }
}

int ack_loss(double ack_loss_rate)
{
  double x = rand() / (RAND_MAX +1. );
  if ( x < ack_loss_rate) {
    return 0;
  } else {
    return  1;
  }
}

