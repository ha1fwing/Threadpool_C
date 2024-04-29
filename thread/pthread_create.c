#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

struct test
{
    int num;
    int age;    
};





// 回调函数的返回值是 void*
void* callback(void* arg)
{
    for(int i = 0; i < 5; i++)
    {
        printf("%d\n",i);
    }
    printf("子线程id = %ld\n",pthread_self());

    // 为什么要用static 因为子线程释放之后,该子线程的栈区还回去了,无法获取原来的数据
    // 必须使用全局变量保证数据不先被释放
    //static struct test t;

    struct test * t = (struct test*)arg;
    t->age = 40;
    t->num = 100;

    
    pthread_exit(t);
}

int main()
{
    pthread_t p;
    // 子线程地址,null,回调函数(就是子线程要执行的函数),回调函数的参数

    // 或者把t放到主线程的栈空间
    struct test t;

    pthread_create(&p,NULL,callback,&t);
    // 线程分离
    pthread_detach(p);

    printf("主线程id = %ld\n",pthread_self());

    pthread_exit(NULL);
    
    return 0;
}