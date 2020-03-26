#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdbool.h>
int i=0,j=0,k=0;
int P=0;
int R=0;

pthread_mutex_t mutex;
int *resources,*available,**allocation,**maximum,**need;
int request(int processid,int req[]);
int release(int processid,int rel[]);
int greater(int processid,int req[]);
int releasecorrect(int processid,int rel[]);
int safe();
int corralloc(int req[]);
int *safeSeq;

void *pcode(void *P);
int main(int argc,char const *argv[])
{
	
	
printf("Enter the number of resources");

scanf("%d",&R);
printf("Enter the number of processes");

scanf("%d",&P);

//available size
available = (int *)malloc(R * sizeof(*available));
//max size
maximum = (int **)malloc(P * sizeof(*maximum));
for(i=0; i<P; i++)
	maximum[i] = (int *)malloc(R * sizeof(**maximum));
//allocation size
allocation = (int **)malloc(P * sizeof(*allocation));
for(i=0; i<P; i++)
	allocation[i] = (int *)malloc(R * sizeof(**allocation));
//need size
need = (int **)malloc(P * sizeof(*need));
 for( i=0; i<P; i++)
	need[i] = (int *)malloc(R * sizeof(**need));
//user input
//allocated resources
for( i=0; i<P; i++) {

    printf("\nResource allocated to process %d (R1 R2 ...)? ", i+1);
            printf("\n");
	for( j=0; j<R; j++)
        scanf("%d", &allocation[i][j]);

}
// maximum required resources
	for( i=0; i<P; i++) {
		printf("\nMaximum resource required by process %d (R1 R2 ...)? ", i+1);
		        printf("\n");
		for( j=0; j<R; j++)
			scanf("%d", &maximum[i][j]);

    }
//Available resources
printf("\nCurrently Available resources (R1 R2 ...)? ");
        printf("\n");
	for(i=0; i< R; i++)
		scanf("%d", &available[i]);
//need calculation
for ( i=0;i<P;i++) {

        for ( j=0; j<R;j++) {

            need[i][j] = maximum[i][j]-allocation[i][j];

        }

    }
    	// get safe sequence

	safeSeq = (int *)malloc(P * sizeof(*safeSeq));

        for(i=0; i<P; i++) safeSeq[i] = -1;
         if(!safetyCheck()) {

                printf("\nUnsafe State! The processes leads the system to a unsafe state.\n\n");

                exit(-1);

        }



        printf("\n\nSafe Sequence Found : ");

        for( i=0; i<P; i++) {

                printf("%-3d", safeSeq[i]+1);

        }



        printf("\nExecuting Processes...\n\n");

        sleep(1);

   /*
pthread_mutex_init(&mutex,NULL);
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_t *tid=malloc(sizeof(pthread_t)*P);*/
pthread_t processes[P];

        pthread_attr_t attr;

        pthread_attr_init(&attr);
	int processNumber[P];
	for(i=0; i<P; i++) processNumber[i] = i;
        for(i=0; i<P; i++)
                pthread_create(&processes[i], &attr, pcode, (void *)(&processNumber[i]));
        for(i=0; i<P; i++)
                pthread_join(processes[i], NULL);
        printf("\nAll Processes Finished\n");	

/*threads
for(i=0;i<P;i++)
{
	pthread_create(&tid[i],NULL,pcode,(void*)(size_t)i);
}
for(i=0;i<P;i++)
{
    pthread_join(tid[i],NULL);
}*/
return 0;
}
void *pcode(void *P)
{
	int processid=*(int*)P;
	for(i=0;i<R;i++)
	{
		sleep(1);
		int req[R];
		//lock for global variable
		pthread_mutex_lock(&mutex);
		for(i=0;i<R;i++)
		{
			if(need[processid][i]!=0)
			{
				req[i]=rand() %(need[processid][i]);
			}
			else
			{
				req[i]=0;
			}
		}
	    int reqgrant=request(processid,req);
	    //unlock
	    pthread_mutex_unlock(&mutex);
	    while(reqgrant<0)
	    {
	    	        printf("\n");
			printf("request can not be granted");
	    	sleep(rand()%10);
	    	reqgrant=request(processid,req);
		}
		        printf("\n");
		printf("customer: %d request satisfied",processid);
		
		//release resources
		//sleep(rand()%100);//sleep for 100 milisec
	    int rel[R];
	    pthread_mutex_lock(&mutex);
		for(i=0;i<R;i++)
		{
			if(allocation[processid][i]!=0)
			{
				rel[i]=allocation[processid][i];
			}
			else
			{
				rel[i]=0;
			}
		}
		release(processid,rel);
		//unlock
		pthread_mutex_unlock(&mutex);	    
	}
	
}
int request(int P,int req[])
{
	int j,flag;

    flag=0;

    for (j=0; j<R; j++) {

        if(req[j]>need[P][j])

        {

            printf("Error! Process has exceeded its maximum claim! Request Denied.\n");

            flag = -1;

            break;

        }

    }

    if (flag == 0) {

        for (j=0;j<R;j++) {

            if(req[j]>available[j])

            {

                printf("Resources are not available now,P(%d)must wait.\n",P);

                flag = -1;

            }

        }

        if(flag==0){

            for (j=0; j<R; j++) {

                available[j]-=req[j];

                allocation[P][j]+=req[j];

                need[P][j]-=req[j];

            }

            if(safetyCheck()==1)

                printf("It is safe!Resources have been allocated.\n");

            else

            {

                printf("It is unsafe!P(%d) has to wait.\n",P);

                for (j=0; j<R; j++) {

                    available[j]+=req[j];

                    allocation[P][j]-=req[j];

                    need[P][j]+=req[j];

                }

                flag=-1;

            }



        }

    }

    return flag;

}
int release(int P,int rel[])
{
    int j,flag;

    flag=0;

    for (j=0; j<R; j++) {

        if(allocation[P][j]<rel[j])

        {

            printf("Error!P(%d)don't have that many resources to release.\n",P);

            flag = -1;

            break;

        }

    }

    for (j=0;j<R;j++) {

        if (flag==0) {

            available[j]+=rel[j];

            allocation[P][j]-=rel[j];

            need[P][j]+=rel[j];

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
//checks if the process own enough resources to release
/*int releasecorrect(int processid,int rel[])
{
	for (i = 0; i < R; ++i)
	{
		if (rel[i] <= allocation[processid][i])
		{
			continue;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

//check if requested resource is greater than need
int greater(int processid,int req[])
{
	for (i = 0; i < R; ++i)
	{
		if (req[i] <= need[processid][i])
		{
			continue;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}


//check if available resource is enough to satisfy request
int correctalloc(int req[])
{
	//first element of requestVector is processID
	for (i = 0; i < R; ++i)
	{
		if (req[i] <= available[i])
		{
			continue;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

//checks if current state is a safe state
int safe()
{   

	int ifFinish[P];

	//temporary available resources
	int work[R];

	for(i = 0; i <R; ++i)
	{
		work[i] = available[i];
		//printf("%d ",available[i]);
	}
	int k;
	for(i = 0; i <P; i++)
	{
		if (ifFinish[i] == 0)
		{
			for(j = 0; j <R; j++)
			{
				if(need[i][j] <= work[j])
				{
					if(j == R - 1)//means the whole vector is checked
					{
						ifFinish[i] = 1;
						for (k = 0; k <R; ++k)
						{
							work[k] += allocation[i][k];
							//execute and release resources
						}
						//if we break here, it will not check all processes, so we should reset i to let it check from beginning
						//If we cannot find any runnable process from beginning to the end in i loop, we can determine that
						//there is no any runnable process, but we cannot know if we do not reset i.
						i = -1; //at the end of this loop, i++, so -1++ = 0
						break; //in loop j, break to loop i and check next runnable process
					}
					else //not finished checking all resource, but this kind of resources is enough
					{
						continue;
					}
				}
				else //resources not enough, break to loop i for next process
				{
					//because no change happened, so we do not need to reset i in this condition.
					break;
				}
			}
		}
		else
		{
			continue;
		}
	}
	//there are two conditions if we finish loop i
	//1. there is no process that can run in this condition.
	//2. all processes are running, which means it is in safe status.
	for(i = 0; i <P; i++)
	{
		if (ifFinish[i] == 0)
		{
			//not all processes are running, so it is condition 1.
			//printf("problem\n");
			return -1;
		}
		else
		{
			continue;
		}
	}
	//finished loop, so it is condition 2
	return 0;
}*/
int safetyCheck()

{


	// get safe sequence

        int tempRes[R];

        for(i=0; i<R; i++) tempRes[i] = available[i];



        bool finished[P];

        for(i=0; i<P; i++) finished[i] = false;

        int nfinished=0;

        while(nfinished < P) {

                bool safe = false;



                for(i=0; i<P; i++) {

                        if(!finished[i]) {

                                bool possible = true;



                                for(j=0; j<R; j++)

                                        if(need[i][j] > tempRes[j]) {

                                                possible = false;

                                                break;

                                        }



                                if(possible) {

                                        for(j=0; j<R; j++)

                                                tempRes[j] += allocation[i][j];

                                        safeSeq[nfinished] = i;

                                        finished[i] = true;

                                        ++nfinished;

                                        safe = true;

                                }

                        }

                }



                if(!safe) {

                        for(k=0; k<P; k++) safeSeq[k] = -1;

                        return false; // no safe sequence found

                }

        }

        return true; // safe sequence found


}

