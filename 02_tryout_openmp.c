#include<stdio.h>
#include<stdlib.h>
#include <omp.h>
#include<unistd.h>
#include <stdlib.h>
#include<time.h>

#define NUM_THREADS 12
#define STATIC_CHUNK 10
#define DYNAMIC_CHUNK 10
#define NUM_LOOPS 10
#define SLEEP_EVERY_N 3

void replacecharacters(char dnabig[]);				// function to replace characters R and W 
void countA(char dnabig[]);					// function to count the number of A's in the sequence of 10^6


int main(int argc, char *argv[])				// main function
{
	double total_time;					// variables to calculate time taken by program
	clock_t start, end;
	float nStatic1[NUM_LOOPS], nStaticN[NUM_LOOPS];
   	float nDynamic1[NUM_LOOPS], nDynamicN[NUM_LOOPS];
    	float nGuided[NUM_LOOPS];	
	omp_set_num_threads(NUM_THREADS);

	char dna[]={'A','G','T','C','R','W'};			// array of 6 character
	char dnabig[1000000];					// initializing array dnabig to contain combination of all 6 characters
	int randomnumber;
	//int len=sizeof(dna);

	omp_set_num_threads(12);
	
	start = clock();
	srand(time(NULL));
#pragma omp parallel
{	
        #pragma omp for schedule(static, 1)		      //// case of static with chunk size=1
	for (int j = 0 ; j < NUM_LOOPS ; ++j) 
	{
	for(int i=0;i<1000000; i++)			      // to randomly fill the dbabig array with 10^6 sequence of dna's
	{
		randomnumber = rand() % 6;
		dnabig[i]=dna[randomnumber];
	}
	
	replacecharacters(dnabig);	
	countA(dnabig);
	end = clock();
	total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	nStatic1[j] = total_time;
	
	}

	#pragma omp for schedule(static, STATIC_CHUNK)		//// case of static with chunk size=10
	
	for (int j = 0 ; j < NUM_LOOPS ; ++j) 
	{
	for(int i=0;i<1000000; i++)			 	// to randomly fill the dbabig array with 10^6 sequence of dna's
	{
		randomnumber = rand() % 6;
		dnabig[i]=dna[randomnumber];
	}
	
	replacecharacters(dnabig);	
	countA(dnabig);
	end = clock();
	total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	nStaticN[j] = total_time;
	}


	 #pragma omp for schedule(dynamic, 1)			//// case of dynamic with chunk size=1
	for (int j = 0 ; j < NUM_LOOPS ; ++j) 
	{
	for(int i=0;i<1000000; i++)			 	// to randomly fill the dbabig array with 10^6 sequence of dna's
	{
		randomnumber = rand() % 6;
		dnabig[i]=dna[randomnumber];
	}
	
	replacecharacters(dnabig);	
	countA(dnabig);
	end = clock();
	total_time = ((double) (end - start)) / CLOCKS_PER_SEC;

	nDynamic1[j] = total_time;
	}

	#pragma omp for schedule(dynamic, DYNAMIC_CHUNK)	//// case of dynamic with chunk size=10
	for (int j = 0 ; j < NUM_LOOPS ; ++j) 
	{
	for(int i=0;i<1000000; i++)				// to randomly fill the dbabig array with 10^6 sequence of dna's
	{
		randomnumber = rand() % 6;
		dnabig[i]=dna[randomnumber];
	}
	
	replacecharacters(dnabig);	
	countA(dnabig);
	end = clock();
	total_time = ((double) (end - start)) / CLOCKS_PER_SEC;

	nDynamicN[j] = total_time;
	}

	#pragma omp for schedule(guided)			//// case of guided
	for (int j = 0 ; j < NUM_LOOPS ; ++j) 
	{
	for(int i=0;i<1000000; i++)				// to randomly fill the dbabig array with 10^6 sequence of dna's
	{
		randomnumber = rand() % 6;
		dnabig[i]=dna[randomnumber];
	}
	
	replacecharacters(dnabig);	
	countA(dnabig);
	end = clock();
	total_time = ((double) (end - start)) / CLOCKS_PER_SEC;

	nGuided[j] = total_time;
	}

	
}
    printf("------------------------------------------------\n");
    printf("| static \t| static \t| dynamic \t| dynamic \t| guided |\n");
    printf("|    1   \t|    %d  \t|    1    \t|    %d   \t|        |\n", STATIC_CHUNK, DYNAMIC_CHUNK);
    printf("------------------------------------------------\n");

    for (int i=0; i<NUM_LOOPS; ++i) 
    {
        printf("|    %f   |    %f   |    %f    |    %f    |   %f   |\n", nStatic1[i], nStaticN[i], nDynamic1[i], nDynamicN[i], nGuided[i]);
    }

    printf("------------------------------------------------\n");
    return 0;
}

void replacecharacters(char dnabig[])				// function to replace characters R and W 
{
	int c=0;
	int cc=0;
	//char newseq[1000000];
	for(int i=0;i<1000000;i++)
	{
	if((dnabig[i]!='R')&&(dnabig[i]!='W'))
		{
		//newseq[l]=dnabig[i];
		continue;
		}
	else if(dnabig[i]=='R')
		{
		if(c%2==0)
			{
				dnabig[i]='A';
				c=c+1;
			}
		else
			{
				dnabig[i]='G';
				c=c+1;
			}
		}
	else if(dnabig[i]=='W')
		{
		if(cc%2==0)
			{
			dnabig[i]='A';
			cc=cc+1;
			}
		else
			{
			dnabig[i]='T';
			cc=cc+1;
			}
		}
	}

	
}


void countA(char dnabig[])					// function to count the number of A's in the sequence of 10^6
{

	int count=0;
	for(int i=0;i<1000000;i++)
	{
		if(dnabig[i]=='A')
			{		
			count=count+1;
			}
		else
			{
			continue;
			}
	}
	
}


