#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstdio>

#define DATA_BUFFER_SIZE 1494
#define COMM_PORT "4950"
#define w 400
#define RECEIVER_IP "127.0.0.1"
// #define RECEIVER_IP "10.100.10.214"
#define SIZE_OF_MATRIX(X) ((X.total())*(X.elemSize()))


int send_data(const char* const ip_address, void* data, size_t sz)
{

  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;
  unsigned int ack;
  size_t ack_size;

  unsigned char packt_data[1496];
  size_t i=0;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(ip_address, COMM_PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("talker: socket");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "talker: failed to create socket\n");
    return 2;
  }

  if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
    perror("client: connect");
    close(sockfd);
  }

  while(sz > 0)
  {
    if (sz > DATA_BUFFER_SIZE)
    {
      if ((numbytes = send(sockfd, data, DATA_BUFFER_SIZE, 0)) == -1) {
        perror("talker: sendto");
        return(1);
      }


    }
    else
    {
      if ((numbytes = send(sockfd, data, sz, 0)) == -1) {
        perror("talker: send..");
        return(1);
      }
      break;
    }

    // wait(1);

    if ((numbytes = recv(sockfd, (void*)&ack, 4 , 0)) == -1)
    {
        perror("talker: recv..");
        return(1);
    }
    else
    {
      if(ack == (unsigned int)sz)
      {
        data = ((unsigned char*)data) + DATA_BUFFER_SIZE;
        sz = sz - DATA_BUFFER_SIZE;

        if ((numbytes = send(sockfd, (void*)&sz, 4 , 0)) == -1)
        {
            perror("talker: send..");
            return(1);
        }

      }
    }
  }

  freeaddrinfo(servinfo);
  close(sockfd);
}

int main ( void ){
	// char atom_window[] = "Drawing: Atom, Sender";
	// Mat atom_image = Mat::zeros( w, w, CV_8UC3 );
  // Mat atom_image = Mat(w, w, CV_8UC3, cv::Scalar(255,255,255));
  size_t i;
  // MatIterator_<Vec3b> it, end;

  unsigned char s[480000];

  for(i=0;i<(sizeof s);i++)
  {
    s[i]=i;
  }
  
  send_data(RECEIVER_IP,&s,sizeof s);

  // std::cout << SIZE_OF_MATRIX(atom_image) << std::endl;
  // printf("%s\n", );

	// imshow( atom_window, atom_image );
  	// moveWindow( atom_window, 0, 200 );

	// waitKey( 0 );
	return(0);
}
