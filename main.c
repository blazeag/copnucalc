/*****************************************************************************
 * CoPNuF - Compulsive Prime Numbers Finder
 *****************************************************************************
 * (c) 2006 Andrea Gardoni <andrea.gardonitwentyfour@gmail.com> minus 24
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3.0 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program;
 * if not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "include/functions.h"

#define MAXNUM 100000000 // Max findable numbers (used for memory allocation)
#define TRUE 1
#define FALSE !TRUE

typedef unsigned short bool;



// Main
// --------------------------------------------
int main (int argc, char * argv[])
{
	uint64_t *ram_buffer, half;
	char *filename;
	FILE *fp;
	uint64_t i, j, counter, found;
	bool not_divisible;
	double time;
	
	
	
	// Read parameters and allocate memory
	// -------------------------------
	read_parameters(argc, argv, &filename);
	
	// Allocate memory for prime numbers array
	ram_buffer = (uint64_t*) calloc(MAXNUM, sizeof(uint64_t));
	
	if (ram_buffer == NULL)
	{
		printf("Insufficient physical memory.\n");
		exit(-1);
	}
	
	
	
	// Load prime numbers into memory
	// if specified file is not empty 
	// -------------------------------
	found = restore(filename, ram_buffer);
	
	
	
	// Calculation procedure
	// -------------------------------
	start_time();
	
	printf("Calculation in progress. Press Enter to stop... ");
	fflush(stdout);
	
	counter = found;			// Currently found numbers counter
	i = ram_buffer[found - 1];	// Starts from the last found prime number
	
	// DO cycle till a keyboard hit or maximum number of prime numbers found
	do
	{
		// Jump to next odd number
		i = i + 2;
		not_divisible = TRUE;
		
		// Calculate half the number
		half = (int) (i + 1) / 2;
		
		// Try to divide the current number for every prime number found so far
		for (j = 0; j < found; j++)
		{
			// If currently compared prime number is more than the half of the current number, it is a prime number
			if (ram_buffer[j] > half) break;
			
			// If the number is divisible by the current prime number, it is not prime
			if ((i % ram_buffer[j]) == 0)
			{
				not_divisible = FALSE;
				break;
			}
		}
		
		// If number was found not divisible by all prime number found so far smaller that its half, it's a prime number
		if (not_divisible)
		{
			found++;
			ram_buffer[found - 1] = i;
		}
	}
	while (! kbhit() && found < (MAXNUM - 1));
	
	time = prn_time();
	printf("Stopped!\n");
	
	// Calculate how many numbers have been found during this session
	counter = found - counter;
	
	// *** Calculation procedure end
	
	
	
	// Sore found numbers to output file
	// -------------------------------
	fp = fopen(filename, "a");
	
	for (i = counter; i > 0; i--)
	{
		fprintf(fp, "%" PRIu64 "\n", ram_buffer[found - i]);
	}
	
	fclose(fp);
	
	
	
	// Output statistics
	// -------------------------------
    show_statistics (found, counter, ram_buffer, time);
}
