#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <stdlib.h>
#include<time.h>

void replacecharacters(char dnabig[]);				// function to replace characters R and W 
void countA(char dnabig[]);					// function to count the number of A's in the sequence of 10^6


int main(int argc, char *argv[])				// main function
{
	double total_time;					// variables to calculate time taken by program
	clock_t start, end;
	start = clock();
	srand(time(NULL));
	char dna[]={'A','G','T','C','R','W'};			// array of 6 character
	char dnabig[1000000];					// initializing array dnabig to contain combination of all 6 characters
	int len;
	int k=0;

	int randomnumber;
	len=sizeof(dna);


		for(int i=0;i<1000000; i++)			// to randomly fill the dbabig array with 10^6 sequence of dna's
		{
			randomnumber = rand() % 6;
			dnabig[i]=dna[randomnumber];
		}

	replacecharacters(dnabig);	
	countA(dnabig);
	end = clock();
	total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	//calulate total time
	printf("Time taken to find the percentage of 'A' is: %f \n", total_time);
	printf("\n");
	return 0;
}

void replacecharacters(char dnabig[])				// function to replace characters R and W 
{
	int c=0;
	int cc=0;
	int l=0;
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
	for(int i=0;i<1000000; i++)				// to print the dna sequence of 10^6 combinations without containg R and W
		{	
		printf("%c",dnabig[i]);
		}
	printf("\n \n");
	
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
	//percentage = count/1000000;
	printf("The total no of A's is generated sequence is %d \n",count);
	float d = (float)count/1000000;
	float percentage = d*100;
	//percentage = (float)((count/1000000)*100);
	printf("The percentage of A's in generated sequence is %.2f \n",percentage);
}


