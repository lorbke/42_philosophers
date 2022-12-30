#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>

//------------------------------------------------------

// time calculation
int	main(int argc, char **argv)
{
	struct timeval time;
	time_t ts;
	time_t temp;
	
	gettimeofday(&time, NULL); // NULL is the timezone
	ts = time.tv_usec;
	usleep(1000);
	gettimeofday(&time, NULL);
	temp = time.tv_usec;
	temp = temp - ts;
	printf("%ld\n", temp); 
	return 0;
}

//------------------------------------------------------

// passing number of thread to the thread_func
void *thread_func(void *arg)
{
    int thread_num = *(int *)arg; // cast the argument to an int pointer and dereference it

    printf("Hello from thread %d\n", thread_num);

    return NULL;
}

int main()
{
    pthread_t thread1, thread2;
    int thread_num1 = 1, thread_num2 = 2;

    // create the threads and pass the thread number as an argument
    pthread_create(&thread1, NULL, thread_func, &thread_num1);
    pthread_create(&thread2, NULL, thread_func, &thread_num2);

    // wait for the threads to finish
    pthread_join(thread1, NULL); // will block calling thread until thread1 finishes
    pthread_join(thread2, NULL);

    return 0;
}

//------------------------------------------------------
