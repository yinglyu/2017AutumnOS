
#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <semaphore.h>  
#include <unistd.h>  
#include <dispatch/dispatch.h>

#include <sched.h>
#define	BUFFER_SIZE 5
typedef struct __Buf {  
    int *buf;       // 缓冲区  
    int n;          // 最大缓冲区  
    int head;       // 缓冲区头  
    int tail;       // 缓冲区尾  
    dispatch_semaphore_t full;    // 锁定没产品的消费者  
    dispatch_semaphore_t empty;    // 锁定没空的生产者  
    dispatch_semaphore_t mutex;    // 确保同时只能有一个消费者或生产者对缓存区进行操作  
} Buf;  
  
// 声明缓冲区  
Buf g_buf;  
// 声明生存时间
int lifeTime;
  
// 生产者插入项目  
int insert_item(Buf *b, int item, pthread_t id) {  

    dispatch_semaphore_wait(b->empty, DISPATCH_TIME_FOREVER); 

    dispatch_semaphore_wait(b->mutex, DISPATCH_TIME_FOREVER); 
    // 插入数据  
    b->buf[(++b->tail)%(b->n)] = item; 
    printf("producer[%d] insert buffer[%d] = %d\n", (int)id,(b->tail)%(b->n),item); 
	dispatch_semaphore_signal(b->mutex);  
    // signal等价於把信号量加一，令消费者能对缓冲区操作(说明缓冲区状态为非空)  
    dispatch_semaphore_signal(b->full);  
    return 0;
}  
  
// 生产者线程  
void *producer() {  
   // int i=0;  
    //usleep(1);
    //pthread_t id = pthread_self();
    while(1) {  
    	int rand = random()/10000;
    	
        insert_item(&g_buf, rand, pthread_self());
       // printf("producer: %d\n",i);    
    }  
    return 0;  
}  
  
// 消费者获取项目  
int remove_item(Buf *b,pthread_t id) {  
    int item;  
    dispatch_semaphore_wait(b->full, DISPATCH_TIME_FOREVER);
    dispatch_semaphore_wait(b->mutex, DISPATCH_TIME_FOREVER);
    // 获取数据(意义上是删除了缓冲区中一个项目)  
    item = b->buf[(++b->head)%(b->n)];  
    printf("\t\t\t\t\tconsumer[%d] remove buffer[%d] = %d\n",(int)id,(b->head)%(b->n), item);  

	dispatch_semaphore_signal(b->mutex);  
    // post等价于把信号量加一，令生产者能对缓冲区操作(说明缓冲区状态为未满)  
	dispatch_semaphore_signal(b->empty);  
    return 0;  
}  
  
// 消费者线程  
void *consumer() {  
    int  data;  
   // usleep(1);
	while (1) {  
        data = remove_item(&g_buf,pthread_self());  
       // printf("consumer: %d\n", data);  
        // 错开生产者与消费者的节奏，查看生产者在缓冲区填满时的操作
      	//usleep(10);  
    }  
    return 0;  
}  
  
int main(int argc,char **argv){  
  
	int proNum,conNum,i;
    // 设置缓冲区  
    g_buf.n = BUFFER_SIZE;  
    g_buf.buf = (int *)malloc(g_buf.n*sizeof(int));  
	//sem_init、sem_open方法无效后改用dispatch_semaphore_create
	g_buf.mutex=dispatch_semaphore_create(1);
	g_buf.empty=dispatch_semaphore_create(g_buf.n);
	g_buf.full=dispatch_semaphore_create(0);  
	printf("How long to sleep before terminating(ms):");
	scanf("%d",&lifeTime);
	printf("The number of producer threads:");
	scanf("%d",&proNum);
	printf("The number of consumer threads:");
	scanf("%d",&conNum);

    pthread_t *tid_pro= (pthread_t *)malloc(proNum*sizeof(pthread_t));  
    pthread_t *tid_con= (pthread_t *)malloc(conNum*sizeof(pthread_t));     
    
    g_buf.head = -1;  
    g_buf.tail = -1;  
  
    // 创建生产者与消费者线程  
    for( i=0; i<conNum; i++)
    	pthread_create(tid_con+i, 0, consumer, 0);
    for( i=0; i<proNum; i++)  
   		pthread_create(tid_pro+i, 0, producer, 0);  

	usleep(lifeTime);   
    // 释放已申请的资源 
    exit(0);
    for( i=0; i<proNum; i++)  
    	pthread_join(*(tid_pro+i), 0);  
    for( i=0; i<conNum; i++) 
        pthread_join(*(tid_con+i), 0);  
    dispatch_release(g_buf.full);  
    dispatch_release(g_buf.empty);  
    dispatch_release(g_buf.mutex);  


}  
