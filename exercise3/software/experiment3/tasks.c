// Copyright by Adam Kinsman, Henry Ko and Nicola Nicolici
// Developed for the Embedded Systems course (COE4DS4)
// Department of Electrical and Computer Engineering
// McMaster University
// Ontario, Canada

#include "define.h"

// Definition of semaphore for PBs
OS_EVENT *PBSemaphore[4];

// Definition of Mutex for Switches and LEDs
OS_EVENT *SWITCH_A;
OS_EVENT *SWITCH_B;
OS_EVENT *RED_LED;
OS_EVENT *GREEN_LED;

// Definition of task stacks
OS_STK	  initialize_task_stk[TASK_STACKSIZE];
OS_STK	  task_launcher_stk[TASK_STACKSIZE];

OS_STK	  custom_task_stk[NUM_TASK][TASK_STACKSIZE];
OS_TCB	  custom_task_tcb[NUM_TASK];

extern alt_up_character_lcd_dev *lcd_0;

// Local function prototypes
void custom_delay(int);

// Custom task 0
// It has a high priority to monitor PB0
// And it display OS info when PB0 is pressed
void custom_task_0(void* pdata) {
	INT8U return_code = OS_NO_ERR;
	OS_TCB tcb_data;
	int check;

	while (1) {

		check = 0;

		printf("Pending Semaphore 0.\n");
		OSSemPend(PBSemaphore[0], 0, &return_code);
		alt_ucosii_check_return_code(return_code);


		#if MY_MUTEX_ENABLE == 1
			printf("Pending Switch A Mutex\n");
			//OSSemPend(SWITCH_A, 0, &return_code);
			OSMutexPend(SWITCH_A, 0, &return_code);
			alt_ucosii_check_return_code(return_code);

			if (return_code == OS_NO_ERR){
				printf("Pending Red LED Mutex\n");
				//OSSemPend(RED_LED, 0, &return_code);
				OSMutexPend(RED_LED, 0, &return_code);
				alt_ucosii_check_return_code(return_code);
				check = 1;
			}
		#endif

		if (check == 1){
			IOWR(LED_RED_O_BASE, 0, IORD(SWITCH_GRPA_I_BASE, 0));
			//OSTimeDly(3500); // 3.5ms delay
			custom_delay(3500);
			IOWR(LED_RED_O_BASE, 0, 0x0); // reset red LED

			printf("Printing task info:\n");
			return_code = OSTaskQuery(CUSTOM_TASK_0_PRIORITY , &tcb_data);
			if (return_code == OS_NO_ERR) {
				printf("Task 0: Priority = %d, Status = %d, # of times active = %4ld\n",
					tcb_data.OSTCBPrio,
					tcb_data.OSTCBStat,
					tcb_data.OSTCBCtxSwCtr);
			}
			printf("\n");

			#if MY_MUTEX_ENABLE == 1
				printf("Releasing Red LED Mutex.\n");
				//return_code= OSSemPost(RED_LED);
				return_code= OSMutexPost(RED_LED);
				alt_ucosii_check_return_code(return_code);

				printf("Releasing Switch A Mutex.\n");
				//return_code = OSSemPost(SWITCH_A);
				return_code = OSMutexPost(SWITCH_A);
				alt_ucosii_check_return_code(return_code);
			#endif
		}
	}
}

// Custom task 1
// It monitors PB1
// And it display a message on LCD when PB1 is pressed
void custom_task_1(void* pdata) {
	INT8U return_code = OS_NO_ERR;
	OS_TCB tcb_data;
	int check;

	while (1) {
		check = 0;

		printf("Pending Semaphore 1.\n");
		OSSemPend(PBSemaphore[1], 0, &return_code);
		alt_ucosii_check_return_code(return_code);

		#if MY_MUTEX_ENABLE == 1
			printf("Pending Switch A Mutex\n");
			//OSSemPend(SWITCH_A, 0, &return_code);
			OSMutexPend(SWITCH_A, 0, &return_code);
			alt_ucosii_check_return_code(return_code);

			if (return_code == OS_NO_ERR){
				printf("Pending Green LED Mutex\n");
				//OSSemPend(GREEN_LED, 0, &return_code);
				OSMutexPend(GREEN_LED, 0, &return_code);
				alt_ucosii_check_return_code(return_code);
				check = 1;
			}
		#endif

		if (check == 1){
			IOWR(LED_GREEN_O_BASE, 0, IORD(SWITCH_GRPA_I_BASE, 0));
			//OSTimeDly(3500); // 3.5ms delay
			custom_delay(3500);
			IOWR(LED_GREEN_O_BASE, 0, 0x0); // reset green LED

			printf("Printing task info:\n");
			return_code = OSTaskQuery(CUSTOM_TASK_1_PRIORITY , &tcb_data);
			if (return_code == OS_NO_ERR) {
				printf("Task 1: Priority = %d, Status = %d, # of times active = %4ld\n",
					tcb_data.OSTCBPrio,
					tcb_data.OSTCBStat,
					tcb_data.OSTCBCtxSwCtr);
			}
			printf("\n");

			#if MY_MUTEX_ENABLE == 1
				printf("Releasing Green LED Mutex.\n");
				//return_code= OSSemPost(GREEN_LED);
				return_code= OSMutexPost(GREEN_LED);
				alt_ucosii_check_return_code(return_code);

				printf("Releasing Switch A Mutex.\n");
				//return_code = OSSemPost(SWITCH_A);
				return_code = OSMutexPost(SWITCH_A);
				alt_ucosii_check_return_code(return_code);
			#endif
		}
	}
}

// Custom task 2
// It monitors PB2
// And it display a message on LCD when PB2 is pressed
void custom_task_2(void* pdata) {
	INT8U return_code = OS_NO_ERR;
	OS_TCB tcb_data;
	int check;

	while (1) {
		check = 0;

		printf("Pending Semaphore 2.\n");
		OSSemPend(PBSemaphore[2], 0, &return_code);
		alt_ucosii_check_return_code(return_code);

		#if MY_MUTEX_ENABLE == 1
			printf("Pending Switch B Mutex\n");
			//OSSemPend(SWITCH_B, 0, &return_code);
			OSMutexPend(SWITCH_B, 0, &return_code);
			alt_ucosii_check_return_code(return_code);
			if (return_code == OS_NO_ERR){
				printf("Pending Red LED Mutex\n");
				//OSSemPend(RED_LED, 0, &return_code);
				OSMutexPend(RED_LED, 0, &return_code);
				alt_ucosii_check_return_code(return_code);
				check = 1;
			}
		#endif

		if (check == 1){
			IOWR(LED_RED_O_BASE, 0, IORD(SWITCH_GRPB_I_BASE, 0));
			//OSTimeDly(2500); // 2.5ms delay
			custom_delay(2500);
			IOWR(LED_RED_O_BASE, 0, 0x0); // Reset red LED

			printf("Printing task info:\n");
			return_code = OSTaskQuery(CUSTOM_TASK_2_PRIORITY , &tcb_data);
			if (return_code == OS_NO_ERR) {
				printf("Task 2: Priority = %d, Status = %d, # of times active = %4ld\n",
					tcb_data.OSTCBPrio,
					tcb_data.OSTCBStat,
					tcb_data.OSTCBCtxSwCtr);
			}
			printf("\n");

			#if MY_MUTEX_ENABLE == 1
				printf("Releasing Red LED Mutex.\n");
				//return_code= OSSemPost(RED_LED);
				return_code= OSMutexPost(RED_LED);
				alt_ucosii_check_return_code(return_code);

				printf("Releasing Switch B Mutex.\n");
				//return_code = OSSemPost(SWITCH_B);
				return_code = OSMutexPost(SWITCH_B);
				alt_ucosii_check_return_code(return_code);
			#endif
		}
	}
}

// Custom task 3
// It monitors PB3
// And it erases all characters on LCD when PB3 is pressed
void custom_task_3(void* pdata) {
	INT8U return_code = OS_NO_ERR;
	OS_TCB tcb_data;
	int x, y;
	int check;

	while (1) {
		check = 0;

		printf("Pending Semaphore 3.\n");
		OSSemPend(PBSemaphore[3], 0, &return_code);
		alt_ucosii_check_return_code(return_code);

		#if MY_MUTEX_ENABLE == 1
			printf("Pending Switch B Mutex\n");
			//OSSemPend(SWITCH_B, 0, &return_code);
			OSMutexPend(SWITCH_B, 0, &return_code);
			alt_ucosii_check_return_code(return_code);

			if (return_code == OS_NO_ERR){
				printf("Pending Green LED Mutex\n");
				//OSSemPend(GREEN_LED, 0, &return_code);
				OSMutexPend(GREEN_LED, 0, &return_code);
				alt_ucosii_check_return_code(return_code);
				check = 1;
			}
		#endif

		if (check == 1){
			IOWR(LED_GREEN_O_BASE, 0, IORD(SWITCH_GRPB_I_BASE, 0));
			//OSTimeDly(2500); // 2.5ms delay
			custom_delay(2500);
			IOWR(LED_GREEN_O_BASE, 0, 0x0); // reset green LED

			printf("Printing task info:\n");
			return_code = OSTaskQuery(CUSTOM_TASK_3_PRIORITY , &tcb_data);
			if (return_code == OS_NO_ERR) {
				printf("Task 3: Priority = %d, Status = %d, # of times active = %4ld\n",
					tcb_data.OSTCBPrio,
					tcb_data.OSTCBStat,
					tcb_data.OSTCBCtxSwCtr);
			}
			printf("\n");

			#if MY_MUTEX_ENABLE == 1
				printf("Releasing Green LED Mutex.\n");
				//return_code= OSSemPost(GREEN_LED);
				return_code= OSMutexPost(GREEN_LED);
				alt_ucosii_check_return_code(return_code);

				printf("Releasing Switch B Mutex.\n");
				//return_code = OSSemPost(SWITCH_B);
				return_code = OSMutexPost(SWITCH_B);
				alt_ucosii_check_return_code(return_code);
			#endif
		}
	}
}

// Task launcher
// It creates all the custom tasks
// And then it deletes itself
void task_launcher(void *pdata) {
	INT8U return_code = OS_NO_ERR;

	#if OS_CRITICAL_METHOD == 3
		OS_CPU_SR cpu_sr;
	#endif

	printf("Starting task launcher...\n");
	while (1) {
		OS_ENTER_CRITICAL();
		
		//IOWR(LED_GREEN_O_BASE, 0, IORD(SWITCH_GRPA_I_BASE, 0));
		//IOWR(LED_RED_O_BASE, 0, IORD(SWITCH_GRPB_I_BASE, 0));
		
		printf("Creating tasks...\n");

		return_code = OSTaskCreateExt(custom_task_0,
			NULL,
			(void *)&custom_task_stk[0][TASK_STACKSIZE-1],
			CUSTOM_TASK_0_PRIORITY,
			CUSTOM_TASK_0_PRIORITY,
			&custom_task_stk[0][0],
			TASK_STACKSIZE,
			&custom_task_tcb[0],
			0);
		alt_ucosii_check_return_code(return_code);

		return_code = OSTaskCreateExt(custom_task_1,
			NULL,
			(void *)&custom_task_stk[1][TASK_STACKSIZE-1],
			CUSTOM_TASK_1_PRIORITY,
			CUSTOM_TASK_1_PRIORITY,
			&custom_task_stk[1][0],
			TASK_STACKSIZE,
			&custom_task_tcb[1],
			0);
		alt_ucosii_check_return_code(return_code);

		return_code = OSTaskCreateExt(custom_task_2,
			NULL,
			(void *)&custom_task_stk[2][TASK_STACKSIZE-1],
			CUSTOM_TASK_2_PRIORITY,
			CUSTOM_TASK_2_PRIORITY,
			&custom_task_stk[2][0],
			TASK_STACKSIZE,
			&custom_task_tcb[2],
			0);
		alt_ucosii_check_return_code(return_code);

		return_code = OSTaskCreateExt(custom_task_3,
			NULL,
			(void *)&custom_task_stk[3][TASK_STACKSIZE-1],
			CUSTOM_TASK_3_PRIORITY,
			CUSTOM_TASK_3_PRIORITY,
			&custom_task_stk[3][0],
			TASK_STACKSIZE,
			&custom_task_tcb[3],
			0);
		alt_ucosii_check_return_code(return_code);


		printf("Finish creating tasks...\n");

		printf("PB 0: Task 0 is 3.5ms.\n");
		printf("PB 1: Task 1 is 3.5ms.\n");
		printf("PB 2: Task 2 is 2.5ms.\n");
		printf("PB 3: Task 3 is 2.5ms.\n");
		printf("\n");

		// Delay for 1 sec
		OSTimeDlyHMSM(0, 0, 1, 0);

		// Delete itself
		return_code = OSTaskDel(OS_PRIO_SELF);
		alt_ucosii_check_return_code(return_code);

		OS_EXIT_CRITICAL();
	}
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
