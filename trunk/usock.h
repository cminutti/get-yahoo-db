/*
 * Get Yahoo Finance Database
 *
 *   Copyright (C) 2012 Carlos Minutti <cminutti@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */
 
/*
 * Socket functions
 *
 */

#ifndef _MYSOCK_H_
#define _MYSOCK_H_ 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

#ifndef _WINDOWS_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> // gethostbyname

#else

#include <winsock.h>
#include <winsock2.h>

#endif


char *fsock(char *, unsigned short, char *);
char *htmlrequest(char *, char *, int, char **);
void h_error(char *);


// READSIZE <= MAXBUFFSIZE
#define MAXBUFFSIZE 1024*80
#define READSIZE 512
char *fsock(char *host, unsigned short port, char *request) {

	unsigned fd, ncars;
	unsigned long len, templen;
	struct hostent *hostip;
	struct sockaddr_in remote_socket;
	char buffer[MAXBUFFSIZE], *p;
	
	if( (hostip = gethostbyname (host)) == NULL ) {
		h_error(" gethostbyname() ");
		exit(1);		 
	}
	
	remote_socket.sin_family  = AF_INET;
	remote_socket.sin_port = htons(port);
	remote_socket.sin_addr = *((struct in_addr *)(hostip->h_addr));
	memset(&remote_socket.sin_zero, 0, 8); // fill
	
	if( (fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
		perror(" socket() ");
		exit(1);	
	}
	
	if ( connect(fd, (struct sockaddr *) &remote_socket, sizeof(remote_socket)) == -1 ) {
		perror(" connect() ");
		exit(1);
	}
	

	ncars=0;
	len = strlen(request);
	while( (ncars+=send(fd, request+ncars, len-ncars, 0)) < len ) ; // when no write all bytes

	p = NULL; // realloc need it for the first iteration (functions like malloc)
	len = templen = 0;
	do {
	
		while( ((len-templen)+READSIZE<=MAXBUFFSIZE) && (ncars=recv(fd, buffer+(len-templen), READSIZE, 0)) ) 
			len+=ncars;

		p = (char *)xrealloc(p, len+1);
		memcpy(p+templen, buffer, len-templen);
		templen=len;

	} while(ncars);

	p[len]='\0';

	close(fd);
	return (p);
	
}


char *htmlrequest(char *host, char *file, int port, char **complete_reply) {

	char *request, *data;
	short aux = strlen("GET / HTTP/1.0\r\n\r\n");
	//\r\nHost: \r\nConnection: close

	request = (char *)xmalloc(aux + strlen(file) + 1);
	sprintf(request, "GET /%s HTTP/1.0\r\n\r\n", file);
	
	data = strstr(*complete_reply=fsock(host, port, request), "\r\n\r\n") + 4;
	free(request);
	
	return data;
	
}

void h_error(char *msg) {

	char *buffer[] = { "Host not found", "Try again", "No recovery", "No address" };
	short i;
	
	if(h_errno==HOST_NOT_FOUND) i=0;
	else if(h_errno==TRY_AGAIN) i=1;
	else if(h_errno==NO_RECOVERY) i=2;
	else if(h_errno==NO_ADDRESS) i=3;
	
	printf("%s: %s\n", msg, buffer[i]);

}

#endif
