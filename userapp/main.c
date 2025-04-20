/*
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <inttypes.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>
#include <syscall.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "graphics.h"
#include <string.h>
#include"controlModule.c"
#include"ssort.c"


extern void *tlsf_create_with_pool(void* mem, size_t bytes);
extern void *g_heap;

/**
 * GCC insists on __main
 *    http://gcc.gnu.org/onlinedocs/gccint/Collect2.html
 */
void __main()
{
    size_t heap_size = 32*1024*1024;
    void  *heap_base = mmap(NULL, heap_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	g_heap = tlsf_create_with_pool(heap_base, heap_size);
}
/*
结构体定义
*/
struct passArg
{
    int* array;
    int size;
    int lEdge;
};


/**
 * 新线程所执行的函数
 */
// void task_foo1(void *pv)
// {
//     // int i ;
//     // for (i = 0; i < 2000; i++)
//     // {
//     //     printf("priority: 1:%d\n2:%d\n3:%d\n",getpriority(1),getpriority(2),getpriority(3));
//     //     printf("O");
//     //     sleep(1);
//     // }
    
//     // printf("Task1 has done!\n");
//     // task_exit(0);
//     // while(1)
//     //     ;
//     struct passArg* args = (struct passArg*)pv; 
//     insertionSort(args->array,args->size,args->lEdge);    //调用排序函数
    
//     task_exit(0);
//     while(1)
//     ;
// }

// void task_foo2(void *pv)
// {
//     //画出线程优先级进度条
//     int temp1 = 0;int temp2 = 0;
//     while(1)
//     {
//     int rightPrio = getpriority(2);
//     if(temp1 != rightPrio)
//         drawBar(15, rightPrio*20, 410, 580, RGB(0,0,255));
//     temp1 = rightPrio;

    
//     int leftPrio = getpriority(3);
//     if (temp2 != leftPrio)
//         drawBar(15, leftPrio*20, 0, 580, RGB(0,0,255));
//     temp2 = leftPrio;
// }
//     task_exit(0);
//     while(1)
//         ;
// }

void producer(void *p){
	int i;
	for(i=0;;i++,i %= blockSize){
		sem_wait(empty);

            // line(0, 0, 640, 800, RGB(0, 255, 255)); //清屏
		if (arrs[i]!=NULL){
			free(arrs[i]);
		}

		srand(time(NULL));
		int j;
		int * arr;
		arr = malloc((arrSize)*sizeof(int));
        //清屏函数
        clear(130*(i%blockSize),0,130*(i%blockSize)+130,570);
        nanosleep_delay(); //延时一会，让用户能看出高亮效果
		for(j=0;j<arrSize;j++){
			arr[j] = rand()%(100-10);
            drawBar(5, arr[j], 130*(i%blockSize), j * 5, RGB(255,0,0));
            nanosleep_delay(); 
            
		}
		arrs[i] = arr;
        // drawArr(arrs[i],arrSize,130*(i%blockSize),RGB(255,0,0));    //画出初始数组,作为底板
        sleep(1);
        sem_signal(full);

	}
    
}

void consumer(void *p){
	int i;
	for (i =0 ;;i++){
		sem_wait(full);
        // // line(0, 0, 640, 480, RGB(255, 255, 255)); //清屏
		bubbleSort(arrs[i%blockSize],arrSize,130*(i%blockSize));    //调用排序函数
		sem_signal(empty);
        // line(0, 0, 640, 480, RGB(255, 255, 255)); //清屏
	}

}


/**
 * 第一个运行在用户模式的线程所执行的函数
 */

void main(void *pv)
{
    printf("task #%d: I'm the first user task(pv=0x%08x)!\r\n",
            task_getid(), pv);

    //TODO: Your code goes here
    init_graphic(0x115);
    // list_graphic_modes();
    full = sem_create(0); //创建信号量full，初始值为0
    empty = sem_create(blockSize);
    unsigned char*producer_stack;
    unsigned char*consumer_stack;
    unsigned int stack_size = 1024*1024;

    producer_stack = (unsigned char*)malloc(stack_size);
    consumer_stack = (unsigned char*)malloc(stack_size);

    pid = task_create(producer_stack+stack_size, &producer, NULL); //创建线程1
    cid = task_create(consumer_stack+stack_size, &consumer, NULL); //创建线程2

    // tid_foo2 = task_create(stack_foo2+stack_size, &task_foo2, NULL);
    control(pid,cid);
    

    exit_graphic(); //退出图形界面
    task_exit(0);

}

