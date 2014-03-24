<html>
<body>

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int A[10][10];   //A matrix
int B[10][10];   //B matrix 
int C[10][10];   //A matrix * B matrix

int M,N,K;   //index

pthread_mutex_t mutx;

//get number of col(\n)
int getCol(char*fileName)
{
	int col =0;
	char c;
	FILE*fp=fopen(fileName,"r");   //open file

	/*get number of col*/
	while((c=getc(fp))!=EOF)
	{
		if(c=='\n')
			col++;
	}
	fclose(fp);   //close file

	return col;  //return number of col 
}
// get number of row(' ')
int getRow(char*fileName)
{
	int row=0;
	char c;
	FILE*fp=fopen(fileName,"r");   //open file

	/*get number of row*/
	while((c=getc(fp))!='\n')
	{
		if(c!=' ')
			row++;
		
	}

	fclose(fp);   //close file
	return row;   //return number of row
}
//thread function
void *matrixMult(void *data);

int main()
{
	printf("[2011722070 HAJOOYOUNG]\n");
	int i,j;
	FILE*fp1,*fp2;
	int c;


	int row,col;

	row=getRow("a.mat");   //get row of a.mat
	col=getCol("b.mat");   //get col of b,mat

	printf("a_row=%d b_col=%d\n",row,col);
	printf("a_col=%d b_row=%d\n",getCol("a.mat"),getRow("b.mat"));
	
	/* if row of a.mat != col of b.mat, exit main*/
	if(row!=col)   
	{
		printf("matrix row!=col\n");
		return 0;
	}
	else   //calculate matrix
	{
		/*get indext of matrix*/
		M=getCol("a.mat");   
		N=getRow("b.mat");
		K=row;
	}

	/*init thread*/
	pthread_t threadID[M];

	//initiate mutex
	int state=pthread_mutex_init(&mutx,NULL);
	if(state!=0)
		printf("pthread_mutex_init() error\n");
   
	pthread_attr_t attr;
	pthread_attr_init(&attr);
		
	/*read a.mat and copy matix of a.mat to 2-array A*/	
	fp1=fopen("a.mat","r");
	i=0;
	j=0;

	while((c=getc(fp1))!=EOF)
	{
		if(c==' ')
			continue;
		else
		{
			if(c=='\n')
			{
				i++;
				j=0;
				continue;
			}
			else
			{
				A[i][j]=(int)c-48;
				printf("A[%d][%d] : %d\n",i,j,A[i][j]);
				j++;
				continue;
			}
		}
		printf("\n");
	}
	fclose(fp1);   //close a.mat

	/* read b.mat and copy matrix b.mat to 2-array B*/
	fp2=fopen("b.mat","r");
	i=0;
	j=0;
	while((c=getc(fp2))!=EOF)
	{
		if(c==' ')
			continue;
		else
		{
			if(c=='\n')
			{
				i++;
				j=0;
				continue;
			}
			else
			{
				B[i][j]=(int)c-48;
				printf("B[%d][%d] : %d\n",i,j,B[i][j]);			
				j++;
				continue;
			}
		}
	}
	fclose(fp2);   //close b.mat

	//create multi-thread
	for(i=0;i<M;i++)
	{
		
		pthread_create(&threadID[i],NULL,*matrixMult,(void *)i);	
		sleep(1);
	}

	
	
	for(i=0;i<M;i++)
	{
		pthread_join(threadID[i],NULL);   //exit thread
			
		
	}

	//remove mutex
	pthread_mutex_destroy(&mutx);
	return 0;
}
void *matrixMult(void*data)
{
	FILE*fp1=fopen("c.mat","a+");   //open c.mat for write result of matrix
	int i,t;
	int sum=0;

	pthread_mutex_lock(&mutx);

	int data2=(int)data;   //copy struct 
	if(data2==0)
		fprintf(fp1,"[2011722070 HAJOOYOUNG]\n");
	for(i=0;i<N;i++){
		sum=0;	
		for(t=0;t<K;t++){
			sum+=(A[data2][t]*B[t][i]);   //calculate matrix
		}
	C[data2][i]=sum;
	fprintf(fp1,"%d ",C[data2][i]);   //print result of matrix
	}
	fprintf(fp1,"\n");
			

	fclose(fp1);   //close file c.mat
	pthread_mutex_unlock(&mutx);
}
</body>
</html>
