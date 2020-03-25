#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>



int nResources,nProcesses;
    int *available;
    int *safesequence;
    int **allocation;
    int **maximum;
/* the remaining need of each customer */
    int **need;
    int pran=0;
/* the available amount of each resource */



pthread_mutex_t lockResources;

pthread_cond_t condition;



/*set maximum demand*/

// void setMaximum(int maximum[nProcesses][nResources]);
/*set allocation*/
//void setAllocation(int allocation[nProcesses][nResources]);
/*calculate Need*/
//void cNeed();
/*calculate available*/
//void cAvailable(int tmp[nResources]);
/*print out the result*/
//void result();
/*check if the request is safe*/
//int safetyCheck();
//int request_resources(int customer_num, int request[]);
//int release_resources(int customer_num, int release[]);
/*prevent race conditions*/
//void* thread_control(void* arg);











int safetyCheck()

{

    int i,j,t;

    int result=1;

    int work[nResources];

    int flag[nResources]={0};

    int finish[nResources] ={0};

    for (i=0; i<nResources;i++) {

        work[i]=available[i];

    }

    for (t=0; t<nProcesses; t++) {

        for (i=0;i<nProcesses;i++) {

            if(finish[i]!=1)

            {

                for (j=0;(j<nResources);j++) {

                    if(need[i][j]>work[j])

                    {

                        break;

                    }

                    else

                        flag[i]++;





                }

                if(flag[i]==nResources)

                {

                    printf("work is: ");

                    for (j=0; j<nResources; j++) {

                        work[j]+=allocation[i][j];

                        printf(" %d ",work[j]);

                    }

                    printf("\n");

                    finish[i]=1;

                    printf("Process %d is finished!\n",i);

                }

            }

        }

    }

    for (i=0;i<nProcesses;i++) {

        if(finish[i]==0)

        {

            result=0;

            printf("It is unsafe!\n");

            break;

        }

    }

   return result;

}



int request_resources(int customer_num, int request[])

{

    int j,flag;

    flag=0;

    for (j=0; j<nResources; j++) {

        if(request[j]>need[customer_num][j])

        {

            printf("Error! Process has exceeded its maximum claim! Request Denied.\n");

            flag = -1;

            break;

        }

    }

    if (flag == 0) {

        for (j=0;j<nResources;j++) {

            if(request[j]>available[j])

            {

                printf("Resources are not available now,P(%d)must wait.\n",customer_num);

                flag = -1;

            }

        }

        if(flag==0){

            for (j=0; j<nResources; j++) {

                available[j]-=request[j];

                allocation[customer_num][j]+=request[j];

                need[customer_num][j]-=request[j];

            }

            if(safetyCheck()==1)

                printf("It is safe!Resources have been allocated.\n");

            else

            {

                printf("It is unsafe!P(%d) has to wait.\n",customer_num);

                for (j=0; j<nResources; j++) {

                    available[j]+=request[j];

                    allocation[customer_num][j]-=request[j];

                    need[customer_num][j]+=request[j];

                }

                flag=-1;

            }



        }

    }

    return flag;

}



int release_resources(int customer_num, int release[])

{

    int j,flag;

    flag=0;

    for (j=0; j<nResources; j++) {

        if(allocation[customer_num][j]<release[j])

        {

            printf("Error!P(%d)don't have that many resources to release.\n",customer_num);

            flag = -1;

            break;

        }

    }

    for (j=0;j<nResources;j++) {

        if (flag==0) {

            available[j]+=release[j];

            allocation[customer_num][j]-=release[j];

            need[customer_num][j]+=release[j];

        }

    }

    if (flag==0) {

        printf("Yes! Resources have been released.\n");

    }

    else

    {

        printf("Error! Resources cannot be released.\n");

    }

    return flag;



}



/*void* thread_control(void* arg)

{

    int i = (int)(size_t)arg;

    int j;

    srand((unsigned)time(NULL));

    int lock_ret = 1;

    int request[NUMBER_OF_RESOURCES]={0, 1, 0};

    int release[NUMBER_OF_RESOURCES]={0, 1, 0};

    lock_ret = pthread_mutex_lock(&mutex);

    do{

        if (lock_ret) {

            printf("lock process %d failed...\n",i);

        }

        else

        {

            printf("lock process %d success!\n",i);

        }

    }while (lock_ret);



    printf("process %d request:\n",i);

    for (j=0; j<NUMBER_OF_RESOURCES; j++) {

        printf("%d ",request[j]);

    }

    printf("\n");

    request_resources(i,request);

    print();

    printf("process %d release:\n",i);

    for (j=0; j<NUMBER_OF_RESOURCES; j++) {

        printf("%d ",release[j]);

    }

    printf("\n");

    release_resources(i,release);

    lock_ret = pthread_mutex_unlock(&mutex);

    printf("Unlock process %d success!\n\n",i);

    return NULL;

}
*/
void* processCode(void *arg) {

        int p = *((int *) arg);



	// lock resources

        pthread_mutex_lock(&lockResources);



        // condition check

        while(p != safesequence[pran])

                pthread_cond_wait(&condition, &lockResources);



	// process

        printf("\n--> Process %d", p+1);

        printf("\n\tAllocated : ");

        for(int i=0; i<nResources; i++)

                printf("%3d", allocation[p][i]);



        printf("\n\tNeeded    : ");

        for(int i=0; i<nResources; i++)

                printf("%3d", need[p][i]);



        printf("\n\tAvailable : ");

        for(int i=0; i<nResources; i++)

                printf("%3d", available[i]);



        printf("\n"); sleep(1);



        printf("\tResource Allocated!");

        printf("\n"); sleep(1);

        printf("\tProcess Code Running...");

        printf("\n"); sleep(rand()%3 + 2); // process code

        printf("\tProcess Code Completed...");

        printf("\n"); sleep(1);

        printf("\tProcess Releasing Resource...");

        printf("\n"); sleep(1);

        printf("\tResource Released!");



	for(int i=0; i<nResources; i++)

                available[i] += allocation[p][i];



        printf("\n\tNow Available : ");

        for(int i=0; i<nResources; i++)

                printf("%3d",available[i]);

        printf("\n\n");



        sleep(1);



	// condition broadcast

        pran++;

        pthread_cond_broadcast(&condition);

        pthread_mutex_unlock(&lockResources);

	pthread_exit(NULL);

}
int main(int argc, char const *argv[]) {
    printf("Enter the number of resources");
    scanf("%d",&nResources);
    printf("Enter the number of processes");
    scanf("%d",&nProcesses);
    //int available[nResources];
    //int safesequence[nProcesses];
	
/*the maximum demand of each customer */
//int maximum[nProcesses][nResources];
/* the amount currently allocated to each customer */
//int allocation[nProcesses][nResources];
/* the remaining need of each customer */
//int need[nProcesses][nResources];
/*mutex*/

    int i;

    int temp[nResources];

    pthread_t tid[nProcesses];

    pthread_mutex_init(&lockResources,NULL);
    for(i=0;i<argc-1;i++)

    {

        temp[i] = atoi(argv[i+1]);

    }

    //setAllocation(allocation);

    //setMaximum(maximum);

    //cNeed();

    //cAvailable(temp);

    //print();
    allocation = (int **)malloc(nProcesses * sizeof(*allocation));

        for(int i=0; i<nProcesses; i++)

                allocation[i] = (int *)malloc(nResources * sizeof(**allocation));

        for(int i=0; i<nProcesses; i++) {

                printf("\nResource allocated to process %d (R1 R2 ...)? ", i+1);

                for(int j=0; j<nResources; j++)

                        scanf("%d", &allocation[i][j]);

        }

        printf("\n");            

    
  /*  void setAllocation(int allocation[][])

{

    printf("Please enter the allocation:\n");

    int i,j;

    for (i=0;i<nProcesses;i++) {

        for (j=0; j<nResources;j++) {

            scanf("%d",&allocation[i][j]);

        }

    }

}
*/
 maximum = (int **)malloc(nProcesses * sizeof(*maximum));

        for(int i=0; i<nProcesses; i++)

                maximum[i] = (int *)malloc(nResources * sizeof(**maximum));
// maximum required resources

        for(int i=0; i<nProcesses; i++) {

                printf("\nMaximum resource required by process %d (R1 R2 ...)? ", i+1);

                for(int j=0; j<nResources; j++)

                        scanf("%d", &maximum[i][j]);

        }


/*void setMaximum(int maximum[][])

{

    printf("Please enter the maximum:\n");

    int i,j;

    for (i=0;i<nProcesses;i++) {

        for (j=0; j<nResources;j++) {

            scanf("%d",&maximum[i][j]);

        }

    }

}
*/
/*void cNeed()

{

    int i,j;
*/
available = (int *)malloc(nResources * sizeof(*available));

        printf("\nCurrently Available resources (R1 R2 ...)? ");

        for(int i=0; i<nResources; i++)

                scanf("%d", &available[i]);
    printf("\n");
//need
need = (int **)malloc(nProcesses * sizeof(*need));

        for(int i=0; i<nProcesses; i++)

                need[i] = (int *)malloc(nResources * sizeof(**need));

    for (int i=0;i<nProcesses;i++) {

        for (int j=0; j<nResources;j++) {

            need[i][j] = maximum[i][j]-allocation[i][j];

        }

    }


/*
void cAvailable(int temp[])

{

    int i,j;

    int sum[nResources]={0};

    for (int i=0;i<nResources;i++) {

        for (int j=0; j<nProcesses;j++) {

            sum[i]+=allocation[j][i];

        }



    }

    for (i=0;i<nResources; i++) {

        available[i]=temp[i]-sum[i];

    }




void print()

{

    int i,j;
*/



    for (int i=0;i<nProcesses;i++) {

        for (int j=0;j<nResources;j++) {

            printf(" %2d",allocation[i][j]);

        }

        printf("     ");

        for (int j=0;j<nResources;j++) {

            printf("%2d ",maximum[i][j]);

        }

    printf("\n");

    }

    /*printf("-----Available-----\n");

    for (int i=0; i<nResources; i++) {

        printf(" %2d",available[i]);

    }
*/


    printf("-----Need-----\n");

    for (int i=0; i<nProcesses; i++) {

        for (int j=0; j<nResources; j++) {

            printf(" %2d",need[i][j]);

        }

        printf("\n");

    }
    
    	// get safe sequence

	safesequence = (int *)malloc(nProcesses * sizeof(*safesequence));

        for(int i=0; i<nProcesses; i++) safesequence[i] = -1;



        if(!safetyCheck()) {

                printf("\nUnsafe State! The processes leads the system to a unsafe state.\n\n");

                exit(-1);

        }



        printf("\n\nSafe Sequence Found : ");

        for(int i=0; i<nProcesses; i++) {

                printf("%-3d", safesequence[i]+1);

        }



        printf("\nExecuting Processes...\n\n");

        sleep(1);

	




    /*create thread*/

    for (i=0; i<nProcesses; i++)

    {



        pthread_create(&tid[i],NULL,processCode,(void*)(size_t)i);

    }

    /*join thread */

    for (i=0; i<nProcesses; i++)

    {

        pthread_join(tid[i],NULL);

    }

    return 0;

}
