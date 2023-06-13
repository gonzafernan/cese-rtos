/* Copyright 2020, Juan Manuel Cruz.
 * All rights reserved.
 *
 * This file is part of Project => freertos_app_Example002
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


/*--------------------------------------------------------------------*-

    app.c (Released 2022-06)

--------------------------------------------------------------------

    app file for FreeRTOS - Event Driven System (EDS) - Project for
    STM32F429ZI_NUCLEO_144.

    See readme.txt for project information.

-*--------------------------------------------------------------------*/


// ------ Includes -----------------------------------------------------
/* Project includes. */
#include "main.h"
#include "cmsis_os.h"

/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Demo includes. */
#include "supporting_Functions.h"

/* Application & Tasks includes. */
#include "app_Resources.h"
#include "app.h"
#include "task_A.h"
#include "task_B.h"
#include "task_Test.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------
/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
 * semaphore that is used to synchronize a task with other task. */
xSemaphoreHandle xBinarySemaphoreEntry;
xSemaphoreHandle xBinarySemaphoreExit[Task_BQuantity];
// xSemaphoreHandle xBinarySemaphoreContinue;

/* Replace binary semaphore with counting semaphore */
xSemaphoreHandle xCountingSemaphoreContinue;

/* Declare a variable of type xSemaphoreHandle.  This is used to reference the
 * mutex type semaphore that is used to ensure mutual exclusive access to ........ */
xSemaphoreHandle xMutex;

/* Declare a variable of type xTaskHandle. This is used to reference tasks. */
xTaskHandle vTask_AHandle;
xTaskHandle vTask_BHandle[Task_BQuantity];
xTaskHandle vTask_TestHandle;

/* Task B names */
char Task_BNames[2][10] = {"Task B1", "Task B2"};
//char pcTextForTask_B[2][100] = {"==> Task B1 - Running\r\n", "==> Task B2 - Running\r\n"};
//char pcTextForTask_B_lTasksCnt[2][100] = {"  <=> Task    B1 - lTasksCnt :", "  <=> Task    B2 - lTasksCnt :"};
//char pcTextForTask_B_WaitExit[2][100] = {"  ==> Task    B1 - Wait:   Exit        \r\n\n", "  ==> Task    B2 - Wait:   Exit        \r\n\n"};
//char pcTextForTask_B_SignalContinue[2][100] = {"  ==> Task    B1 - Signal: Continue ==>\r\n\n", "  ==> Task    B2 - Signal: Continue ==>\r\n\n"};
//char pcTextForTask_B_WaitMutex[2][100] = {"  ==> Task    B1 - Wait:   Mutex       \r\n\n", "  ==> Task    B2 - Wait:   Mutex       \r\n\n"};
//char pcTextForTask_B_SignalMutex[2][100] = {"  ==> Task    B1 - Signal: Mutex    ==>\r\n\n", "  ==> Task    B2 - Signal: Mutex    ==>\r\n\n"};
Task_B_Param Task_BParam[Task_BQuantity];

/* Task A & B Counter	*/
uint32_t	lTasksCnt;

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
const char *pcTextForMain = "freertos_app_Example002 is running: parking lot\r\n\n";

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------


/*------------------------------------------------------------------*/
/* App Initialization */
void appInit( void )
{
	/* Print out the name of this Example. */
  	vPrintString( pcTextForMain );

    /* Before a semaphore is used it must be explicitly created.
     * In this example a binary semaphore is created. */
    vSemaphoreCreateBinary( xBinarySemaphoreEntry    );
    //vSemaphoreCreateBinary( xBinarySemaphoreExit     );
    // vSemaphoreCreateBinary( xBinarySemaphoreContinue );

    /* Create counting semaphore */
    xCountingSemaphoreContinue = xSemaphoreCreateCounting(1, 0);

    /* Check the semaphore was created successfully. */
	configASSERT( xBinarySemaphoreEntry    !=  NULL );
	// configASSERT( xBinarySemaphoreExit     !=  NULL );
	configASSERT( xCountingSemaphoreContinue !=  NULL );

    /* Add semaphore to registry. */
	vQueueAddToRegistry(xBinarySemaphoreEntry,    "xBinarySemaphoreEntry");
    //vQueueAddToRegistry(xBinarySemaphoreExit,     "xBinarySemaphoreExit");
    vQueueAddToRegistry(xCountingSemaphoreContinue, "xCountingSemaphoreContinue");

    /* Before a semaphore is used it must be explicitly created.
     * In this example a mutex semaphore is created. */
    xMutex = xSemaphoreCreateMutex();

    /* Check the mutex was created successfully. */
    configASSERT( xMutex !=  NULL );

    /* Add mutex to registry. */
	vQueueAddToRegistry(xMutex, "xMutex");

	BaseType_t ret;

    /* Task A thread at priority 2 */
    ret = xTaskCreate( vTask_A,						/* Pointer to the function thats implement the task. */
					   "Task A",					/* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. 				*/
					   NULL,						/* We are not using the task parameter.		*/
					   (tskIDLE_PRIORITY + 2UL),	/* This task will run at priority 1. 		*/
					   &vTask_AHandle );				/* We are using a variable as task handle.	*/

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );

    /* Task B thread at priority 2 */
    for (uint8_t i = 0; i < Task_BQuantity; i++)
    {
    	/* Create a binary semaphore for each exit task B */
    	vSemaphoreCreateBinary( xBinarySemaphoreExit[i]     );
    	configASSERT( xBinarySemaphoreExit[i]     !=  NULL );

    	/* Initialize parameters structure for task B */
    	Task_BParam[i].taskId = i;
    	Task_BParam[i].xBinarySemaphoreExit = xBinarySemaphoreExit[i];
    	Task_BParam[i].lTask_BFlag = 0;

    	//vQueueAddToRegistry(xBinarySemaphoreExit,     "xBinarySemaphoreExit");
    	/* Create Task_BQuantity tasks vTaskB */
    	ret = xTaskCreate( vTask_B,						/* Pointer to the function thats implement the task. */
    					   Task_BNames[i],					/* Text name for the task. This is to facilitate debugging only. */
						   (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. 				*/
						   (void *)&Task_BParam[i],						/* Receive the semaphore as parameter.		*/
						   (tskIDLE_PRIORITY + 2UL),	/* This task will run at priority 1. 		*/
						   &vTask_BHandle[i] );				/* We are using a variable as task handle.	*/

		/* Check the task was created successfully. */
		configASSERT( ret == pdPASS );
    }

	/* Task Test at priority 1, periodically excites the other tasks */
    ret = xTaskCreate( vTask_Test,					/* Pointer to the function thats implement the task. */
					   "Task Test",					/* Text name for the task. This is to facilitate debugging only. */
					   (2 * configMINIMAL_STACK_SIZE),	/* Stack depth in words. 				*/
					   NULL,						/* We are not using the task parameter.		*/
					   (tskIDLE_PRIORITY + 1UL),	/* This task will run at priority 2. 		*/
					   &vTask_TestHandle );			/* We are using a variable as task handle.	*/

    /* Check the task was created successfully. */
    configASSERT( ret == pdPASS );
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
