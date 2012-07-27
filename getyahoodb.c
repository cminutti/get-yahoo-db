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
 * By providing a list of symbols and the desired date range, you get a 
 * database with information of adjusted closing prices of the assets in 
 * a single CSV file.
 *
 * https://launchpad.net/getyahoodb
 * https://code.google.com/p/get-yahoo-db
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ustring.h"
#include "usock.h"
#include "functions.h"

#define MAX_FILE_NAME_LEN 140
#define HOST "ichart.finance.yahoo.com"	// Download host

#ifndef _WINDOWS_

#define CLEAR "clear"
#define DIRSEP "/"
#define WINDLLINIT 
#define WINDLLCLEAR 

#else					// windows variant

#define CLEAR "cls"
#define DIRSEP "\\"
#define WINDLLINIT WSADATA wsaData; WSAStartup(MAKEWORD(1,0),&wsaData);
#define WINDLLCLEAR  WSACleanup();

#endif


int main(int argc, char *argv[]) {

	unsigned a, b, c, d, e, f;		// days, months and year fields
	char **symbols_array, *csvfile, *complete_reply;
	char ****data, **csvline_in, **fulldate, prefix[20];
	char *inputcsv = (char *) xmalloc(MAX_FILE_NAME_LEN);
	short int open = 0;					// OPEN 
	short int high = 0;					// HIGH
	short int low = 0;					// LOW 
	short int close = 0;				// CLOSE
	short int volume = 0;				// VOLUME 
	short int adjclose = 1;				// ADJCLOSE
	unsigned i, j, n, n_symbols;
	unsigned *n_lines, *k, total_nlines, max_rows;
	FILE *outputcsvfp;


	// List of symbols to get
	char *symbols;

	
	system(CLEAR);


	// help
	if (argc<4) {
		printf("Get Yahoo Finance Database\n");
		
		printf("\nUsage :    %s dd/mm/yyyy dd/mm/yyyy list [open close high low volume]\n", argv[0]);
		printf("\nExample :  %s 01/01/2011 01/01/2012 ^GSPC+^IXIC+^HSI", argv[0]);
		printf("\nExample :  %s 01/01/2011 01/01/2012 AAPL+IBM+SNE low hight", argv[0]);
		printf("\nExample :  %s 01/01/2011 01/01/2012 GOOG+YHOO+AMZN open close volume\n", argv[0]);
		
		printf("\n'list' is a list of symbols separated by '+' character.");
		printf("\nGood luck ^_^\n\n");
		
		exit(EXIT_FAILURE);
	}
	
	
	// save day, month and year
	sscanf(argv[1], "%d/%d/%d", &b, &a, &c);
	sscanf(argv[2], "%d/%d/%d", &e, &d, &f);
	
	
	// symbols to upper-case
	symbols=strtoupr(argv[3]);
	
	// check other options
	for(i=(argc-1); i>=4; i--) {
		strtolwr(argv[i]);
		if(strcmp("open", argv[i])==0) open=1;
		else if(strcmp("close", argv[i])==0) close=1;
		else if(strcmp("high", argv[i])==0) high=1;
		else if(strcmp("low", argv[i])==0) low=1;
		else if(strcmp("volume", argv[i])==0) volume=1;
	}

	// get symbols
	symbols_array = explode(symbols, "+");
	n_symbols = count(symbols_array);
	data = (char ****)xmalloc(sizeof(char *) * n_symbols);
	n_lines = (unsigned *)xmalloc(sizeof(unsigned ) * n_symbols);
	
	// for windows : initialize dll socket
	WINDLLINIT 

	// Get the csv table in a new matrix
	for(n=0; n<n_symbols; n++) {
		// The URL to download
		sprintf(inputcsv, "table.csv?s=%s&a=%d&b=%d&c=%d&d=%d&e=%d&f=%d&g=d&ignore=.csv", symbols_array[n], a-1, b, c, d-1, e, f);
		csvfile = htmlrequest(HOST, inputcsv, 80, &complete_reply);
		
		// for debugging propourse
		//printf("%s\n", csvfile);
		//sprintf(inputcsv, "table1.csv");
		//csvfile = htmlrequest("localhost", inputcsv, 80, &complete_reply);
		
		csvline_in = explode(csvfile, "\n");

		n_lines[n] = count(csvline_in);
;
		data[n] = (char ***)xmalloc(sizeof(char *) * n_lines[n]);

		for(i=0; i<n_lines[n]; i++)
			data[n][i] =  explode(csvline_in[i], ",");

		printf("%s/%s\n", HOST, inputcsv);
 		
		free(csvline_in);
	}
	
	total_nlines = 0;
	for(n=0; n<n_symbols; n++)
		total_nlines += n_lines[n];
	fulldate = (char **)xmalloc(sizeof(char *) * total_nlines);

	// Set all dates availables. First element is the name ('Date')
	j=0;
	for(n=0; n<n_symbols; n++)
		for(i=0; i<n_lines[n]; i++) 
			fulldate[j++] = data[n][i][0];
	
	// for debugging propourse
	/*	
	j=1;
	for(n=0; n<n_symbols; n++)
		for(i=0; i<n_lines[n]; i++) 	
			printf("%d %s %d\n", total_nlines, *data[n][i], j++);
	printf("%s\n", *data[n_symbols-1][n_lines[n_symbols-1]-1]);
	*/
	
	max_rows = array_unique(fulldate, total_nlines); // Max numbre of dates



	/////////////////  Data base: Date in columns ///////////////////////
	
	outputcsvfp = fopen("output.csv", "w");	// Open for write and delete old file
	k = (unsigned *)xmalloc(sizeof(unsigned) * n_symbols);
	for(n=0; n<n_symbols; n++) k[n] = 0;	// array for control of each quote
	
	for(i=0; i<max_rows; i++) {
		
		fprintf(outputcsvfp, "%s", fulldate[i]);	// Date for the new row

		for(n=0; n<n_symbols; n++) {

			if(i==0) sprintf(prefix, "%s_", symbols_array[n]);	// For column names
			else sprintf(prefix, "");
		
			// Save data to the CSV file (line by line)
			
			if(open) fprintf(outputcsvfp, ",%s%s", prefix, data[n][k[n]][1]);
			if(high) fprintf(outputcsvfp, ",%s%s", prefix, data[n][k[n]][2]);
			if(low) fprintf(outputcsvfp, ",%s%s", prefix, data[n][k[n]][3]);
			if(close) fprintf(outputcsvfp, ",%s%s", prefix, data[n][k[n]][4]);
			if(volume) fprintf(outputcsvfp, ",%s%s", prefix, data[n][k[n]][5]);
			if(adjclose) fprintf(outputcsvfp, ",%s%s", prefix, data[n][k[n]][6]);

			// While no inclrease index, writes the some iteration (for dates without data)
			if((strcmp(data[n][k[n]][0], fulldate[i])==0) && (k[n]+1)<n_lines[n]) k[n]++;
			
		}
		fputs("\n", outputcsvfp);
	}

	fclose (outputcsvfp);

	printf("\nSee %s%soutput.csv\n", getcwd(inputcsv, MAX_FILE_NAME_LEN), DIRSEP);

	// for windows : clear dll socket
	WINDLLCLEAR

	return EXIT_SUCCESS;
}





