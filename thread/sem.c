#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>

pthread_mutex_t mutex;

sem_t semp;
sem_t semc;


struct Node
{
    int val;
    struct Node *next;
};

struct Node * head = NULL;


void * producer(void * arg)
{
    while(1){
        sem_wait(&semp);
        pthread_mutex_lock(&mutex);

        struct Node * newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->val = rand()%1000;
        newNode->next = head;
        head = newNode;
        printf("生产者id = %ld, number = %d\n",pthread_self(),newNode->val);

        pthread_mutex_unlock(&mutex);
        sem_post(&semc);
        sleep(1);
        
    }

    return NULL;
}

void * consumer(void * arg)
{
    while(1){
        sem_wait(&semc);
        pthread_mutex_lock(&mutex);

        struct Node* tempnode = head;
        printf("消费者id = %ld, number = %d\n",pthread_self(),tempnode->val);
        head = tempnode->next;
        free(tempnode);

        pthread_mutex_unlock(&mutex);
        sem_post(&semp);
        sleep(1);
    }
    return NULL;
}

int main()
{
    sem_init(&semp,0,5);
    sem_init(&semc,0,0);
    pthread_mutex_init(&mutex,NULL);

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
    sem_destroy(&semp);
    sem_destroy(&semc);
    return 0;
}