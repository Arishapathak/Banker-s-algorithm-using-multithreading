#include<conio.h>
#include<stdio.h>
main()
{
	int P,R,count=0,process;
	printf("Enter the total number of processes");
	scanf("%d",&P);
	printf("Enter the number of total number of resources");
	scanf("%d",&R);
	int maximum[10][10],need[10][10],allocation[10][10],available[10],safesequence[10],completed[10];
	for(int i=0;i<P;i++)
	{
		completed[i]=0;
	}
	printf("Enter the maximum limit");
	for(int i=0;i<P;i++)
	{
		printf("For process %d:",i+1);
		for(int j=0;j<R;j++)
		{
			scanf("%d",&maximum[i][j]);
		}
	}
	printf("Enter the allocation of each process");
	for(int i=0;i<P;i++)
	{
		printf("For process %d:",i+1);
		for(int j=0;j<R;j++)
		{
			scanf("%d",&allocation[i][j]);
		}
	}
	printf("Enter the available resources");
	for(int i=0;i<R;i++)
	{
		scanf("%d",&available[i]);
	}
	//need of resources for each process
	for(int i=0;i<P;i++)
	{
		for(int j=0;j<R;j++)
		{
			need[i][j]=maximum[i][j]-allocation[i][j];
		}
	}
	do
	{
		printf("Maximum:");
		for(int i=0;i<P;i++)
		{
			for(int j=0;j<R;j++)
			{
				printf("%d ",maximum[i][j]);
			}
		}
		printf("Allocation:");
		for(int i=0;i<P;i++)
		{
			for(int j=0;j<R;j++)
			{
				printf("%d ",allocation[i][j]);
			}
		}
		process=-1;
		for(int i=0;i<P;i++)
		{
			if(completed[i]==0)
			{
				process=i;
				for(int j=0;j<R;j++)
				{
					if(available[j]<need[i][j])
					{
						process=-1;
						break;
						
					}
				}
		    }	
		}
			if(process!=-1)
			{
				printf("Process %d completed",process+1);
				safesequence[count]=process+1;
				count++;
				for(int j=0;j<R;j++)
				{
					available[j]=available[j]+allocation[process][j];
					allocation[process][j]=0;
					maximum[process][j]=0;
					completed[process]=1;
				}
			}
		
	
	}while(count!=P&&process!=-1);
	if(count==P)
	{
		printf("System is in a safe state");
		printf("Safe sequence:");
		for(int i=0;i<P;i++)
		{
			printf("%d ",safesequence[i]);
		}
	}
	else
	{
		printf("Unsafe state");
	}
	
}
