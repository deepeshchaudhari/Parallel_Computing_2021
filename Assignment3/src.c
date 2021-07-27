#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int gl_row,gl_col;
int step;
// Function for import data in 2d matrix and return its address
float **import_data(char *file_path){
	// initialization
	float **tdata;
	char buffer[1024];
	int clen=0;
	int rlen=0;

	//  finding row and column in tdata.csv
	FILE* fp = fopen(file_path, "r");
	while(fgets(buffer,1024, fp)){
		if (rlen==0)
		{
			char* value = strtok(buffer, ", ");
            while(value){
            	value = strtok(NULL, ", ");
            	clen++;
            }
		}
		rlen++;
	}
	gl_col=clen;
	gl_row=rlen;

	//  Allocating space for tdata.csv 
	tdata = (float **)malloc(gl_row*sizeof(float*));
	if (tdata==NULL)
	{
		// printf("Memory not allocated\n");
		exit(0);
	}
	else{

		FILE* fp = fopen(file_path, "r");
		// printf("Memory allocated successfully\n");	
		if (fp == NULL)
		{
			printf("Not loaded\n");
		}
		int column;
		int row=0;
		while (fgets(buffer,1024, fp)) {
	            column = 0;
	            // To avoid printing of column
	            // names in file can be changed
	            // according to need

	            // Splitting the data
	            char* value = strtok(buffer, ", ");
	            // printf("%s\n",value );
	  			float *temp = (float *)malloc(gl_col*sizeof(float*));
	            while (value) {  
		                temp[column]=atof(value);
		                value = strtok(NULL, ", ");
		                column++;
		         
	            }
	            tdata[row] = temp;
	            row++;
	            if (row==gl_row)
	            {
	            	break;
	            }
			}
	// printf("File %s loaded.\n",file_path);
	// returning address of 2d array
	return tdata;
	}
}	

// Function for data distribution and computation
float multi_processor_solution(char *file_path){
	// Initializing COMM_WORLD and there variable
	int numtasks, rank, name_len,count;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &numtasks);
	
	// Loading tdata data ...
	float **tdata;
	if (rank == 0)
	{
		tdata = import_data(file_path);
		if (tdata == NULL)
		{
			printf("Memory not allocated\n");
		}
		else{
		}

	}
// ==============================================
	MPI_Barrier(MPI_COMM_WORLD);

	double stime = MPI_Wtime();

// Broadcasting row and col info to every process
	MPI_Bcast(&gl_col,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&gl_row,1,MPI_INT,0,MPI_COMM_WORLD);

//  Initializing 1D Array of row*col size for scattering data...
	float *Scatter_data = (float *)malloc((gl_col*gl_row)*sizeof(float*));
	int recvsize = ((gl_col-2)/numtasks)*(gl_row-1);
//  Initializind !D array recieve by every process after scatter...
	float *recvMess= (float *)malloc(recvsize*sizeof(float*));


//  Converting 2D data to 1D in Scatter_data...
	int index=0;
	if (rank == 0)
	{	
		
		for (int i = 2; i < gl_col; i++)
		{	
			for (int j = 1; j < gl_row; j++)
			{

				Scatter_data[index] = tdata[j][i];
				index++;
			}
		}

	}

//=============== Scatter all data ====================
    MPI_Scatter (Scatter_data, recvsize, MPI_FLOAT, recvMess, recvsize, MPI_FLOAT, 0, MPI_COMM_WORLD);
    int column_send = (gl_col-2)/numtasks;
    float min_arr[column_send];
    float min_temp = INFINITY;
// ====================================================
    for (int i = 0; i < recvsize; ++i)
    {

    	if (recvMess[i] < min_temp)
    	{
    		min_temp = recvMess[i];
    	}
    	if (i%(gl_row-1) == (gl_row-1)-1)
    	{
    		min_arr[i/(gl_row-1)]= min_temp;
    		min_temp = INFINITY;
    	}

    }

    // ===================  Gather all solution =================
    recvsize = ((gl_col-2)/numtasks)*numtasks;
    float recvSolution[recvsize];
  	MPI_Gather (min_arr, column_send, MPI_FLOAT, recvSolution, column_send, MPI_FLOAT, 0, MPI_COMM_WORLD);

	// ==========================================================
  	float remain_min = INFINITY;
    if (rank ==0 && recvsize < gl_col-2)

    {	
    	for (int i = 1; i < gl_row; ++i)
    	{
    		if (tdata[i][gl_col-1] < remain_min )
    		{
    			remain_min = tdata[i][gl_col-1];
     		}
    	}
    } 


// ========= printing output ======================================
	if (rank==0)
	{
		float global_min = INFINITY;
		for (int i = 0; i < recvsize; ++i)
		{
			printf("%f ",recvSolution[i]);

		}
		if (remain_min != INFINITY )
			{
    			printf("%f\n",remain_min );
    			global_min = remain_min;
			}
		else{
			printf("\n");
		}
		for (int i = 0; i < recvsize; ++i)
		{
			if (recvSolution[i]<global_min)
			{
				global_min = recvSolution[i];
			}
		}
		printf("%f\n",global_min );

	}
	double etime = MPI_Wtime();
	return etime-stime;
}
float single_processor_solution(char *file_path){
	int column,row=0,count=43;
	float **tdata;
	int N=430500;
	char buffer[1024];
	tdata = (float **)malloc(N*sizeof(float*));

	float stime,etime,x = INFINITY;
	float min_temp_year[count];
	float global_min = x;
	
	for (int i = 0; i < count; ++i)
	{
		min_temp_year[i]=x;
	}

	FILE* fp = fopen(file_path, "r");
	// printf("File %s loaded.\n", file_path );
	stime =MPI_Wtime();
	while (fgets(buffer,1024, fp)) {
        column = 0;
        // To avoid printing of column
        // names in file can be changed
        // according to need

        // Splitting the data
        char* value = strtok(buffer, ", ");
		float *temp = (float *)malloc(count*sizeof(float*));
        while (value) {  
            temp[column]=atof(value);
            value = strtok(NULL, ", ");
            // printf("%d\n",column );
            if (temp[column] < min_temp_year[column])
            {
            	min_temp_year[column]=temp[column];
            }
            column++;
        }
        tdata[row] = temp;
        row++;
	}

	for (int i = 0; i < count; ++i)
	{
		if (i<2)
		{
			continue;
		}
		printf("%lf ",min_temp_year[i] );
		if (min_temp_year[i] < global_min)
		 {
		 	global_min=min_temp_year[i];
		 } 
	}
	etime = MPI_Wtime();
	printf("\n");
	printf("%lf\n",global_min );
	return etime-stime;
}

int main(int argc,char *argv[]){

	// init comm_world
	int numtasks, rank, name_len,count;
	char name[20];
	char *file_path;
	// initialize MPI
	MPI_Init (&argc, &argv);
	// get number of tasks
	MPI_Comm_size (MPI_COMM_WORLD, &numtasks);
	// get my rank
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	// get preocessor name
	MPI_Get_processor_name(name,&name_len);

	MPI_Status status, sstatus;
	MPI_Request request;

	file_path = argv[1];

	float max_time;
	float time = multi_processor_solution(file_path);
	MPI_Reduce(&time,&max_time, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);
	if (rank==0)
	{
		printf("%lf\n",max_time );
		FILE * fPtr;
		fPtr = fopen("time_data.txt", "a");
	    if(fPtr == NULL)
	    {
	        /* File not created hence exit */
	        printf("Unable to create time.txt\n");
	    }
	    fprintf(fPtr, "%lf\n",max_time);
	    fclose(fPtr);
	}
	MPI_Finalize();

    return(0);

}