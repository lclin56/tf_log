#include "tf_log.h"
#include <pthread.h>
#include <stdlib.h>

LOG_INIT(); // Initialize the logger

int log_callback(int level, const char *log_str)
{
    fprintf(stdout, "Callback received (level %d): %s", level, log_str);
    return 0;
}

void* log_thread(void* arg)
{
    int thread_num = *((int*) arg);
    for (int i = 0; i < 10; i++)
    {
        LOG_INFO("Thread %d: This is an INFO message: %d.", thread_num, i);
        LOG_DEBUG("Thread %d: This is a DEBUG message: %d.", thread_num, i);
        LOG_WARN("Thread %d: This is a WARN message: %d.", thread_num, i);
        LOG_ERROR("Thread %d: This is an ERROR message: %d.", thread_num, i);
    }
    return NULL;
}

#define NUM_THREADS 4

int main()
{
    // Configure the logger settings
    LOG_SET_LEVEL(TF_LOG_LEVEL_WARN);
    LOG_SET_MODE(TF_LOG_MODE_FILE);
    LOG_SET_MAX_SIZE(1 * 1024 * 1024);

    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    // Launch multiple threads to log messages
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, log_thread, (void*) &thread_args[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
