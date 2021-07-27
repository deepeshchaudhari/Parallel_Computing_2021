#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>


// global variable
int gl_rank,gl_numtasks,gl_name_len;
char gl_name[10];

// toplogy info in array

int topology[100] = {  0,1,1,1,1,1,1,1,1,1,
                       1,1,2,1,1,1,2,2,2,2,
                       2,2,2,2,2,2,2,2,2,2,
                       1,2,3,3,3,3,3,3,3,3,
                       3,3,3,3,4,3,4,4,4,4,
                       4,4,4,4,0,4,0,4,4,4,
                       4,5,5,5,5,5,5,5,5,5,
                       5,5,5,5,5,5,5,5,1,1,
                       6,6,6,6,6,6,6,6,6,6,
                       6,6,0,0,0,0,0,0,0,0
                      };



// ---------- Optimized Bcast -------------------------
double optimized_bcast(int D, int ppn,
                       int *node_1_rank, int top1,
                       int *node_2_rank, int top2,
                       int *node_3_rank, int top3,
                       int *node_4_rank, int top4,
                       int *node_5_rank, int top5,
                       int *node_6_rank, int top6
                      )
{


    int rank;
    int arrSize = D;
    arrSize = (arrSize*1000)/8;
    double senddata[arrSize];

    // get communicator size
    MPI_Comm_size (MPI_COMM_WORLD, &gl_numtasks);
    // get my rank
    MPI_Comm_rank (MPI_COMM_WORLD, &gl_rank);
    // initialize data for communication
    if (gl_rank==0)
      {
        
      for (int i = 0; i < arrSize; ++i)
        {
          senddata[i]= (double)rand();
        }
      }

    // create a new group
    MPI_Group g_group;
    MPI_Comm_group (MPI_COMM_WORLD, &g_group);

    // leader communicator and group initialization
    int i=0;
    int leader_array[10];
    for (int i = 0; i < 10; ++i)
    {
      leader_array[i] = -1;
    }

    MPI_Group leader_group;
    MPI_Comm leader_group_comm;

    // group 1 communicator and group initialization
    MPI_Group group_1;
    MPI_Comm group_1_comm;

    // group 2 communicator and group initialization
    MPI_Group group_2;
    MPI_Comm group_2_comm;

    // group 3 communicator and group initialization
    MPI_Group group_3;
    MPI_Comm group_3_comm;

    // group 4 communicator and group initialization
    MPI_Group group_4;
    MPI_Comm group_4_comm;

    // group 5 communicator and group initialization
    MPI_Group group_5;
    MPI_Comm group_5_comm;

    // group 6 communicator and group initialization
    MPI_Group group_6;
    MPI_Comm group_6_comm;
    double sTime = MPI_Wtime();

    if (top1>0)
     {
       /* creating group for cluster 1 */
       MPI_Group_incl (g_group, top1, node_1_rank, &group_1);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_1, 123, &group_1_comm);
       leader_array[i]=node_1_rank[0];
       i++;
     } 
    if (top2>0)
     {
       /* creating group for cluster 2 */
       MPI_Group_incl (g_group, top2, node_2_rank, &group_2);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_2, 123, &group_2_comm);
       leader_array[i]=node_2_rank[0];
       i++;
     } 
    if (top3>0)
     {
       /* creating group for cluster 3 */
       MPI_Group_incl (g_group, top3, node_3_rank, &group_3);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_3, 123, &group_3_comm);
       leader_array[i]=node_3_rank[0];
       i++;
     } 
    if (top4>0)
     {
       /* creating group for cluster 4 */
       MPI_Group_incl (g_group, top4, node_4_rank, &group_4);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_4, 123, &group_4_comm);
       leader_array[i]=node_4_rank[0];
       i++;
     } 
    if (top5>0)
     {
       /* creating group for cluster 5 */
       MPI_Group_incl (g_group, top5, node_5_rank, &group_5);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_5, 123, &group_5_comm);
       leader_array[i]=node_5_rank[0];
       i++;

     } 
    if (top6>0)
     {
       /* creating froup for cluster 6 */
       MPI_Group_incl (g_group, top6, node_6_rank, &group_6);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_6, 123, &group_6_comm);
       leader_array[i]=node_6_rank[0];
       i++;
     }
     if (i>0)
     {
       /* creating froup for leader */
       MPI_Group_incl (g_group, i, leader_array, &leader_group);
       MPI_Comm_create_group (MPI_COMM_WORLD, leader_group, 123, &leader_group_comm);
     }

// ======================== Broadcasting data to each leader ======================
      int leader_size,leader_rank;
      int data;
      for (int i = 0; i < 10; ++i)
      {
        if (gl_rank == leader_array[i])
        {
          MPI_Comm_size (leader_group_comm, &leader_size);
          MPI_Comm_rank (leader_group_comm, &leader_rank);
          MPI_Bcast(senddata, arrSize, MPI_DOUBLE, 0, leader_group_comm);
        }
      }

// =========================== Getting size of group communicator ==================
    rank = gl_rank;
    int cluster_size, cluster_rank;
    int max_group_size=0;

    if (top1>max_group_size)
    {
      max_group_size=top1;
    }
    if (top2>max_group_size)
    {
      max_group_size=top2;
    }
    if (top3>max_group_size)
    {
      max_group_size=top3;
    }
    if (top4>max_group_size)
    {
      max_group_size=top4;
    }
    if (top5>max_group_size)
    {
      max_group_size=top5;
    }
    if (top6>max_group_size)
    {
      max_group_size=top6;
    }
    
    // ======================= Broadcasting data to each group ====================

    for (int i = 0; i < max_group_size; ++i)
    {
          if (gl_rank== node_1_rank[i])
          {
            MPI_Comm_size (group_1_comm, &cluster_size);
            MPI_Comm_rank (group_1_comm, &cluster_rank);
            MPI_Bcast(senddata, arrSize, MPI_DOUBLE, 0,group_1_comm);
          }
          if (gl_rank== node_2_rank[i])
          {
            MPI_Comm_size (group_2_comm, &cluster_size);
            MPI_Comm_rank (group_2_comm, &cluster_rank);
            MPI_Bcast(senddata, arrSize, MPI_DOUBLE, 0,group_2_comm);
          }
          if (gl_rank== node_3_rank[i])
          {
            MPI_Comm_size (group_3_comm, &cluster_size);
            MPI_Comm_rank (group_3_comm, &cluster_rank);
            MPI_Bcast(senddata, arrSize, MPI_DOUBLE, 0,group_3_comm);
          }
          if (gl_rank== node_4_rank[i])
          {
            MPI_Comm_size (group_4_comm, &cluster_size);
            MPI_Comm_rank (group_4_comm, &cluster_rank);
            MPI_Bcast(senddata, arrSize, MPI_DOUBLE, 0,group_4_comm);
          }
          if (gl_rank== node_5_rank[i])
          {
            MPI_Comm_size (group_5_comm, &cluster_size);
            MPI_Comm_rank (group_5_comm, &cluster_rank);
            MPI_Bcast(senddata, arrSize, MPI_DOUBLE, 0,group_5_comm);
          }
          if (gl_rank== node_6_rank[i])
          {
            MPI_Comm_size (group_6_comm, &cluster_size);
            MPI_Comm_rank (group_6_comm, &cluster_rank);
            MPI_Bcast(senddata, arrSize, MPI_DOUBLE, 0,group_6_comm);
          }
    }


    double eTime;
    eTime = MPI_Wtime();
    MPI_Barrier(MPI_COMM_WORLD);
    return (eTime-sTime);
}

// ================= Normal Broadcast fuunction =======================

double bcast(int D, int ppn){

  int rank;
  int arrSize = D;
  arrSize = (arrSize*1000)/8;
  double senddata[arrSize];
  rank = gl_rank;

  // initialize data for communication
  if (gl_rank==0)
  {
  for (int i = 0; i < arrSize; ++i)
    {
      senddata[i]= (double)rand();
    }
  }

  // Bcast has to be called by all processes
  double sTime = MPI_Wtime();
  MPI_Bcast(senddata, arrSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  double eTime = MPI_Wtime();

  return (eTime-sTime);
}


// ================= Optimized Gather Function ==================================

double optimized_gather(int D, int ppn,
                        int *node_1_rank, int top1,
                        int *node_2_rank, int top2,
                        int *node_3_rank, int top3,
                        int *node_4_rank, int top4,
                        int *node_5_rank, int top5,
                        int *node_6_rank, int top6){

    int rank;
    int arrSize = (D*1000)/8;
    arrSize = (int)arrSize/gl_numtasks;
    double senddata[arrSize];


    // get communicator size
    MPI_Comm_size (MPI_COMM_WORLD, &gl_numtasks);
    // get my rank
    MPI_Comm_rank (MPI_COMM_WORLD, &gl_rank);
    // initialize data
      for (int i = 0; i < arrSize; ++i)
        {
          senddata[i]= (double)rand();
        }

    // create a new group
    MPI_Group g_group;
    MPI_Comm_group (MPI_COMM_WORLD, &g_group);

    // Initializing leader communicator and group 
    int i=0;
    int leader_array[10];
    for (int i = 0; i < 10; ++i)
    {
      leader_array[i] = -1;
    }

    MPI_Group leader_group;
    MPI_Comm leader_group_comm;

    // group 1 communicator and group initialization
    MPI_Group group_1;
    MPI_Comm group_1_comm;

    // group 2 communicator and group initialization
    MPI_Group group_2;
    MPI_Comm group_2_comm;

    // group 3 communicator and group initialization
    MPI_Group group_3;
    MPI_Comm group_3_comm;

    // group 4 communicator and group initialization
    MPI_Group group_4;
    MPI_Comm group_4_comm;

    // group 5 communicator and group initialization
    MPI_Group group_5;
    MPI_Comm group_5_comm;

    // group 6 communicator and group initialization
    MPI_Group group_6;
    MPI_Comm group_6_comm;

    if (top1>0)
     {
       /* creating froup for cluster 1 */
       MPI_Group_incl (g_group, top1, node_1_rank, &group_1);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_1, 123, &group_1_comm);
       leader_array[i]=node_1_rank[0];
       i++;
     } 
    if (top2>0)
     {
       /* creating froup for cluster 2 */
       MPI_Group_incl (g_group, top2, node_2_rank, &group_2);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_2, 123, &group_2_comm);
       leader_array[i]=node_2_rank[0];
       i++;
     } 
    if (top3>0)
     {
       /* creating froup for cluster 3 */
       MPI_Group_incl (g_group, top3, node_3_rank, &group_3);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_3, 123, &group_3_comm);
       leader_array[i]=node_3_rank[0];
       i++;
     } 
    if (top4>0)
     {
       /* creating froup for cluster 4 */
       MPI_Group_incl (g_group, top4, node_4_rank, &group_4);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_4, 123, &group_4_comm);
       leader_array[i]=node_4_rank[0];
       i++;
     } 
    if (top5>0)
     {
       /* creating froup for cluster 5 */
       MPI_Group_incl (g_group, top5, node_5_rank, &group_5);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_5, 123, &group_5_comm);
       leader_array[i]=node_5_rank[0];
       i++;

     } 
    if (top6>0)
     {
       /* creating froup for cluster 6 */
       MPI_Group_incl (g_group, top6, node_6_rank, &group_6);
       MPI_Comm_create_group (MPI_COMM_WORLD, group_6, 123, &group_6_comm);
       leader_array[i]=node_6_rank[0];
       i++;

     }
     if (i>0)
     {
       /* creating froup for leader */
       MPI_Group_incl (g_group, i, leader_array, &leader_group);
       MPI_Comm_create_group (MPI_COMM_WORLD, leader_group, 123, &leader_group_comm);

     }


    // =========================== Gather data in leader node of each group ==================
    rank = gl_rank;
    int cluster_size, cluster_rank;
    int max_group_size=0;

    if (top1>max_group_size)
    {
      max_group_size=top1;
    }
    if (top2>max_group_size)
    {
      max_group_size=top2;
    }
    if (top3>max_group_size)
    {
      max_group_size=top3;
    }
    if (top4>max_group_size)
    {
      max_group_size=top4;
    }
    if (top5>max_group_size)
    {
      max_group_size=top5;
    }
    if (top6>max_group_size)
    {
      max_group_size=top6;
    }
    double sTime = MPI_Wtime();
    double recvMessage[arrSize * gl_numtasks];
    double received_gather[arrSize*gl_numtasks];
    for (int i = 0; i < max_group_size; ++i)
    {
          if (gl_rank== node_1_rank[i])
          {
            MPI_Comm_size (group_1_comm, &cluster_size);
            MPI_Comm_rank (group_1_comm, &cluster_rank);
            MPI_Gather (senddata, arrSize, MPI_DOUBLE, recvMessage, arrSize, MPI_DOUBLE, 0, group_1_comm);
          }
          if (gl_rank== node_2_rank[i])
          {
            MPI_Comm_size (group_2_comm, &cluster_size);
            MPI_Comm_rank (group_2_comm, &cluster_rank);
            MPI_Gather (senddata, arrSize, MPI_DOUBLE, recvMessage, arrSize, MPI_DOUBLE, 0, group_2_comm);
          }
          if (gl_rank== node_3_rank[i])
          {
            MPI_Comm_size (group_3_comm, &cluster_size);
            MPI_Comm_rank (group_3_comm, &cluster_rank);
            MPI_Gather (senddata, arrSize, MPI_DOUBLE, recvMessage, arrSize, MPI_DOUBLE, 0, group_3_comm);
          }
          if (gl_rank== node_4_rank[i])
          {
            MPI_Comm_size (group_4_comm, &cluster_size);
            MPI_Comm_rank (group_4_comm, &cluster_rank);
            MPI_Gather (senddata, arrSize, MPI_DOUBLE, recvMessage, arrSize, MPI_DOUBLE, 0, group_4_comm);
            
          }
          if (gl_rank== node_5_rank[i])
          {
            MPI_Comm_size (group_5_comm, &cluster_size);
            MPI_Comm_rank (group_5_comm, &cluster_rank);
            MPI_Gather (senddata, arrSize, MPI_DOUBLE, recvMessage, arrSize, MPI_DOUBLE, 0, group_5_comm);
            
          }
          if (gl_rank== node_6_rank[i])
          {
            MPI_Comm_size (group_6_comm, &cluster_size);
            MPI_Comm_rank (group_6_comm, &cluster_rank);
            MPI_Gather (senddata, arrSize, MPI_DOUBLE, recvMessage, arrSize, MPI_DOUBLE, 0, group_6_comm);
            
          }
    }

    // =============== Gathering data by leader nodes in leader  group ====================
    int leader_size,leader_rank;
    for (int i = 0; i < 10; ++i)
    {
      if (gl_rank == leader_array[i] && cluster_rank == 0)
      {
        MPI_Comm_size (leader_group_comm, &leader_size);
        MPI_Comm_rank (leader_group_comm, &leader_rank);
        MPI_Gather (recvMessage, arrSize*cluster_size, MPI_DOUBLE, received_gather, arrSize*cluster_size, MPI_DOUBLE, 0, leader_group_comm);
      }
    }

    double eTime= MPI_Wtime();
  return eTime-sTime;
}

// ==================== Normal Gather Function ===============================================

double gather(int D,int ppn){
  int rank,numtasks;
  rank = gl_rank;
  numtasks = gl_numtasks;
  

  int arrSize = (D*1000)/8;
  arrSize = (int)(arrSize/gl_numtasks);
  double senddata[arrSize];
  
  // initialize array 
  for (int i = 0; i < arrSize; i++) {
    senddata[i] = (double)rand();
  }
  
  // Gather at root
  double sTime,eTime;
  sTime = MPI_Wtime();
  double recvMessage[numtasks*arrSize]; //significant at the root process
  MPI_Gather (&senddata, arrSize, MPI_DOUBLE, recvMessage, arrSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  eTime = MPI_Wtime();
  return (eTime - sTime);
}

// ===================== Normal Reduce Function ============================================
double reduce(int D, int ppn){
  
  double sTime,eTime;
  sTime= MPI_Wtime();

  int arrSize = D;
  arrSize = (arrSize*1000)/8;
  double senddata[arrSize], recv_data[arrSize];

  // initialize array
  for (int i = 0; i < arrSize; i++) {
    senddata[i] = (double)i;
    }

 MPI_Reduce(senddata, recv_data, arrSize, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD); // find sum of sendvals
 eTime=MPI_Wtime();
 return eTime-sTime;
}

// ===================== Optimized Reduce Func
double optimized_reduce(int D, int ppn,
                        int *node_1_rank, int top1,
                        int *node_2_rank, int top2,
                        int *node_3_rank, int top3,
                        int *node_4_rank, int top4,
                        int *node_5_rank, int top5,
                        int *node_6_rank, int top6){
      double sTime,eTime;
      int arrSize = D;
      arrSize = (arrSize*1000)/8;
      double senddata[arrSize];

      // get communicator size
      MPI_Comm_size (MPI_COMM_WORLD, &gl_numtasks);
      // get my rank
      MPI_Comm_rank (MPI_COMM_WORLD, &gl_rank);
      // initialize data
      for (int i = 0; i < arrSize; ++i)
        {
          senddata[i]= i;
        }


      // create a new group
      MPI_Group g_group;
      MPI_Comm_group (MPI_COMM_WORLD, &g_group);
      // leader communicator and group initialization
      int i=0;
      int leader_array[10];
      for (int i = 0; i < 10; ++i)
      {
        leader_array[i] = -1;
      }

      MPI_Group leader_group;
      MPI_Comm leader_group_comm;

      // group 1 communicator and group initialization
      MPI_Group group_1;
      MPI_Comm group_1_comm;

      // group 2 communicator and group initialization
      MPI_Group group_2;
      MPI_Comm group_2_comm;

      // group 3 communicator and group initialization
      MPI_Group group_3;
      MPI_Comm group_3_comm;

      // group 4 communicator and group initialization
      MPI_Group group_4;
      MPI_Comm group_4_comm;

      // group 5 communicator and group initialization
      MPI_Group group_5;
      MPI_Comm group_5_comm;

      // group 6 communicator and group initialization
      MPI_Group group_6;
      MPI_Comm group_6_comm;

      if (top1>0)
       {
          // creating froup for cluster 1 
         MPI_Group_incl (g_group, top1, node_1_rank, &group_1);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_1, 123, &group_1_comm);
         leader_array[i]=node_1_rank[0];
         i++;
       } 
      if (top2>0)
       {
         /* creating froup for cluster 2 */
         MPI_Group_incl (g_group, top2, node_2_rank, &group_2);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_2, 123, &group_2_comm);
         leader_array[i]=node_2_rank[0];
         i++;
       } 
      if (top3>0)
       {
         /* creating froup for cluster 3 */
         MPI_Group_incl (g_group, top3, node_3_rank, &group_3);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_3, 123, &group_3_comm);
         leader_array[i]=node_3_rank[0];
         i++;
       } 
      if (top4>0)
       {
         /* creating froup for cluster 4 */
         MPI_Group_incl (g_group, top4, node_4_rank, &group_4);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_4, 123, &group_4_comm);
         leader_array[i]=node_4_rank[0];
         i++;

       } 
      if (top5>0)
       {
         /* creating froup for cluster 5 */
         MPI_Group_incl (g_group, top5, node_5_rank, &group_5);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_5, 123, &group_5_comm);
         leader_array[i]=node_5_rank[0];
         i++;

       } 
      if (top6>0)
       {
         /* creating froup for cluster 6 */
         MPI_Group_incl (g_group, top6, node_6_rank, &group_6);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_6, 123, &group_6_comm);
         leader_array[i]=node_6_rank[0];
         i++;

       }
       if (i>0)
       {
         /* creating froup for leader */
         MPI_Group_incl (g_group, i, leader_array, &leader_group);
         MPI_Comm_create_group (MPI_COMM_WORLD, leader_group, 123, &leader_group_comm);

       }

    // =========================== Reduce data in leader node in each group=============
    int cluster_size, cluster_rank;
    int max_group_size=0;

    if (top1>max_group_size)
    {
      max_group_size=top1;
    }
    if (top2>max_group_size)
    {
      max_group_size=top2;
    }
    if (top3>max_group_size)
    {
      max_group_size=top3;
    }
    if (top4>max_group_size)
    {
      max_group_size=top4;
    }
    if (top5>max_group_size)
    {
      max_group_size=top5;
    }
    if (top6>max_group_size)
    {
      max_group_size=top6;
    }
     sTime = MPI_Wtime();

    double recv_data[arrSize];
    double received_reduce[arrSize];
    for (int i = 0; i < max_group_size; ++i)
    {
          if (gl_rank== node_1_rank[i])
          {
            MPI_Comm_size (group_1_comm, &cluster_size);
            MPI_Comm_rank (group_1_comm, &cluster_rank);
            MPI_Reduce(senddata, recv_data, arrSize, MPI_DOUBLE, MPI_SUM, 0, group_1_comm); // find sum of sendvals

          }
          if (gl_rank== node_2_rank[i])
          {
            MPI_Comm_size (group_2_comm, &cluster_size);
            MPI_Comm_rank (group_2_comm, &cluster_rank);
            MPI_Reduce(senddata, recv_data, arrSize, MPI_DOUBLE, MPI_SUM, 0, group_2_comm); // find sum of sendvals

          }
          if (gl_rank== node_3_rank[i])
          {
            MPI_Comm_size (group_3_comm, &cluster_size);
            MPI_Comm_rank (group_3_comm, &cluster_rank);
            MPI_Reduce(senddata, recv_data, arrSize, MPI_DOUBLE, MPI_SUM, 0, group_3_comm); // find sum of sendvals

          }
          if (gl_rank== node_4_rank[i])
          {
            MPI_Comm_size (group_4_comm, &cluster_size);
            MPI_Comm_rank (group_4_comm, &cluster_rank);
            MPI_Reduce(senddata, recv_data, arrSize, MPI_DOUBLE, MPI_SUM, 0, group_4_comm); // find sum of sendvals

          }
          if (gl_rank== node_5_rank[i])
          {
            MPI_Comm_size (group_5_comm, &cluster_size);
            MPI_Comm_rank (group_5_comm, &cluster_rank);
            MPI_Reduce(senddata, recv_data, arrSize, MPI_DOUBLE, MPI_SUM, 0, group_5_comm); // find sum of sendvals
          }
          if (gl_rank== node_6_rank[i])
          {
            MPI_Comm_size (group_6_comm, &cluster_size);
            MPI_Comm_rank (group_6_comm, &cluster_rank);
            MPI_Reduce(senddata, recv_data, arrSize, MPI_DOUBLE, MPI_SUM, 0, group_6_comm); // find sum of sendvals

          }
    }

    // ===== Reduce data in root node of leader group ==================================================  

    int leader_size,leader_rank;
    for (int i = 0; i < 10; ++i)
    {
      if (gl_rank == leader_array[i] && cluster_rank == 0)
      {
        MPI_Comm_size (leader_group_comm, &leader_size);
        MPI_Comm_rank (leader_group_comm, &leader_rank);
        MPI_Reduce(recv_data, received_reduce, arrSize, MPI_DOUBLE, MPI_SUM, 0, leader_group_comm); // find sum of sendvals

      }
    }

  eTime = MPI_Wtime();
  return eTime-sTime;
}


//==================Normal AlltoAllv Function =======================================

double alltoallv(int D,int ppn){
  // Setup
  double sTime,eTime;
  int rank = gl_rank;
  int P = gl_numtasks;

  sTime = MPI_Wtime();
  // Allocate message at the senders (may be different sized for different processes)
  int N = D/8;

  int send_count[P], send_displ[P];
  int recv_count[P], recv_displ[P];

  int displ = 0; // note that root process is 0 here

  // this information is needed by the root
  double random_value = rand()%N; 

   for (int i = 0; i < P; i++) {
    send_count[i] = (rand() % N)*8 + i ;
    send_displ[i] = displ;
    displ += send_count[i];
   }

  double message[displ]; 
  for (int i = 0; i < displ; i++) {
    message[i] = (double)(rand() % N);
  }

  MPI_Alltoall(send_count, 1, MPI_INT, recv_count, 1, MPI_INT, MPI_COMM_WORLD);
  // every process initializes their local array 

  displ = 0;
   for (int i = 0; i < P; i++) {
    recv_displ[i] = displ;
    displ += recv_count[i];
   }

  int recvMessage[displ];    // significant at the root process
  // receive different counts of elements from different processes
  MPI_Alltoallv (message, send_count, send_displ, MPI_DOUBLE, recvMessage, recv_count, recv_displ, MPI_DOUBLE, MPI_COMM_WORLD);
  eTime = MPI_Wtime();

  return eTime-sTime;
}
//====================== Optimized Alltoallv Function ========================================

double optimized_alltoallV(int D,int ppn,
                        int *node_1_rank, int top1,
                        int *node_2_rank, int top2,
                        int *node_3_rank, int top3,
                        int *node_4_rank, int top4,
                        int *node_5_rank, int top5,
                        int *node_6_rank, int top6){

      double sTime,eTime;      
      // get communicator size 
      MPI_Comm_size (MPI_COMM_WORLD, &gl_numtasks);
      // get my rank
      MPI_Comm_rank (MPI_COMM_WORLD, &gl_rank);
      // create a new group
      MPI_Group g_group;
      MPI_Comm_group (MPI_COMM_WORLD, &g_group);
      // leader communicator and group initialization
      int i=0;
      int leader_array[10];
      for (int i = 0; i < 10; ++i)
      {
        leader_array[i] = -1;
      }

      MPI_Group leader_group;
      MPI_Comm leader_group_comm;

      // group 1 communicator and group initialization
      MPI_Group group_1;
      MPI_Comm group_1_comm;

      // group 2 communicator and group initialization
      MPI_Group group_2;
      MPI_Comm group_2_comm;

      // group 3 communicator and group initialization
      MPI_Group group_3;
      MPI_Comm group_3_comm;

      // group 4 communicator and group initialization
      MPI_Group group_4;
      MPI_Comm group_4_comm;

      // group 5 communicator and group initialization
      MPI_Group group_5;
      MPI_Comm group_5_comm;

      // group 6 communicator and group initialization
      MPI_Group group_6;
      MPI_Comm group_6_comm;

      if (top1>0)
       {
          // creating group for cluster 1 
         MPI_Group_incl (g_group, top1, node_1_rank, &group_1);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_1, 123, &group_1_comm);
         leader_array[i]=node_1_rank[0];
         i++;
       } 
      if (top2>0)
       {
         /* creating group for cluster 2 */
         MPI_Group_incl (g_group, top2, node_2_rank, &group_2);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_2, 123, &group_2_comm);
         leader_array[i]=node_2_rank[0];
         i++;
       } 
      if (top3>0)
       {
         /* creating group for cluster 3 */
         MPI_Group_incl (g_group, top3, node_3_rank, &group_3);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_3, 123, &group_3_comm);
         leader_array[i]=node_3_rank[0];
         i++;
       } 
      if (top4>0)
       {
         /* creating group for cluster 4 */
         MPI_Group_incl (g_group, top4, node_4_rank, &group_4);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_4, 123, &group_4_comm);
         leader_array[i]=node_4_rank[0];
         i++;

       } 
      if (top5>0)
       {
         /* creating group for cluster 5 */
         MPI_Group_incl (g_group, top5, node_5_rank, &group_5);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_5, 123, &group_5_comm);
         leader_array[i]=node_5_rank[0];
         i++;

       } 
      if (top6>0)
       {
         /* creating group for cluster 6 */
         MPI_Group_incl (g_group, top6, node_6_rank, &group_6);
         MPI_Comm_create_group (MPI_COMM_WORLD, group_6, 123, &group_6_comm);
         leader_array[i]=node_6_rank[0];
         i++;

       }
       if (i>0)
       {
         /* creating group for leader */
         MPI_Group_incl (g_group, i, leader_array, &leader_group);
         MPI_Comm_create_group (MPI_COMM_WORLD, leader_group, 123, &leader_group_comm);

       }

    // =========================== Alltoall data in each group ==================
      
    int cluster_size, cluster_rank;
    int max_group_size=0;

    if (top1>max_group_size)
    {
      max_group_size=top1;
    }
    if (top2>max_group_size)
    {
      max_group_size=top2;
    }
    if (top3>max_group_size)
    {
      max_group_size=top3;
    }
    if (top4>max_group_size)
    {
      max_group_size=top4;
    }
    if (top5>max_group_size)
    {
      max_group_size=top5;
    }
    if (top6>max_group_size)
    {
      max_group_size=top6;
    }
     sTime = MPI_Wtime();


      int rank = gl_rank;
      int P = gl_numtasks;
    

      // Allocate message at the senders (may be different sized for different processes)
      int N = D/8;
   
      int send_count[P], send_displ[P];
      int recv_count[P], recv_displ[P];

      int displ = 0;                       // note that root process is 0 here

      //  ============= Alltoall in leader =================================
      // this information is needed by the root
      double random_value = rand()%N; 
      for (int i = 0; i < P; i++) {
          send_count[i] = (rand() % N)*8 + i ;
          send_displ[i] = displ;
          displ += send_count[i];
       }

      double message[displ]; 
      for (int i = 0; i < displ; i++) {
        message[i] = (double)(rand() % N);
      }

      MPI_Alltoall(send_count, 1, MPI_INT, recv_count, 1, MPI_INT, MPI_COMM_WORLD);
      // every process initializes their local array 
      displ = 0;
      for (int i = 0; i < P; i++) {
        recv_displ[i] = displ;
        displ += recv_count[i];
      }


      int recvMessage[displ];    // significant at the root process
      // receive different counts of elements from different processes
      MPI_Alltoallv (message, send_count, send_displ, MPI_DOUBLE, recvMessage, recv_count, recv_displ, MPI_DOUBLE, MPI_COMM_WORLD);
      eTime = MPI_Wtime();

  return eTime-sTime;


}
//====================== Finial Function ========================

void finalize(){
    MPI_Finalize();
}

// ========================= MAIN PROGRAM ========================

int main(int argc, char *argv[]) {

  // init comm_world
  int numtasks, rank, name_len,count;
  // initialize MPI
  MPI_Init (&argc, &argv);
  // get number of tasks
  MPI_Comm_size (MPI_COMM_WORLD, &gl_numtasks);
  // get my rank
  MPI_Comm_rank (MPI_COMM_WORLD, &gl_rank);
  // get preocessor name
  MPI_Get_processor_name(gl_name,&gl_name_len);

  rank = gl_rank;
  numtasks = gl_numtasks;
  name_len = gl_name_len;

  MPI_Status status, sstatus;
  MPI_Request request;


  // declare and initialize groups rank array
  int node_1_rank[100],top1=0;
  int node_2_rank[100],top2=0;
  int node_3_rank[100],top3=0;
  int node_4_rank[100],top4=0;
  int node_5_rank[100],top5=0;
  int node_6_rank[100],top6=0;

  for (int i = 0; i < 100; i++)
        {
          node_1_rank[i]=-1;
          node_2_rank[i]=-1;
          node_3_rank[i]=-1;
          node_4_rank[i]=-1;
          node_5_rank[i]=-1;
          node_6_rank[i]=-1;
        }

  // variable send and recv node toplogy info
  int recv_rank_node[2];
  int send_rank_node[2];

  // finding group info and creating group rank array
  if (rank == 0)    /* code for process 0 */
      {
        int machine_no  = atoi(&gl_name[5]);
        for (int i = 0; i < numtasks; i++)
            {
              if (i==0)
              {
                recv_rank_node[0] = rank;
                recv_rank_node[1] = topology[machine_no];
                    
              }
              else{
                   MPI_Recv(recv_rank_node, 2, MPI_INT, i, 99, MPI_COMM_WORLD, &status);
                   MPI_Get_count (&status, MPI_INT, &count);
            
              }
              
              switch(recv_rank_node[1]){
                    case 1:
                        node_1_rank[top1]= recv_rank_node[0];
                        top1++;
                        break;
                    case 2:
                        node_2_rank[top2]= recv_rank_node[0];
                        top2++;
                        break;

                    case 3:
                        node_3_rank[top3]= recv_rank_node[0];
                        top3++;
                        break;

                    case 4:
                        node_4_rank[top4]= recv_rank_node[0];
                        top4++;
                        break;

                    case 5:
                        node_5_rank[top5]= recv_rank_node[0];
                        top5++;
                        break;
                      
                    case 6:
                        node_6_rank[top6]= recv_rank_node[0];
                        top6++;
                        break;

                    default:
                        break;
                }

            }

      }
      else{
          int machine_no  = atoi(&gl_name[5]);
          send_rank_node[0] = rank;
          send_rank_node[1] = topology[machine_no];
          MPI_Send(send_rank_node, 2, MPI_INT, 0, 99, MPI_COMM_WORLD);
   
      }



      // broadcast cluster ranks once to all process
        MPI_Bcast(node_1_rank, 100, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(node_2_rank, 100, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(node_3_rank, 100, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(node_4_rank, 100, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(node_5_rank, 100, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(node_6_rank, 100, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

      // broadcast top value once to all processes
        MPI_Bcast(&top1, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&top2, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&top3, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&top4, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&top5, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&top6, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);


// ------------------------------------------------------------------
         int D = atoi(argv[1]);
         int ppn =atoi(argv[2]);
    
    // =============== CALL NORMAL BCAST =====================
    double maxtim,opt_maxtim;
    double tim = bcast(D,ppn);
    MPI_Reduce(&tim, &maxtim, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); // find max of sendvals
    MPI_Barrier(MPI_COMM_WORLD);

    // =============== CALL OPTIMIZED BCAST ==================
    double opt_tim = optimized_bcast( D,ppn,
                                      node_1_rank,top1,
                                      node_2_rank,top2,
                                      node_3_rank,top3,
                                      node_4_rank,top4,
                                      node_5_rank,top5,
                                      node_6_rank,top6
                                      );
    MPI_Reduce(&opt_tim, &opt_maxtim, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); // find max of sendvals
    MPI_Barrier(MPI_COMM_WORLD);
    if (gl_rank==0)
    {
       
      printf("Bcast %d %d %d %lf \n",D,ppn,(gl_numtasks/ppn),maxtim);
      printf("OBcast %d %d %d %lf \n",D,ppn,(gl_numtasks/ppn),opt_maxtim);
    }

  // WAIT untill bcast finishes
  MPI_Barrier(MPI_COMM_WORLD);

  // ================= CALL NORMAL GATHER =================== 
  double gather_time,max_gather_time;
  gather_time = gather(D,ppn);
  MPI_Reduce(&gather_time, &max_gather_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); // find max of sendvals
  MPI_Barrier(MPI_COMM_WORLD);

  // ================= CALL OPTIMIZED GATHER ================ 
  double opt_gather_time,max_opt_gather_time;
  opt_gather_time = optimized_gather( D,ppn,
                                      node_1_rank,top1,
                                      node_2_rank,top2,
                                      node_3_rank,top3,
                                      node_4_rank,top4,
                                      node_5_rank,top5,
                                      node_6_rank,top6
                                      );
  MPI_Reduce(&opt_gather_time, &max_opt_gather_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); // find max of sendvals
  if (gl_rank==0)
    {
      printf("Gather %d %d %d %lf \n",D,ppn,(gl_numtasks/ppn),max_gather_time);
      printf("OGather %d %d %d %lf \n",D,ppn,(gl_numtasks/ppn),max_opt_gather_time);
    }
  // WAIT untill Gather finishes
  MPI_Barrier(MPI_COMM_WORLD);


  // ================== CALL NORMAL REDUCE =================
  double reduce_time,max_reduce_time;
  reduce_time = reduce(D,ppn);
  MPI_Reduce(&reduce_time, &max_reduce_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); // find max of sendvals

  // ================== CALL OPTIMIZED REDUCE ==============
  double opt_reduce_time,max_opt_reduce_time;
  opt_reduce_time = optimized_reduce(D,ppn,
                                    node_1_rank,top1,
                                    node_2_rank,top2,
                                    node_3_rank,top3,
                                    node_4_rank,top4,
                                    node_5_rank,top5,
                                    node_6_rank,top6);
  MPI_Reduce(&opt_reduce_time, &max_opt_reduce_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); // find max of sendvals
  if (gl_rank==0)
  {
    printf("Reduce %d %d %d %lf \n",D,ppn,(gl_numtasks/ppn),max_reduce_time);
    printf("OReduce %d %d %d %lf \n",D,ppn,(gl_numtasks/ppn),max_opt_reduce_time);
  }
  // Wait till Reduce finishes
  MPI_Barrier(MPI_COMM_WORLD);
  
  // ================== CALL NORMAL ALLTOALLV ===============
  double alltoallv_time,max_alltoallv_time;
  alltoallv_time = alltoallv(D,ppn);
  MPI_Reduce(&alltoallv_time,&max_alltoallv_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  
  // ================== CALL OPTIMIZED ALLTOALLV ============
  double opt_alltoallv_time,max_opt_alltoallv_time;
  opt_alltoallv_time = optimized_alltoallV(D,ppn,
                                          node_1_rank,top1,
                                          node_2_rank,top2,
                                          node_3_rank,top3,
                                          node_4_rank,top4,
                                          node_5_rank,top5,
                                          node_6_rank,top6);
  MPI_Reduce(&opt_alltoallv_time,&max_opt_alltoallv_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
    
  if (gl_rank==0)
    {
      printf("AlltoAllv %d %d %d %lf \n",D,ppn,(gl_numtasks/ppn),max_alltoallv_time);
      printf("OAlltoAllv %d %d %d %lf \n",D,ppn,(gl_numtasks/ppn),max_opt_alltoallv_time);
    }  

  // done with MPI
  finalize();

  // DONE WITH ASSIGNMENT2!!!
  return 0;
}