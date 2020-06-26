#include <msp430.h>
#include <stdint.h>
#include "tasks.h"


#define STACK_START 0x2800
#define STACK_SIZE  80
#define MAX_TASKS 5
#define PRIORITY_HIGH 2
#define PRIORITY_LOW 0


typedef struct{
    void (*pTask)();
    uint16_t *pStackNow;
    volatile uint32_t wait;
    uint8_t priority;
    uint8_t quantum;
    uint8_t finished;
}task_t;

typedef struct{
    task_t *tasks[MAX_TASKS];
    uint16_t head, tail, size;
}queue_t;


void registerTask(void *pTask, uint8_t priority, uint8_t quantum);
void startBRTOS(void);
void put(queue_t *queue, task_t *pTask);
task_t get(queue_t *queue);


task_t task[MAX_TASKS];
task_t runningTask;
queue_t tasks[3];
uint8_t nRegTasks = 0;
uint8_t taskNumber = 0;
void *schedSP = (void*)0x2000;


void registerTask(void *pTask, uint8_t priority, uint8_t quantum)
{
    task[nRegTasks].priority = priority;
    task[nRegTasks].quantum = quantum;
    task[nRegTasks].wait = 0;
    task[nRegTasks].pTask = pTask;
    task[nRegTasks].pStackNow= (uint16_t *) STACK_START + STACK_SIZE * nRegTasks;

    //Inicializar a pilha
    task[nRegTasks].pStackNow--;
    *task[nRegTasks].pStackNow= pTask; //PC
    task[nRegTasks].pStackNow --;
    *task[nRegTasks].pStackNow = GIE | ((uint32_t)pTask>>4 & 0xF000); //SR
    task[nRegTasks].pStackNow -= 24; //Contexto

    put(&tasks[priority], &task[nRegTasks]);
    nRegTasks++;
}

void wait(uint32_t wtime){
/*    tasks[taskNumber].wait = wtime;
    while(tasks[taskNumber].wait);*/
}

void idle(void){
    while(1);
}

void put(queue_t *queue, task_t *pTask){
    queue->tasks[queue->tail] = pTask;
    queue->size = (queue->size + 1) % MAX_TASKS; //zerando
    queue->tail = (queue->tail + 1) % MAX_TASKS;
}

task_t get(queue_t *queue){
    queue->size = (queue->size - 1) % MAX_TASKS;
    queue->head = (queue->head + 1) % MAX_TASKS;
    return *queue->tasks[queue->head-1];
}

void startBRTOS(){
    registerTask(idle, PRIORITY_LOW, 1);

    SFRIE1 = WDTIE;
    WDTCTL = WDTPW | WDTTMSEL_1 | WDTCNTCL | WDTIS_6 | WDTSSEL_1  ;

    runningTask = get(&tasks[PRIORITY_HIGH]);
    runningTask.finished = runningTask.quantum;

    runningTask.pStackNow += 26;

    //alterna p/ a pilha da 1a tarefa
    asm("movx.a %0, SP" :: "m"(runningTask.pStackNow));
    //coloca addr da 1a tarefa na pilha
    asm("pushx.a %0" :: "m"(runningTask.pTask));


    _enable_interrupts();

    asm("reta");
}

void scheduler(){
/*
    for(int i=0; i<nRegTasks; i++){
        if(tasks[i].wait != 0){
            tasks[i].wait--;
        }
    }

    do{
        taskNumber = (taskNumber + 1) % nRegTasks;
    }while(tasks[taskNumber].wait != 0);
*/

    //PRIORIDADES

    if(runningTask.finished--){
        return;
    }
    put(&tasks[runningTask.priority], &runningTask);

    for(uint8_t p=PRIORITY_HIGH; p>=PRIORITY_LOW; p--){
        if(tasks[p].size != 0){
            runningTask = get(&tasks[p]);
            runningTask.finished = runningTask.quantum;
            return;
        }
    }
}


__attribute__ ((naked))
__attribute__ ((interrupt(WDT_VECTOR)))
void WDT_ISR(void)
{
    //Save Context
    asm("pushm.a #12, R15");
    //Save Tasks Stack Pointer
    asm("movx.a SP,%0 ": "=m"(runningTask.pStackNow));
    //Restore Scheduler SP
    asm("movx.a %0,SP ":: "m"(schedSP));

    scheduler();

    //Save Scheduler Stack Pointer
    asm("movx.a SP,%0 ": "=m"(schedSP));
    //Restore Stack Pointer
    asm("movx.a %0, SP"::"m"(runningTask.pStackNow));
    //Restore Context
    asm("popm.a #12, R15");
    asm("RETI");

}
