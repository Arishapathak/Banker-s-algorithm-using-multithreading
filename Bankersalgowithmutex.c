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
int safetyCheck();
int *safeq;
void *pcode(void *P);
main()
{
	//input of number of resources and processes
printf("Number of Processes:   ");
scanf("%d",&P);
	
printf("Number of Resources are:   ");
scanf("%d",&R);

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

    printf("\nNumber of resources allocated to P(%d)\n", i+1);
            printf("\n");
	for( j=0; j<R; j++)
        scanf("%d", &allocation[i][j]);

}
// maximum required resources
	for( i=0; i<P; i++) {
		printf("\nMaximum number of resources P(%d) can hold:\n ", i+1);
		        printf("\n");
		for( j=0; j<R; j++)
			scanf("%d", &maximum[i][j]);

    }
//Available resources
printf("\nResources currently available in the system:\n   ");
        printf("\n");
	for(i=0; i< R; i++)
		scanf("%d", &available[i]);
//need calculation
for ( i=0;i<P;i++) {

        for ( j=0; j<R;j++) {

            need[i][j] = maximum[i][j]-allocation[i][j];

        }

    }
    	//safetycheck and safe sequence

safeq = (int *)malloc(P * sizeof(*safeq));
        for(i=0; i<P; i++) 
		safeq[i] = -1;
         if(!safetyCheck()) {
				printf("\n!!!!The system is in unsafe state.!!!!\n\n");
				exit(-1);
				}



        printf("\n\nThe safe sequence is : ");
		for( i=0; i<P; i++) {
				printf("%-3d", safeq[i]+1);

        }
 printf("\n\n");
sleep(1);
pthread_t processes[P];

        pthread_attr_t attr;

        pthread_attr_init(&attr);
	int processnumber[P];
	for(i=0; i<P; i++) processnumber[i] = i;
        for(i=0; i<P; i++)
                pthread_create(&processes[i], &attr, pcode, (void *)(&processnumber[i]));
        for(i=0; i<P; i++)
                pthread_join(processes[i], NULL);
        printf("\nTask completed\n");	
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
		printf("Process: %d request completed",processid+1);
		
		//release resources
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
 printf("Error! Request is more than need. Request Denied.\n");
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
                printf("Safe!!Allocation done.\n");
            else
	    {
                printf("Error!!!unsafe!wait....\n",P);
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
            printf("\nResources can not be released.\n",P);
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
        printf("\nResources have been released.\n");
    }
    else
    {
        printf("Error! Resources cannot be released.\n");
    }
    return flag;
}
int safetyCheck()

{
        int temp[R];
        for(i=0; i<R; i++) temp[i] = available[i];
        bool finished[P];
        for(i=0; i<P; i++) finished[i] = false;
        int nfinished=0;
        while(nfinished < P) {
                bool safe = false;
                for(i=0; i<P; i++) {
                    if(!finished[i]) {
                        bool possible = true;
                        for(j=0; j<R; j++)
                        if(need[i][j] > temp[j]) {
                            possible = false;
                            break;
                       }
						if(possible) {
                            for(j=0; j<R; j++)
                            temp[j] += allocation[i][j];
                        safeq[nfinished] = i;
	                    finished[i] = true;
                        ++nfinished;
                        safe = true;
                       }
                   }
                }
                if(!safe) {

                        for(k=0; k<P; k++) safeq[k] = -1;
                        return false; // if no safe sequence is there
                }
        }
        return true; // safe sequence found
}

