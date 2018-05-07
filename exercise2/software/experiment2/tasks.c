// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include "define.h"

// For the performance counter
void *performance_name = PERFORMANCE_COUNTER_0_BASE;

// Definition of semaphore for PBs
OS_EVENT *PBSemaphore[4];

// Definition of task stacks
OS_STK	  	initialize_task_stk[TASK_STACKSIZE];
OS_STK	  	custom_scheduler_stk[TASK_STACKSIZE];
OS_TCB	  	custom_scheduler_tcb;

OS_STK	  	periodic_task_stk[NUM_TASK][TASK_STACKSIZE];
OS_TCB	  	periodic_task_tcb[NUM_TASK];

// Task stack for Push Button Priorities
OS_STK  	pb_prior_stk[10];

// Array for the queue
int			q_array[10];
int       	q_index = 0;

// Struct for storing information about each custom task
typedef struct task_info_struct {
	int priority;
	int execution_time;
	int os_delay;
} task_info_struct;

// Struct for storing information about tasks during dynamic scheduling using the custom_scheduler
typedef struct scheduler_info_struct {
	int valid;
	int id;
	int period;
} scheduler_info_struct;

// Local function prototypes
void custom_delay(int);
void custom_task_priority(int, int, scheduler_info_struct [], task_info_struct []);
void custom_task_create(int, int, scheduler_info_struct [], task_info_struct []);

// Periodic task 0
// It periodically uses a custom delay to occupy the CPU
// Then it suspends itself for a specified period of time
void periodic_task0(void* pdata) {
	task_info_struct *task_info_ptr;

	task_info_ptr = (task_info_struct *)pdata;
	while (1) {
		printf("Start periodic_task0 (%4d ms) (%1d s) (%d priority)\n", task_info_ptr->execution_time, task_info_ptr->os_delay, task_info_ptr->priority);
		custom_delay(task_info_ptr->execution_time);
		//printf("End	  periodic_task0 (%4d ms) (%1d s) (%d priority)\n", task_info_ptr->execution_time, task_info_ptr->os_delay, task_info_ptr->priority);
		OSTimeDlyHMSM(0, 0, task_info_ptr->os_delay/1000, task_info_ptr->os_delay%1000);
	}
}

// Periodic task 1
// It periodically uses a custom delay to occupy the CPU
// Then it suspends itself for a specified period of time
void periodic_task1(void* pdata) {
	task_info_struct *task_info_ptr;

	task_info_ptr = (task_info_struct *)pdata;
	while (1) {
		printf("Start periodic_task1 (%4d ms) (%1d s) (%d priority)\n", task_info_ptr->execution_time, task_info_ptr->os_delay, task_info_ptr->priority);
		custom_delay(task_info_ptr->execution_time);
		//printf("End	  periodic_task1 (%4d ms) (%1d s) (%d priority)\n", task_info_ptr->execution_time, task_info_ptr->os_delay, task_info_ptr->priority);
		OSTimeDlyHMSM(0, 0, task_info_ptr->os_delay/1000, task_info_ptr->os_delay%1000);
	}
}

// Periodic task 2
// It periodically uses a custom delay to occupy the CPU
// Then it suspends itself for a specified period of time
void periodic_task2(void* pdata) {
	task_info_struct *task_info_ptr;

	task_info_ptr = (task_info_struct *)pdata;
	while (1) {
		printf("Start periodic_task2 (%4d ms) (%1d s) (%d priority)\n", task_info_ptr->execution_time, task_info_ptr->os_delay, task_info_ptr->priority);
		custom_delay(task_info_ptr->execution_time);
		//printf("End	  periodic_task2 (%4d ms) (%1d s) (%d priority)\n", task_info_ptr->execution_time, task_info_ptr->os_delay, task_info_ptr->priority);
		OSTimeDlyHMSM(0, 0, task_info_ptr->os_delay/1000, task_info_ptr->os_delay%1000);
	}
}

// Periodic task 3
// It periodically uses a custom delay to occupy the CPU
// Then it suspends itself for a specified period of time
void periodic_task3(void* pdata) {
	task_info_struct *task_info_ptr;

	task_info_ptr = (task_info_struct *)pdata;
	while (1) {
		printf("Start periodic_task3 (%4d ms) (%1d s) (%d priority)\n", task_info_ptr->execution_time, task_info_ptr->os_delay, task_info_ptr->priority);
		custom_delay(task_info_ptr->execution_time);
		//printf("End	  periodic_task3 (%4d ms) (%1d s) (%d priority)\n", task_info_ptr->execution_time, task_info_ptr->os_delay, task_info_ptr->priority);
		OSTimeDlyHMSM(0, 0, task_info_ptr->os_delay/1000, task_info_ptr->os_delay%1000);
	}
}

// The custom_scheduler
// It has the highest priority
// It checks the PBs every 500ms
// It a button has been pressed, it creates/deletes the corresponding task in the OS
// When creating a task, it will assign the new task with the lowest priority among the running tasks
void custom_scheduler(void *pdata) {
	INT8U return_code = OS_NO_ERR;
	int i;
	int PB_pressed[NUM_PB_BUTTON];
	int sem_value;
	int new_pressed;
	//int period;
	
	int num_active_task;	
	// Array of task_info
	task_info_struct task_info[NUM_TASK];
	scheduler_info_struct scheduler_info[NUM_TASK];

	printf("Starting custom scheduler...\n");

	num_active_task = 0;

	for (i = 0; i < NUM_TASK; i++) {
		// Creating periodic task 0 with random execution time and delay time for periodic arrival
		//task_info[i].execution_time = (rand() % (EXECUTION_TIME_LIMIT/100) + 1) * 100;
		//task_info[i].os_delay = rand() % OS_DELAY_LIMIT + 1;
		//task_info[i].priority = num_active_task;
		task_info[i].execution_time = (rand() % EXECUTION_TIME_LIMIT) + 1000;
		task_info[i].os_delay = 10000 + (rand() % OS_DELAY_LIMIT) - task_info[i].execution_time;
		//period = (rand() % 2000)
		task_info[i].priority = -1;

		//scheduler_info[num_active_task].valid = 1;
		scheduler_info[num_active_task].valid = 0;
		scheduler_info[num_active_task].id = i;
		scheduler_info[num_active_task].period = 10000 + (rand() % OS_DELAY_LIMIT);
		//scheduler_info[num_active_task].period = task_info[i].execution_time + task_info[i].os_delay * 1000;
	}

	// Scheduler never returns
	while (1) {
		if (q_index > 0) {
			printf("Locking OS scheduler for new scheduling\n");
			OSSchedLock();			
			
			for (i = 0; i < q_index; i++) {
                if (scheduler_info[q_array[i]].valid == 0){
                    custom_task_create(q_array[i], num_active_task, scheduler_info, task_info);
                    num_active_task++;
                }else{
                    printf("Assigning Task%d the lowest priority\n", q_array[i]);
                    sem_value = task_info[q_array[i]].priority;
                    if (sem_value < num_active_task){
                        custom_task_priority(sem_value, num_active_task, scheduler_info, task_info);
					}
                }
			}
			
			printf("There are %d active task(s).\n", num_active_task);
			printf("Printing task info:\n");
			for (i = 0; i < num_active_task; i++) {
				printf("Priority %d: valid=%d, task_id=%d, period=%d, exec_time=%d, os_delay=%d, pri=%d\n",
					i,
					scheduler_info[i].valid,
					scheduler_info[i].id,
					scheduler_info[i].period,
					task_info[scheduler_info[i].id].execution_time,
					task_info[scheduler_info[i].id].os_delay,
					task_info[scheduler_info[i].id].priority);
			}
			
			q_index = 0;
			printf("Unlocking OS scheduler\n");
			OSSchedUnlock();
		}
		OSTimeDlyHMSM(0, 0, 3, 0);
	}
}

void custom_task_priority(int sem_value, int num_active_task, scheduler_info_struct scheduler_info[], task_info_struct task_info[]) {
    int i, temp;
    int priorities[num_active_task];
    INT8U return_code = OS_NO_ERR;
	
    for(i=0;i<4;i++){
        if (scheduler_info[i].valid == 1){
            priorities[task_info[i].priority-1]=i;
		}
    }
    
    return_code = OSTaskChangePrio(TASK_START_PRIORITY+sem_value, TASK_START_PRIORITY+num_active_task+1);
    alt_ucosii_check_return_code(return_code);
	
    for(i=sem_value;i<num_active_task;i++){
        temp = task_info[priorities[i]].priority;
        return_code = OSTaskChangePrio(TASK_START_PRIORITY+temp, TASK_START_PRIORITY+temp-1);
        alt_ucosii_check_return_code(return_code);
        task_info[priorities[i]].priority--;
    }

    return_code = OSTaskChangePrio(TASK_START_PRIORITY+num_active_task+1, TASK_START_PRIORITY+num_active_task);
    alt_ucosii_check_return_code(return_code);
    task_info[priorities[sem_value-1]].priority=num_active_task;
}

// Function for creating a task in the OS, and update the data structure task_info
// The new task has the lowest priority among the existing tasks
void custom_task_create(int tid, int num_active_task, scheduler_info_struct scheduler_info[], task_info_struct task_info[]) {
	int i, temp;
	int priorities[num_active_task];
	INT8U return_code = OS_NO_ERR;
	
    for(i=0; i<4; i++){
        if (scheduler_info[i].valid == 1){
            priorities[task_info[i].priority-1] = i;
		}
    }

    for(i=num_active_task-1; i>=0; i--){
        temp = task_info[priorities[i]].priority;
        return_code = OSTaskChangePrio(TASK_START_PRIORITY+temp, TASK_START_PRIORITY+temp+1);
        alt_ucosii_check_return_code(return_code);
        task_info[priorities[i]].priority++;
    }
	
	printf("Creating task...\n");
	
    task_info[tid].priority = 1;
    scheduler_info[tid].valid = 1;
	
	switch(tid) {
		case 0:
			return_code = OSTaskCreateExt(periodic_task0,
									 &task_info[tid],
									 (void *)&periodic_task_stk[tid][TASK_STACKSIZE-1],
									 TASK_START_PRIORITY+1,
									 tid,
									 &periodic_task_stk[tid][0],
									 TASK_STACKSIZE,
									 &periodic_task_tcb[tid],
									 0);
			alt_ucosii_check_return_code(return_code);
		break;
		case 1:
			return_code = OSTaskCreateExt(periodic_task1,
									&task_info[tid],
									 (void *)&periodic_task_stk[tid][TASK_STACKSIZE-1],
									 TASK_START_PRIORITY+1,
									 tid,
									 &periodic_task_stk[tid][0],
									 TASK_STACKSIZE,
									 &periodic_task_tcb[tid],
									 0);
			alt_ucosii_check_return_code(return_code);
		break;
		case 2:
			return_code = OSTaskCreateExt(periodic_task2,
									&task_info[tid],
									 (void *)&periodic_task_stk[tid][TASK_STACKSIZE-1],
									 TASK_START_PRIORITY+1,
									 tid,
									 &periodic_task_stk[tid][0],
									 TASK_STACKSIZE,
									 &periodic_task_tcb[tid],
									 0);
			alt_ucosii_check_return_code(return_code);
		break;
		default:
			return_code = OSTaskCreateExt(periodic_task3,
									&task_info[tid],
									 (void *)&periodic_task_stk[tid][TASK_STACKSIZE-1],
									 TASK_START_PRIORITY+1,
									 tid,
									 &periodic_task_stk[tid][0],
									 TASK_STACKSIZE,
									 &periodic_task_tcb[tid],
									 0);
			alt_ucosii_check_return_code(return_code);
		break;
	}

	
	printf("%d task created...\n", tid);
	
	//num_task_created++;
	//printf("%d task created...\n", num_task_created);
	//return num_task_created;
}

// Function for occupying the processor for the specified number of clock ticks
// to simulate custom delay while keeping the task in the processor
void custom_delay(int ticks) {
    INT32U cur_tick;
    ticks--;
    cur_tick = OSTimeGet();
    while (ticks > 0) {
         if (OSTimeGet() > cur_tick) {
            ticks--;
            cur_tick = OSTimeGet();
         }  
    }
}
