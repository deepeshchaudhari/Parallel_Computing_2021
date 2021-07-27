#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

double **alloc_2d_int(int rows, int cols);
double genrate_random(int range);


int main( int argc, char *argv[])
{
  int myrank, size;


  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
  MPI_Comm_size (MPI_COMM_WORLD, &size);


  int no_of_processes =  size;  
  int array_of_sizes = atoi(argv[1]);
  int steps = atoi(argv[2]);
  int sqrt_p =  sqrt(no_of_processes);


  // initialize test array
  // double data[4][4] ={
  //                           {(double)4,(double)2,(double)4,(double)4},
  //                           {(double)1,(double)1,(double)3,(double)3},
  //                           {(double)4,(double)4,(double)2,(double)2},
  //                           {(double)3,(double)3,(double)1,(double)3},
  //                         };


// allocating array dynamically
double **data;
data = alloc_2d_int(array_of_sizes,array_of_sizes);


//  communication using MPI_PACKED 


double sTime, eTime, time;
sTime = MPI_Wtime();

for (int step = 0; step < steps; step++)
{
  
      int up_position=0; 
      int right_position=0; 
      int left_position=0; 
      int down_position=0; 

      MPI_Status status;
      MPI_Request send_request,recv_request,req[no_of_processes];

      // up, down, left and right for send
      double up[array_of_sizes];
      double down[array_of_sizes];
      double right[array_of_sizes];
      double left[array_of_sizes];

      //  up, down, left and right for receive
      double recv_up[array_of_sizes];
      double recv_down[array_of_sizes];
      double recv_right[array_of_sizes];
      double recv_left[array_of_sizes];

      // initialize up,down , left and right
      for (int i = 0; i < array_of_sizes; ++i)  {
        recv_up[i]= (double)(0);
        recv_down[i]= (double)(0);
        recv_right[i]= (double)(0);
        recv_left[i]= (double)(0);
      }

      for (int i=0; i<array_of_sizes; i++){
             for (int j=0; j<array_of_sizes; j++){
                    if (step==0)
                    {
                      data[i][j] = genrate_random(100);
//                      data[i][j] = i*array_of_sizes +j;
                    }
                    if(i==0 && j<array_of_sizes){
                      up[j] = data[i][j];
                      MPI_Pack (&data[i][j], 1, MPI_DOUBLE, up, 8*array_of_sizes, &up_position, MPI_COMM_WORLD);
                    }

                    if(j==0 && i<array_of_sizes){
                      left[i] = data[i][j];
                      MPI_Pack (&data[i][j], 1, MPI_DOUBLE, left, 8*array_of_sizes, &left_position, MPI_COMM_WORLD);
                    }

                    if(i == array_of_sizes-1 && j< array_of_sizes){
                      down[j] = data[i][j];
                      MPI_Pack (&data[i][j], 1, MPI_DOUBLE, down, 8*array_of_sizes, &down_position, MPI_COMM_WORLD);
                    }

                    if(j == array_of_sizes-1 && i<array_of_sizes){
                      right[i] = data[i][j];
                      MPI_Pack (&data[i][j], 1, MPI_DOUBLE, right, 8*array_of_sizes, &right_position, MPI_COMM_WORLD);
                    }
               }
            }

      // if(myrank == 0)
      //   {
      //     for (int i = 0; i < array_of_sizes; i++)
      //     {
      //       for (int j = 0; j < array_of_sizes; j++)
      //       {
      //         printf("%lf ", data[i][j]);
      //       }
      //       printf("\n");
      //     }
      //     printf("=======================================\n");
      //   }

       // sending left, right, up and down to parrallel process
       if (myrank-sqrt_p <0){
       // printf("%d has no up\n",myrank);
       }
       else{
        // setting dest and rank same
        MPI_Isend (up, up_position, MPI_PACKED, myrank-sqrt_p,myrank-sqrt_p , MPI_COMM_WORLD,&send_request);
        MPI_Wait(&send_request, &status);
       }
       if (myrank+sqrt_p >= no_of_processes){
         //     printf("%d has no down\n",myrank);
       }
       else{

        MPI_Isend (down, down_position, MPI_PACKED, myrank+sqrt_p, myrank+sqrt_p, MPI_COMM_WORLD,&send_request);
        MPI_Wait(&send_request, &status);

      }
       if ((myrank+1)%sqrt_p == 0){
         //     printf("%d has no right\n",myrank);
       }
       else{

        MPI_Isend (right, right_position, MPI_PACKED, myrank+1,myrank+1, MPI_COMM_WORLD,&send_request);
        MPI_Wait(&send_request, &status);

      }
       if (myrank%sqrt_p==0){
        //      printf("%d has no left\n",myrank);
       }
       else{

        MPI_Isend (left, left_position, MPI_PACKED, myrank-1, myrank-1, MPI_COMM_WORLD,&send_request);
        MPI_Wait(&send_request, &status);

       }



     //==============================================================================================

      // recieving up, down, left and right

      if (myrank-sqrt_p < 0){
      //      printf("%d has no up\n",myrank);
      }
      else{
              MPI_Recv (recv_up, array_of_sizes, MPI_DOUBLE, myrank-sqrt_p, myrank, MPI_COMM_WORLD, &status);
              // printf("%d recv up from %d\n",myrank,myrank-sqrt_p);
              // for(int i=0;i<array_of_sizes;i++){
              //         printf("[%lf] ",recv_up[i]);
              //     }
              //   printf("\n");


      }
      if (myrank+sqrt_p >= no_of_processes){
      //    printf("%d has no down\n",myrank);
      }
      else{
              MPI_Recv (recv_down, array_of_sizes, MPI_DOUBLE, myrank+sqrt_p, myrank, MPI_COMM_WORLD, &status);
              // printf("%d recv down from %d\n",myrank,myrank+sqrt_p);
              // for(int i=0;i<array_of_sizes;i++){
              //         printf("[%lf] ",recv_down[i]);
              //         }
              // printf("\n");

      }
      if ((myrank+1)%sqrt_p == 0){
        //  printf("%d has no right\n",myrank);
      }
      else{
              MPI_Recv (recv_right, array_of_sizes, MPI_DOUBLE, myrank+1, myrank, MPI_COMM_WORLD, &status); 
              // printf("%d recv right from %d\n",myrank,myrank+1);
              // for(int i=0;i<array_of_sizes;i++){
              //           printf("[%lf] ",recv_right[i]);
              //           }
              // printf("\n");


      }
      if (myrank%sqrt_p==0){
          //printf("%d has no left\n",myrank);
      }
      else{
              MPI_Recv (recv_left, array_of_sizes, MPI_DOUBLE, myrank-1, myrank, MPI_COMM_WORLD, &status);
              // printf("%d recv left from %d\n",myrank,myrank-1);
              // for(int i=0;i<array_of_sizes;i++){
              //           printf("[%lf] ",recv_left[i]);
              //           }
              //   printf("\n");

      }
      // printf("\n");

 
      // solve Halo center and initializing up,down, left, and right
       // double temp_data[array_of_sizes][array_of_sizes];
       double **temp_data;
       temp_data = alloc_2d_int(array_of_sizes,array_of_sizes);
       double up_data;
       double down_data;
       double left_data;
       double right_data;

      for (int i=0;i<array_of_sizes;i++){
            for (int j=0;j<array_of_sizes;j++){
                    temp_data[i][j] = data[i][j];
                    // updating halo values
                    if ((i>0 && i<array_of_sizes-1) && (j>0 && j<array_of_sizes-1)) {
                           up_data = data[i-1][j];
                           down_data = data[i+1][j];
                           left_data = data[i][j-1];
                           right_data = data[i][j+1];
                          temp_data[i][j] = (up_data + down_data + left_data + right_data)/4;
                      }
                    else{
                            if (i==0){
                                // myrank has no up
                                up_data = recv_up[j];
                               }
                            else{
                                up_data = data[i-1][j];
                            }
                            if (i == array_of_sizes-1){
                                // myrank has no down
                                down_data = recv_down[j];
                               }
                            else{
                                down_data = data[i+1][j];

                            }
                            if (j==array_of_sizes-1){
                                // myrank has no right
                                right_data = recv_right[i];
                               }
                            else{
                                right_data = data[i][j+1];
                            }
                            if (j==0){
                                  // myrank has no left
                                left_data = recv_left[i];
                               }
                            else{
                                left_data = data[i][j-1];
                            }
                        // printf("%d %d %d\n",i,j,myrank );
                        // printf("%lf %lf %lf %lf\n",up_data,down_data,left_data,right_data );
                        temp_data[i][j] = (up_data + down_data + left_data + right_data)/4;
                        }
              }
        }

        // update array after computation 
      for (int i = 0; i < array_of_sizes; i++)
          {
              for (int j = 0; j < array_of_sizes; j++)
              {
                data[i][j] = temp_data[i][j];
              }
          }
free(temp_data);
}
//=============   END STEP ITERATION =================

// printing communication and computaion time of MPI_PACKED

int root_rank  = 0;
double global_time = 0.00000;

eTime = MPI_Wtime();
time = eTime- sTime;

MPI_Reduce(&time, &global_time, 1, MPI_DOUBLE, MPI_MAX, root_rank, MPI_COMM_WORLD);

if(myrank == root_rank)
  {
    printf("Maximum time taken by %d processes with %d X %d data points using MPI_PACKED: %lf.\n",size,array_of_sizes,array_of_sizes, global_time);
  }






// ========================= Communication with MPI_vector ============================



// allocating array dynamically
double **vector_data;
vector_data = alloc_2d_int(array_of_sizes,array_of_sizes);



// initialize test array

// double vector_data[4][4] ={
//                           {(double)4,(double)2,(double)4,(double)4},
//                           {(double)1,(double)1,(double)3,(double)3},
//                           {(double)4,(double)4,(double)2,(double)2},
//                           {(double)3,(double)3,(double)1,(double)3},
//                             };





sTime = MPI_Wtime();
for (int step = 0; step < steps; step++)
{
    int up_position=0; 
    int right_position=0; 
    int left_position=0; 
    int down_position=0; 

    MPI_Status status;
    MPI_Request send_request,recv_request,req[no_of_processes];


    // up, down, left and right for send
    double up[array_of_sizes];
    double down[array_of_sizes];
    double right[array_of_sizes];
    double left[array_of_sizes];

    //  up, down, left and right for receive
    double recv_up[array_of_sizes];
    double recv_down[array_of_sizes];
    double recv_right[array_of_sizes];
    double recv_left[array_of_sizes];

    // initialize up,down , left and right
    for (int i = 0; i < array_of_sizes; ++i)
    {
      recv_up[i]= (double)(0);
      recv_down[i]= (double)(0);
      recv_right[i]= (double)(0);
      recv_left[i]= (double)(0);
    }


    MPI_Datatype newvtype;
    // MPI_Request send_request;

    int N = array_of_sizes;
    int count = 1;
    int blocklen = array_of_sizes;
    int stride = array_of_sizes;
    int numVectors = 2;
    int recv_data[array_of_sizes];

    MPI_Type_vector (count, blocklen, stride, MPI_INT, &newvtype);
    MPI_Type_commit (&newvtype);

    for (int i=0; i<array_of_sizes; i++){
           for (int j=0; j<array_of_sizes; j++){

                  if (step==0)
                  {
                    vector_data[i][j] = genrate_random(100);
//                    vector_data[i][j] = i*array_of_sizes +j;
                  }
                  if(i==0 && j<array_of_sizes){
                    up[j] = vector_data[i][j];
                    // MPI_Isend (up, 1, newvtype, 1,1 , MPI_COMM_WORLD,&send_request);
                    // MPI_Wait(&send_request, &status);
                    // MPI_Pack (&data[i][j], 1, MPI_DOUBLE, up, 8*array_of_sizes, &up_position, MPI_COMM_WORLD);
                  }

                  if(j==0 && i<array_of_sizes){
                    left[i] = vector_data[i][j];
                    // MPI_Pack (&data[i][j], 1, MPI_DOUBLE, left, 8*array_of_sizes, &left_position, MPI_COMM_WORLD);
                  }

                  if(i == array_of_sizes-1 && j< array_of_sizes){
                    down[j] = vector_data[i][j];
                    // MPI_Pack (&data[i][j], 1, MPI_DOUBLE, down, 8*array_of_sizes, &down_position, MPI_COMM_WORLD);
                  }

                  if(j == array_of_sizes-1 && i<array_of_sizes){
                    right[i] = vector_data[i][j];
                    // MPI_Pack (&data[i][j], 1, MPI_DOUBLE, right, 8*array_of_sizes, &right_position, MPI_COMM_WORLD);
                  }
                }
        }


     // sending left, right, up and down to parrallel process
     if (myrank-sqrt_p <0){
     // printf("%d has no up\n",myrank);
     }
     else{
      // setting dest and rank same

      MPI_Isend (up, numVectors, newvtype, myrank-sqrt_p,myrank-sqrt_p , MPI_COMM_WORLD,&send_request);
      MPI_Wait(&send_request, &status);
     }
     if (myrank+sqrt_p >= no_of_processes){
       //     printf("%d has no down\n",myrank);
     }
     else{

      MPI_Isend (down,numVectors, newvtype, myrank+sqrt_p, myrank+sqrt_p, MPI_COMM_WORLD,&send_request);
      MPI_Wait(&send_request, &status);

    }
     if ((myrank+1)%sqrt_p == 0){
       //     printf("%d has no right\n",myrank);
     }
     else{

      MPI_Isend (right, numVectors, newvtype, myrank+1,myrank+1, MPI_COMM_WORLD,&send_request);
      MPI_Wait(&send_request, &status);

    }
     if (myrank%sqrt_p==0){
      //      printf("%d has no left\n",myrank);
     }
     else{

      MPI_Isend (left, numVectors, newvtype, myrank-1, myrank-1, MPI_COMM_WORLD,&send_request);
      MPI_Wait(&send_request, &status);

     }

    //==============================================================================================



    // recieving up, down, left and right

     if (myrank-sqrt_p < 0){
      //      printf("%d has no up\n",myrank);
     }
     else{
          MPI_Recv (recv_up, numVectors, newvtype, myrank-sqrt_p, myrank, MPI_COMM_WORLD, &status);
          // printf("%d recv up from %d\n",myrank,myrank-sqrt_p);
          // for(int i=0;i<array_of_sizes;i++){
          //         printf("[%lf] ",recv_up[i]);
          //     }
          //   printf("\n");


     }
     if (myrank+sqrt_p >= no_of_processes){
        //    printf("%d has no down\n",myrank);
     }
     else{
          MPI_Recv (recv_down, numVectors, newvtype, myrank+sqrt_p, myrank, MPI_COMM_WORLD, &status);
          // printf("%d recv down from %d\n",myrank,myrank+sqrt_p);
          // for(int i=0;i<array_of_sizes;i++){
          //         printf("[%lf] ",recv_down[i]);
          //         }
          // printf("\n");

     }
     if ((myrank+1)%sqrt_p == 0){
          //  printf("%d has no right\n",myrank);
     }
     else{
          MPI_Recv (recv_right,  numVectors, newvtype, myrank+1, myrank, MPI_COMM_WORLD, &status); 
          // printf("%d recv right from %d\n",myrank,myrank+1);
          // for(int i=0;i<array_of_sizes;i++){
          //           printf("[%lf] ",recv_right[i]);
          //           }
          // printf("\n");


     }
     if (myrank%sqrt_p==0){
            //printf("%d has no left\n",myrank);
     }
     else{
          MPI_Recv (recv_left, numVectors, newvtype, myrank-1, myrank, MPI_COMM_WORLD, &status);
          // printf("%d recv left from %d\n",myrank,myrank-1);
          // for(int i=0;i<array_of_sizes;i++){
          //           printf("[%lf] ",recv_left[i]);
          //           }
          //   printf("\n");

     }
     // printf("\n");




     // solve Halo center and initializing up,down, left, and right
     // double temp_data[array_of_sizes][array_of_sizes];
     double **temp_data;
     temp_data = alloc_2d_int(array_of_sizes,array_of_sizes);

     double up_data;
     double down_data;
     double left_data;
     double right_data;

     for (int i=0;i<array_of_sizes;i++){
          for (int j=0;j<array_of_sizes;j++){
                  temp_data[i][j] = vector_data[i][j];
                  // updating halo values
                  if ((i>0 && i<array_of_sizes-1) && (j>0 && j<array_of_sizes-1)) {
                         up_data = vector_data[i-1][j];
                         down_data = vector_data[i+1][j];
                         left_data = vector_data[i][j-1];
                         right_data = vector_data[i][j+1];
                        temp_data[i][j] = (up_data + down_data + left_data + right_data)/4;
                    }
                  else{
                          if (i==0){
                              // myrank has no up
                              up_data = recv_up[j];
                             }
                          else{
                              up_data = vector_data[i-1][j];
                          }
                          if (i == array_of_sizes-1){
                              // myrank has no down
                              down_data = recv_down[j];
                             }
                          else{
                              down_data = vector_data[i+1][j];

                          }
                          if (j==array_of_sizes-1){
                              // myrank has no right
                              right_data = recv_right[i];
                             }
                          else{
                              right_data = vector_data[i][j+1];
                          }
                          if (j==0){
                                // myrank has no left
                              left_data = recv_left[i];
                             }
                          else{
                              left_data = vector_data[i][j-1];
                          }
                      // printf("%d %d %d\n",i,j,myrank );
                      // printf("%lf %lf %lf %lf\n",up_data,down_data,left_data,right_data );
                      temp_data[i][j] = (up_data + down_data + left_data + right_data)/4;
                      }
            }
      }
      // update array after computation 
      for (int i = 0; i < array_of_sizes; i++)
        {
            for (int j = 0; j < array_of_sizes; j++)
            {
              vector_data[i][j] = temp_data[i][j];
              // printf("%lf ", vector_data[i][j] );
            }
          // printf("\n");
        }
free(temp_data);
MPI_Type_free (&newvtype);
}// ========================== END OF ITERATION FOR MPI VECTOR =================================


// printing communication and computaion time of MPI_PACKED
global_time = 0.00000;
eTime = MPI_Wtime();
time = eTime- sTime;

MPI_Reduce(&time, &global_time, 1, MPI_DOUBLE, MPI_MAX, root_rank, MPI_COMM_WORLD);

if(myrank == root_rank)
  {
    printf("Maximum time taken by %d processes with %d X %d data points using MPI_VECTOR: %lf.\n",size,array_of_sizes,array_of_sizes, global_time);
  }







// ============================ Single Communication ================================


double **single_data;
single_data = alloc_2d_int(array_of_sizes,array_of_sizes);  
sTime = MPI_Wtime();

for (int step = 0; step < steps; step++)
{

      MPI_Status status;
      MPI_Request send_request,recv_request;

      // up, down, left and right for send
      double up[array_of_sizes];
      double down[array_of_sizes];
      double right[array_of_sizes];
      double left[array_of_sizes];

      //  up, down, left and right for receive
      double recv_up[array_of_sizes];
      double recv_down[array_of_sizes];
      double recv_right[array_of_sizes];
      double recv_left[array_of_sizes];

      // initialize up,down , left and right
      for (int i = 0; i < array_of_sizes; ++i)  {
      recv_up[i]= (double)(0);
      recv_down[i]= (double)(0);
      recv_right[i]= (double)(0);
      recv_left[i]= (double)(0);
      }


      for (int i=0; i<array_of_sizes; i++){
            for (int j=0; j<array_of_sizes; j++){
                  if (step==0)
                  {
                    single_data[i][j] = genrate_random(100);

                  }
                  
                  if(i==0 && j<array_of_sizes){
                    up[j] = single_data[i][j];
                  }

                  if(j==0 && i<array_of_sizes){
                    left[i] = single_data[i][j];
                  }

                  if(i == array_of_sizes-1 && j< array_of_sizes){
                    down[j] = single_data[i][j];
                  }

                  if(j == array_of_sizes-1 && i<array_of_sizes){
                    right[i] = single_data[i][j];
                  }
            }
        }


       // sending left, right, up and down to parrallel process
       if (myrank-sqrt_p <0){
       // printf("%d has no up\n",myrank);
       }
       else{
        // setting dest and rank same
          for (int i = 0; i < array_of_sizes; i++){
              MPI_Isend (&up[i], 1, MPI_DOUBLE, myrank-sqrt_p, myrank-sqrt_p+i , MPI_COMM_WORLD,&send_request);
              MPI_Wait(&send_request, &status);
            }
            // printf("up send\n");
       }
       if (myrank+sqrt_p >= no_of_processes){
         //     printf("%d has no down\n",myrank);
       }
       else{
          for (int i = 0; i < array_of_sizes; i++){
              MPI_Isend (&down[i], 1, MPI_DOUBLE, myrank+sqrt_p, myrank+sqrt_p+i , MPI_COMM_WORLD,&send_request);
              MPI_Wait(&send_request, &status);
            }
            // printf("wait done\n");
      }
       if ((myrank+1)%sqrt_p == 0){
         //     printf("%d has no right\n",myrank);
       }
       else{
          for (int i = 0; i < array_of_sizes; i++){
              MPI_Isend (&right[i], 1, MPI_DOUBLE, myrank+1, myrank+1+i , MPI_COMM_WORLD,&send_request);
              MPI_Wait(&send_request, &status);
            }
            // printf("wait done\n");
      }
       if (myrank%sqrt_p==0){
        //      printf("%d has no left\n",myrank);
       }
       else{
      for (int i = 0; i < array_of_sizes; i++){
          MPI_Isend (&left[i], 1, MPI_DOUBLE, myrank-1, myrank-1+i , MPI_COMM_WORLD,&send_request);
        MPI_Wait(&send_request, &status);
          }
      // printf("wait done\n");

       }


//==================================================================================================

    if (myrank-sqrt_p < 0){
      //      printf("%d has no up\n",myrank);
      }
      else{
            for (int i = 0; i < array_of_sizes; i++){
              MPI_Recv (&recv_up[i], 1, MPI_DOUBLE, myrank-sqrt_p, myrank+i, MPI_COMM_WORLD, &status);

            }
            // printf("wait done\n");
              // printf("%d recv up from %d\n",myrank,myrank-sqrt_p);
              // for(int i=0;i<array_of_sizes;i++){
              //         printf("[%lf] ",recv_up[i]);
              //     }
              //   printf("\n");


      }
      if (myrank+sqrt_p >= no_of_processes){
      //    printf("%d has no down\n",myrank);
      }
      else{
        for (int i = 0; i < array_of_sizes; i++){
              MPI_Recv (&recv_down[i], 1, MPI_DOUBLE, myrank+sqrt_p, myrank+i, MPI_COMM_WORLD, &status);
            }
              // MPI_Recv (recv_down, array_of_sizes, MPI_DOUBLE, myrank+sqrt_p, myrank, MPI_COMM_WORLD, &status);
              // printf("%d recv down from %d\n",myrank,myrank+sqrt_p);
              // for(int i=0;i<array_of_sizes;i++){
              //         printf("[%lf] ",recv_down[i]);
              //         }
              // printf("\n");

      }
      if ((myrank+1)%sqrt_p == 0){
        //  printf("%d has no right\n",myrank);
      }
      else{

        for (int i = 0; i < array_of_sizes; i++){
              MPI_Recv (&recv_right[i], 1, MPI_DOUBLE, myrank+1, myrank+i, MPI_COMM_WORLD, &status);

            }
              // MPI_Recv (recv_right, array_of_sizes, MPI_DOUBLE, myrank+1, myrank, MPI_COMM_WORLD, &status); 
              // printf("%d recv right from %d\n",myrank,myrank+1);
              // for(int i=0;i<array_of_sizes;i++){
              //           printf("[%lf] ",recv_right[i]);
              //           }
              // printf("\n");


      }
      if (myrank%sqrt_p==0){
          //printf("%d has no left\n",myrank);
      }
      else{
        for (int i = 0; i < array_of_sizes; i++){
              MPI_Recv (&recv_left[i], 1, MPI_DOUBLE, myrank-1, myrank+i, MPI_COMM_WORLD, &status);

            }
              // MPI_Recv (recv_left, array_of_sizes, MPI_DOUBLE, myrank-1, myrank, MPI_COMM_WORLD, &status);
              // printf("%d recv left from %d\n",myrank,myrank-1);
              // for(int i=0;i<array_of_sizes;i++){
              //           printf("[%lf] ",recv_left[i]);
              //           }
              //   printf("\n");

      }

// =============== Communication complete =================================



     // solve Halo center and initializing up,down, left, and right
     // double temp_data[array_of_sizes][array_of_sizes];
     double **temp_data;
     temp_data = alloc_2d_int(array_of_sizes,array_of_sizes);

     double up_data;
     double down_data;
     double left_data;
     double right_data;

     for (int i=0;i<array_of_sizes;i++){
          for (int j=0;j<array_of_sizes;j++){
                  temp_data[i][j] = single_data[i][j];
                  // updating halo values
                  if ((i>0 && i<array_of_sizes-1) && (j>0 && j<array_of_sizes-1)) {
                         up_data = single_data[i-1][j];
                         down_data = single_data[i+1][j];
                         left_data = single_data[i][j-1];
                         right_data = single_data[i][j+1];
                        temp_data[i][j] = (up_data + down_data + left_data + right_data)/4;
                    }
                  else{
                          if (i==0){
                              // myrank has no up
                              up_data = recv_up[j];
                             }
                          else{
                              up_data = single_data[i-1][j];
                          }
                          if (i == array_of_sizes-1){
                              // myrank has no down
                              down_data = recv_down[j];
                             }
                          else{
                              down_data = single_data[i+1][j];

                          }
                          if (j==array_of_sizes-1){
                              // myrank has no right
                              right_data = recv_right[i];
                             }
                          else{
                              right_data = single_data[i][j+1];
                          }
                          if (j==0){
                                // myrank has no left
                              left_data = recv_left[i];
                             }
                          else{
                              left_data = single_data[i][j-1];
                          }
                      // printf("%d %d %d\n",i,j,myrank );
                      // printf("%lf %lf %lf %lf\n",up_data,down_data,left_data,right_data );
                      temp_data[i][j] = (up_data + down_data + left_data + right_data)/4;
                      }
            }
      }
      // update array after computation 
      for (int i = 0; i < array_of_sizes; i++)
        {
            for (int j = 0; j < array_of_sizes; j++)
            {
              single_data[i][j] = temp_data[i][j];
              // printf("%lf ", vector_data[i][j] );
            }
          // printf("\n");
        }
free(temp_data);

}// ===================== END OF ITERATION IN SINGLE COMMUNICATION=========================


// printing communication and computaion time of MPI_PACKED
global_time = 0.00000;
eTime = MPI_Wtime();
time = eTime- sTime;

MPI_Reduce(&time, &global_time, 1, MPI_DOUBLE, MPI_MAX, root_rank, MPI_COMM_WORLD);

if(myrank == root_rank)
  {
    printf("Maximum time taken by %d processes with %d X %d data points using SINGLE SEND: %lf.\n",size,array_of_sizes,array_of_sizes, global_time);
  }













//============================== Checking Value =================================================

/*
if (myrank==0)
{
      for (int i = 0; i < array_of_sizes; i++)
        {
            for (int j = 0; j < array_of_sizes; j++)
            {
              // data[i][j] = temp_data[i][j];
              printf("%lf ", data[i][j] );
            }
          printf("\n");
        }

	printf("=======================================================================\n");

     for (int i = 0; i < array_of_sizes; i++)
        {
            for (int j = 0; j < array_of_sizes; j++)
            {
              // vector_data[i][j] = temp_data[i][j];
              printf("%lf ", vector_data[i][j] );
            }
          printf("\n");
        }
  printf("=======================================================================\n");

     for (int i = 0; i < array_of_sizes; i++)
        {
            for (int j = 0; j < array_of_sizes; j++)
            {
              // vector_data[i][j] = temp_data[i][j];
              printf("%lf ", single_data[i][j] );
            }
          printf("\n");
        }
}

*/
  
free(vector_data);
free(single_data);
free(data);

MPI_Finalize();
return 0;

}



// allocating array dynamically

double **alloc_2d_int(int rows, int cols) {
  
  double *data = (double *)malloc(rows*cols*sizeof(double));
  double **array= (double **)malloc(rows*sizeof(double*));
  for (int i=0; i<rows; i++)
      array[i] = &(data[cols*i]);

  return array;
}

double genrate_random(int range){
    double random_value;
    srand ( time ( NULL));
    random_value = (double)(rand()%range);
    return random_value;
}

