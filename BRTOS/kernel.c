#include <msp430.h>
#include <stdint.h>
#include "tasks.h"


#define STACK_START 0x2800
#define STACK_SIZE  802800
#define MAX_TASKS 5


void registerTask(void *pTask);
void startBRTOS(void);

typedef struct{
    void (*pTask)();
    uint16_t *pStackNow;
    volatile uint32_t wait;
}task_t;

typedef struct{
    task_t *tasks[MAX_TASKS];
    uint16_t head;
    uint16_t tail;
    uint16_t size;
}queue_t;

task_t tasks[MAX_TASKS];
queue_t q;
uint8_t nRegTasks = 0;
uint8_t taskNumber = 0;
void *schedSP = (void*)0x2000;


void registerTask(void *pTask)
{
    tasks[nRegTasks].wait = 0;
    tasks[nRegTasks].pTask = pTask;
    tasks[nRegTasks].pStackNow= (uint16_t *) STACK_START + STACK_SIZE * nRegTasks;

    //Inicializar a pilha
    tasks[nRegTasks].pStackNow--;
    *tasks[nRegTasks].pStackNow= pTask; //PC
    tasks[nRegTasks].pStackNow --;
    *tasks[nRegTasks].pStackNow = GIE | ((uint32_t)pTask>>4 & 0xF000); //SR
    tasks[nRegTasks].pStackNow -= 24; //Contexto

    nRegTasks++;
}

void wait(uint32_t wtime){
    tasks[taskNumber].wait = wtime;
    while(tasks[taskNumber].wait);
}

void idle(void){
    while(1);
}

void put(queue_t *queue, task_t *pTask){
    queue->tasks[queue->tail] = pTask;
    queue->size = (queue->size + 1) % MAX_TASKS; //zerando
    queue->tail = (queue->tail + 1) % MAX_TASKS;
}

void get(queue_t *queue, task_t *pTask){
    queue->size = (queue->size - 1) % MAX_TASKS;
    queue->head = (queue->head + 1) % MAX_TASKS;
}

void startBRTOS(){
    registerTask(idle);

    SFRIE1 = WDTIE;
    WDTCTL = WDTPW | WDTTMSEL_1 | WDTCNTCL | WDTIS_4 ;

    tasks[0].pStackNow += 26;

    //alterna p/ a pilha da 1a tarefa
    asm("movx.a %0, SP" :: "m"(tasks[0].pStackNow));
    //coloca addr da 1a tarefa na pilha
    asm("pushx.a %0" :: "m"(tasks[0].pTask));

    _enable_interrupts();

    return;
}

void scheduler(){
    for(int i=0; i<nRegTasks; i++){
        if(tasks[i].wait != 0){
            tasks[i].wait--;
        }
    }

    do{
        taskNumber = (taskNumber + 1) % nRegTasks;
    }while(tasks[taskNumber].wait != 0);
}


__attribute__ ((naked))
__attribute__ ((interrupt(WDT_VECTOR)))
void WDT_ISR(void)
{
    //Save Context
    asm("pushm.a #12, R15");
    //Save Tasks Stack Pointer
    asm("movx.a SP,%0 ": "=m"(tasks[taskNumber].pStackNow));
    //Restore Scheduler SP
    asm("movx.a %0,SP ":: "m"(schedSP));

    scheduler();

    //Save Scheduler Stack Pointer
    asm("movx.a SP,%0 ": "=m"(schedSP));
    //Restore Stack Pointer
    asm("movx.a %0, SP"::"m"(tasks[taskNumber].pStackNow));
    //Restore Context
    asm("popm.a #12, R15");
    asm("RETI");

}
