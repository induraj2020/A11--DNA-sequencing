#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  ARRAYSIZE	1000000
#define  MASTER		0
#include<time.h>

float  dnabig[ARRAYSIZE];

int main (int argc, char *argv[])
{
int   numtasks, taskid, rc, dest, offset, i, j, tag1,
      tag2, source, chunksize; 
double total_time;
char dna[]={'A','G','T','C','R','W'};
float mysum, sum;
float update(int myoffset, int chunk, int myid);
MPI_Status status;
int randomnumber;
srand(time(0));
clock_t start, end;
start = clock();
srand(time(NULL));

/***** Initializations *****/
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
if (numtasks % 5 != 0) {
   printf("Quitting. Number of MPI tasks must be divisible by 4.\n");
   MPI_Abort(MPI_COMM_WORLD, rc);
   exit(0);
   }
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
printf ("MPI task %d has started...\n", taskid);
chunksize = (ARRAYSIZE / numtasks);
tag2 = 1;
tag1 = 2;

/***** Master task only ******/
if (taskid == MASTER){

  /* Initialize the array */

   	for(int i=0;i<1000000; i++)			// to randomly fill the dbabig array with 10^6 sequence of dna's
	{
		randomnumber = rand() % 6;
		dnabig[i]=dna[randomnumber];
	}

  /* Send each task its portion of the array - master keeps 1st part */
  offset = chunksize;
  for (dest=1; dest<numtasks; dest++) {
    MPI_Send(&offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
    MPI_Send(&dnabig[offset], chunksize, MPI_FLOAT, dest, tag2, MPI_COMM_WORLD);
    printf("Sent %d elements to task %d offset= %d\n",chunksize,dest,offset);
    offset = offset + chunksize;
    }

  /* Master does its part of the work */
  offset = 0;
  mysum = update(offset, chunksize, taskid);

  /* Wait to receive results from each task */
  for (i=1; i<numtasks; i++) {
    source = i;
    MPI_Recv(&offset, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
    MPI_Recv(&dnabig[offset], chunksize, MPI_FLOAT, source, tag2,
      MPI_COMM_WORLD, &status);
    }

  /* Get final sum and print sample results */  
  MPI_Reduce(&mysum, &sum, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);
  //printf("Sample results: \n");
  offset = 0;
  printf("*** Final sum= %.2f ***\n",sum);
  float d = (float)sum/1000000;
  float percentage = d*100;
  printf("The percentage of A's in generated sequence is %.2f \n",percentage);
  end = clock();
  total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Time taken to find the percentage of 'A' is: %f \n", total_time);
	printf("\n");
  }  /* end of master section */



/***** Non-master tasks only *****/

if (taskid > MASTER) {

  /* Receive my portion of array from the master task */
  source = MASTER;
  MPI_Recv(&offset, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
  MPI_Recv(&dnabig[offset], chunksize, MPI_FLOAT, source, tag2, 
    MPI_COMM_WORLD, &status);

  mysum = update(offset, chunksize, taskid);

  /* Send my results back to the master task */
  dest = MASTER;
  MPI_Send(&offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
  MPI_Send(&dnabig[offset], chunksize, MPI_FLOAT, MASTER, tag2, MPI_COMM_WORLD);

  MPI_Reduce(&mysum, &sum, 1, MPI_FLOAT, MPI_SUM, MASTER, MPI_COMM_WORLD);

  } /* end of non-master */


MPI_Finalize();

}   /* end of main */


float update(int myoffset, int chunk, int myid) {
  int i; 
  float mysum;
  /* Perform addition to each of my array elements and keep my sum */
  mysum = 0;
  int c=0;
	int cc=0;
	int l=0;
  for(i=myoffset; i < myoffset + chunk; i++) 
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
 
for(i=myoffset; i < myoffset + chunk; i++) 
   {
   if(dnabig[i]=='A')
		{		
		mysum=mysum+1;
		}
	else
		{
		continue;
		}
    }
  printf("Task %d mysum = %.2f\n",myid,mysum);
  return(mysum);
  }
