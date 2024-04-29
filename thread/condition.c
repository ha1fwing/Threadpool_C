#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

struct Node
{
    int val;
    struct Node *next;
};

struct Node * head = NULL;


void * producer(void * arg)
{
    while(1){
        pthread_mutex_lock(&mutex);
        struct Node * newNode = (struct Node*)malloc(sizeof(struct Node));

        newNode->val = rand()%1000;
        newNode->next = head;
        head = newNode;
        printf("生产者id = %ld, number = %d\n",pthread_self(),newNode->val);

        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond);
        sleep(1);
        
    }

    return NULL;
}

void * consumer(void * arg)
{
    while(1){
        pthread_mutex_lock(&mutex);
        while(head == NULL){
            // 如果在这里被阻塞,会自动释放互斥锁
            // 很多线程在这阻塞的话,也都会被阻塞
            pthread_cond_wait(&cond,&mutex);
        }
        struct Node* tempnode = head;
        printf("消费者id = %ld, number = %d\n",pthread_self(),tempnode->val);
        head = tempnode->next;
        free(tempnode);

        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);

    pthread_t p[5],c[5];

    // 创建线程
    for(int i = 0; i<5 ; i++){
        pthread_create(&p[i],NULL,producer,NULL);
    }
    for(int i = 0; i<5 ; i++){
        pthread_create(&c[i],NULL,consumer,NULL);
    }

    // 销毁线程
    for(int i = 0; i<5 ; i++){
        pthread_join(p[i],NULL);
    }
    for(int i = 0; i<5 ; i++){
        pthread_join(c[i],NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}