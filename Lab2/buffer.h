/* buffer.h */
typedef int buffer_item;
#define BUFFER_SIZE 5
void initialize(pthread_attr_t *attr);
void *producer(void *param);
void *consumer(void *param);
