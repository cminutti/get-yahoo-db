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
 * Strings like PHP functions
 *
 */
 
#ifndef _MYSTRING_H_
#define _MYSTRING_H_ 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

char **explode(char *, char *);
int count(char **);
int sort_strings(const void *, const void *);
int array_unique(char **, unsigned);
char *strtolwr(char *);
char *strtoupr(char *);


char *_EOSA_ = "__EOSA__"; // end of strings array

#define NMAX 1000
char **explode(char *string, char *delim) {

	char *token_array[NMAX], **temp;
	unsigned short n=0, m=0;
	
	temp=NULL; // for realloc=malloc
	do {
		n=0;
		
		if(m==0) token_array[n++] = strtok(string, delim);
		while( ((token_array[n++] = strtok(NULL, delim)) != NULL) && n<NMAX ) ;
	
		if(token_array[n-1]==NULL) token_array[n-1] = _EOSA_; 
		
		m += n;
				
		temp = (char **)xrealloc(temp, sizeof(char *) * m); // asign memory for pointers
		memcpy(temp+(m-n), token_array, sizeof(char *) * n); // save pointers 

	} while(strcmp(token_array[n-1], _EOSA_));


	return temp;

}

int count(char **strings) {

	int i=0;
	
	while(strcmp(strings[i++], _EOSA_)!=0);

	return (i-1);
}




int array_unique(char **strings, unsigned n) {
	
	unsigned i, j=0;

	qsort(strings, n, sizeof(strings[0]), sort_strings);
	for(i=1; i<n; i++) 
		if( strcmp(strings[i-1], strings[i])!=0 ) strings[j++]=strings[i-1];
		
	strings[j++] = strings[n-1];
	//strings[j] = _EOSA_;

	return j;

}





int sort_strings(const void *a, const void *b) {

	char *s1 = *(char **)a;
	char *s2 = *(char **)b;

	return (-1)*strcmp(s1, s2);

}



char *strtolwr(char *string) {

	int i, len = strlen(string);
	
	for(i=0; i<len; i++) 
		string[i]=tolower(string[i]);

	return string;
}




char *strtoupr(char *string) {

	int i, len = strlen(string);
	
	for(i=0; i<len; i++) 
		string[i]=toupper(string[i]);

	return string;
}

#endif
