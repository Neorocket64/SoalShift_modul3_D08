#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[101]; //inisialisasi array untuk menampung thread

void* factoriul(void *arg)
{
	int c, fact = 1;
	for (c = arg; c >= 1; c--) 
	{
		fact = fact * c;
	}
	printf("%d! = %d\n", arg, fact);
	return NULL;
}

int main(int argc, char *argv[])
{
	int i = 0, input, factarr[100] = {};
	int err;
	char *ptr;
	//input masukkan segera ke array
	for(i = 0; i < argc - 1; i++)
	{
		input = strtol(argv[i + 1], &ptr, 10);
		factarr[i] = input;
		//printf("belum%d\n", factarr[i]);
	}
	
	//saatnya sorting?
	int o, p, smt;
	for(o = 0; o < i - 1; o++)
    {
        for(p = i - 1; p > o; p--)
        {
            if(factarr[p - 1] > factarr[p])
            {
                smt = factarr[p - 1];
                factarr[p - 1] = factarr[p];
                factarr[p] = smt;
            }
        }
    }
	
	for(i = 0; i < argc - 1; i++)
	{
		err = pthread_create(&(tid[i]), NULL, factoriul, factarr[i]); //membuat thread
		if(err != 0) //cek error
		{
			printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			printf("\n create thread success\n");
		}
		pthread_join(tid[i],NULL);
	}
	exit(0);
	return 0;
}
