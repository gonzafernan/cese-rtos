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

    task_B.c (Released 2022-06)

--------------------------------------------------------------------

    task file for FreeRTOS - Event Driven System (EDS) - Project for
    STM32F429ZI_NUCLEO_144.

    See readme.txt for project information.

-*--------------------------------------------------------------------*/


// ------ Includes -------------------------------------------------
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

/* Application includes. */
#include "app_Resources.h"
#include "task_B.h"

// ------ Macros and definitions ---------------------------------------

// ------ internal data declaration ------------------------------------
/* Task B Flag */
// uint32_t lTask_BFlag;

// ------ internal functions declaration -------------------------------

// ------ internal data definition -------------------------------------
/* Define the strings that will be passed in as the Supporting Functions parameters.
 * These are defined const and off the stack to ensure they remain valid when the
 * tasks are executing. */

const char *pcTextForTask_B    					= "- Running\r\n";

const char *pcTextForTask_B_lTasksCnt			= "Task B - lTasksCnt :";

const char *pcTextForTask_B_WaitExit			= "- Wait:   Exit\r\n\n";
const char *pcTextForTask_B_SignalContinue   	= "- Signal: Continue\r\n\n";

const char *pcTextForTask_B_WaitMutex        	= "- Wait:   Mutex\r\n\n";
const char *pcTextForTask_B_SignalMutex      	= "- Signal: Mutex\r\n\n";

MonitorQueueStruct vehicle_log;

// ------ external data definition -------------------------------------

// ------ internal functions definition --------------------------------

// ------ external functions definition --------------------------------

/*------------------------------------------------------------------*/
/* Task B thread */
void vTask_B( void *pvParameters )
{

	TaskHandle_t xOwnTaskHandle = xTaskGetCurrentTaskHandle();

	/* Receive parameters structure */
	char taskName[15];
	Task_B_Param *task_param;
	task_param = (Task_B_Param *)pvParameters;
	strcpy(taskName, task_param->taskName);
	uint32_t lTask_BFlag = task_param->lTask_BFlag;

	/* Print out the name of this task. */
	vPrintTwoStrings(taskName, pcTextForTask_B );

	xSemaphoreHandle xBinarySemaphoreExit = task_param->xBinarySemaphoreExit;

	/* As per most tasks, this task is implemented within an infinite loop.
	 *
	 * Take the semaphore once to start with so the semaphore is empty before the
	 * infinite loop is entered.  The semaphore was created before the scheduler
	 * was started so before this task ran for the first time.*/
    xSemaphoreTake( xBinarySemaphoreExit, (portTickType) 0 );

    /* Initialize vehicle log info */
    strcpy(vehicle_log.numVehicle, "ABC123");
    vehicle_log.xTask_BXHandle = xOwnTaskHandle;

    /* Reset Task B Flag	*/
    lTask_BFlag = 0;

    while( 1 )
    {
	    /* Toggle LD2 state */
		HAL_GPIO_TogglePin( LD2_GPIO_Port, LD2_Pin );

		/* Use the semaphore to wait for the event.  The task blocks
         * indefinitely meaning this function call will only return once the
         * semaphore has been successfully obtained - so there is no need to check
         * the returned value. */
		vPrintTwoStrings(taskName, pcTextForTask_B_WaitExit );
        xSemaphoreTake( xBinarySemaphoreExit, portMAX_DELAY );
        {
        	/* The semaphore is created before the scheduler is started so already
    		 * exists by the time this task executes.
    		 *
    		 * Attempt to take the semaphore, blocking indefinitely if the mutex is not
    		 * available immediately.  The call to xSemaphoreTake() will only return when
    		 * the semaphore has been successfully obtained so there is no need to check
    		 * the return value.  If any other delay period was used then the code must
    		 * check that xSemaphoreTake() returns pdTRUE before accessing the resource. */
        	vPrintTwoStrings(taskName, pcTextForTask_B_WaitMutex );
        	xSemaphoreTake( xMutex, portMAX_DELAY );
        	{
        		/* The following line will only execute once the semaphore has been
        		 * successfully obtained. */

        		/* Update Task A & B Counter */
        		lTasksCnt--;
    			vPrintStringAndNumber( pcTextForTask_B_lTasksCnt, lTasksCnt);

   			    /* Check Task A & B Counter	*/
    			if( lTasksCnt == (lTasksCntMAX - 1) )
    			{
       			    /* Set Task B Flag	*/
    				lTask_BFlag = 1;
    			}
    			/* 'Give' the semaphore to unblock the tasks. */
    			vPrintTwoStrings(taskName, pcTextForTask_B_SignalMutex );
        		xSemaphoreGive( xMutex );

   			    /* Check Task B Flag	*/
       			if( lTask_BFlag == 1 )
       			{
       			    /* Reset Task B Flag	*/
       			    lTask_BFlag = 0;

        			/* 'Give' the semaphore to unblock the task A. */
       			    vPrintTwoStrings(taskName, pcTextForTask_B_SignalContinue );
       	        	xSemaphoreGive( xCountingSemaphoreContinue );
       			}
        	}
        	xQueueSend(xQueueVehicle, &vehicle_log, 0);
        }
	}
}

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
