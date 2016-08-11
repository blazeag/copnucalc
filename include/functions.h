#include <time.h>

#define TIME_MAX_STRING 100

typedef struct
{
	clock_t begin_clock, save_clock;
	time_t begin_time, save_time;
} time_keeper;

static time_keeper tk;



// Prototypes
// --------------------------------------------
void read_parameters (int argc, char *argv[], char **filename);
uint64_t restore(char *filename, uint64_t *ram_buffer);
void show_statistics (uint64_t found, uint64_t counter, uint64_t *ram_buffer, double time);
int kbhit(void);
void start_time(void);
double prn_time(void);



//====================================================================
// Reads parameters and give back the name of the specified file.
// If it doesn't exists, it will be created.
// -------------------------------------------------------------------
// IN  int argc, char *argv[]
// OUT char **filename
//====================================================================
void read_parameters (int argc, char *argv[], char **filename)
{
	FILE *fp;
	
	// Wrong arguments number
	if (argc != 2)
	{
		printf("Syntax error. -> %s <filename>\n\n", argv[0]);
		exit(-1);
	}
	
	*filename = (char *) calloc(strlen(argv[1]), sizeof(char));
	*filename = argv[1];
	
	fp = fopen(*filename, "r");
	
	// If file doesn't exist, initialize it
	if (! fp)
	{
		fp = fopen(*filename, "w");
		
		if (! fp)
		{
			printf("Unable to create file %s", *filename);
			exit(-2);
		}
		
		fprintf(fp, "2\n3\n");
	}
	
	// Close file pointer
	fclose(fp);
}



// ====================================================================
// Reads the specified file to restore previously found prime numbers
// -------------------------------------------------------------------
// IN  char *filename
// OUT uint64_t *ram_buffer
// ====================================================================
uint64_t restore(char *filename, uint64_t *ram_buffer)
{
	uint64_t i, temp;
	FILE *fp;
	
	printf("Restoring informations from %s... ", filename);
	
	fp = fopen(filename, "r");
	
	for (i = 0; ; i++)
	{
		fscanf(fp, "%" PRIu64, &temp);
		if (feof(fp)) break;
		
		ram_buffer[i] = temp;
	}
	
	fclose(fp);
	
	printf("Done! %" PRIu64 " numbers restored\n", i);
	
	return i;
}



// ===================================================================
// Output statistics
// -------------------------------------------------------------------
// IN  int found, int counter, unsigned int *ram_buffer
// ===================================================================
void show_statistics (uint64_t found, uint64_t counter, uint64_t *ram_buffer, double time)
{
	printf("\n==============================================");
	printf("\n            SESSION STATISTICS");
	printf("\n==============================================\n\n");
	
	printf("Total found   : %10" PRIu64 "\n", found);
	printf("This session  : %10" PRIu64 "\n", counter);
	printf("Last          : %10" PRIu64 "\n\n", ram_buffer[found - 1]);
	printf("Time elapsed  : %10.0lf\n", time);
	printf("Found/sec     : %13.2lf\n", (double) counter / time );
	printf("Found/min     : %13.2lf\n", (double) (counter / time) * 60);
	
	printf("\n==============================================\n");
}



// Keyboard hit event listener
// --------------------------------------------
int kbhit(void)
{
	struct timeval tv;
	fd_set read_fd;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&read_fd);
	FD_SET(0,&read_fd);
	
	if (select(1, &read_fd, NULL, NULL, &tv) == -1)
	{
		return 0;
	}
	
	if(FD_ISSET(0, &read_fd))
	{
		return 1;
	}
	
	return 0;
}



// Start timer
// --------------------------------------------
void start_time(void)
{
	tk.begin_clock = tk.save_clock = clock();
	tk.begin_time = tk.save_time = time (NULL);
}



// Return elapsed time
// --------------------------------------------
double prn_time(void)
{
	char s1[TIME_MAX_STRING], s2[TIME_MAX_STRING];
	int field_width, n1, n2;
	double clocks_per_second = (double) CLOCKS_PER_SEC, user_time, real_time;
	
	user_time = (clock() - tk.save_clock) / clocks_per_second;
	real_time = difftime(time(NULL), tk.save_time);
	tk.save_clock = clock();
	tk.save_time = time(NULL);
	
	return user_time;
}
