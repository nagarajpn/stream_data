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

#include <iostream>
// #include "comm.h"

#define DATA_BUFFER_SIZE 1494
#define w 400
#define SIZE_OF_MATRIX(X) ((X.total())*(X.elemSize()))
#define COMM_PORT "4950"

int receive_data(void* data, size_t sz)
{
	int sockfd,new_fd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int rv;
	int numbytes;
	socklen_t addr_len;
	unsigned int ack;
	size_t ack_size;

	unsigned char packt_data[1496];
	// MatIterator_<Vec3b> it, end;
	size_t i=0;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP..

	if ((rv = getaddrinfo(NULL, COMM_PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}
		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;

	if (listen(sockfd, 1) == -1) {
		perror("listen");
		exit(1);
	}

	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

	// for( it = img.begin<Vec3b>(), end = img.end<Vec3b>(); it != end; ++it)
	// {
	// 	if ((i == 0) || (i > (DATA_BUFFER_SIZE-3)))
	// 	{
	// 		if ((numbytes = recv(new_fd, packt_data, DATA_BUFFER_SIZE , 0)) == -1) {
	// 			perror("recvfrom");
	// 			exit(1);
	// 		}
	// 		i=0;
	// 	}
	// 	// printf("i: %d\n",i);

	// 	(*it)[0] = packt_data[i++];
	// 	(*it)[1] = packt_data[i++];
	// 	(*it)[2] = packt_data[i++];
	// }

	while (sz>0)
	{
		if (sz>DATA_BUFFER_SIZE)
		{
			if ((numbytes = recv(new_fd, data, DATA_BUFFER_SIZE , 0)) == -1) {
				perror("recvfrom");
				exit(1);
			}
			else if(numbytes == 0){
				perror("Sender disconnected");
				//exit(1);
			}
			else{
				if(numbytes < DATA_BUFFER_SIZE)
				{
					// continue;
				}
			}
		}
		else
		{
			if ((numbytes = recv(new_fd, data, sz , 0)) == -1) {
				perror("recvfrom");
				exit(1);
			}
			else if(numbytes == 0){
				perror("Sender disconnected");
				//exit(1);
			}
			else{
				if(numbytes < DATA_BUFFER_SIZE)
				{
					// continue;
				}
			}
			break;
		}

	    if ((numbytes = send(new_fd, (void*)&sz, 4 , 0)) == -1)
	    {
	        perror("receiver: send..");
	        return(1);
	    }
	    else
	    {

			// if ((numbytes = recv(new_fd, (void*)&ack, 4 , 0)) == -1) {
			// 	perror("recvfrom");
			// 	exit(1);
			// }
			// else{
			// 	if(ack == (sz - DATA_BUFFER_SIZE))
			// 	{
					data = ((unsigned char*)data) + DATA_BUFFER_SIZE;
					sz = sz - DATA_BUFFER_SIZE;
			// 	}
			// }

	    }
	}

  	printf("listener: packet is %d bytes long\n", numbytes);

	close(new_fd);
	close(sockfd);
}

int main ( void ){

	// FILE *fptr;
	// Mat atom_image = Mat::zeros( w, w, CV_8UC3 );
	unsigned char r[480000];
	size_t i;

	// fptr = fopen("out.txt", "w");

	// std::cout << "Rows: "<< atom_image.rows << std::endl;
	// std::cout << "Cols: "<< atom_image.cols << std::endl;
	// std::cout << "Size: "<< atom_image.size() << std::endl;

  	// if (!atom_image.isContinuous()) {
   //    atom_image = atom_image.clone();
  	// }

  	// receive_image(atom_image);

	// receive_data(atom_image.data,SIZE_OF_MATRIX(atom_image));
	receive_data(&r,sizeof(r));

	// 	std::cout << "Rows: "<< atom_image.rows << std::endl;
	// std::cout << "Cols: "<< atom_image.cols << std::endl;
	// std::cout << "Size: "<< atom_image.size() << std::endl;

	// std::cout <<"The value of r: " << r << std::endl;

	// for(i=0;i<(sizeof r);i++)
	// {
	// 	fprintf(fptr,"r[%d]: %d\n",i,r[i]);
	// }

	for(i=0;i<(sizeof r);i++)
	{
		if(r[i] != (unsigned char)i)
		{
			printf("Mismatch at index: %d\n",i);
			break;
		}
	}

	if (i==(sizeof r))
		printf("Sent and received arrays are equal\n");

	// imshow( atom_window, atom_image );
 //  	moveWindow( atom_window, 0, 200 );

	// waitKey( 0 );
	return(0);
}