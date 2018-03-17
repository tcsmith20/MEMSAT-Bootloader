#include "task.h"

#ifdef USE_MODULE_EVENT
#include "event.h"
#endif

#ifdef USE_MODULE_WATCHDOG
#include "watchdog.h"
#endif

#include "list.h"
#include "hal_general.h"

// helper function to roll the time
void RollTimer(void);

/// task structure
typedef struct task_t {
    task_fn_t fn; /**< function to run */
    void * pointer; /**< pointer to pass to the task function if it is set */
    tint_t time; /**< the time the task was added to the queue or the time the task last ran */
    tint_t period; /**< the period for repeating the task if it is to be repeated */
} task_t;

task_t task_array[MAX_TASK_LENGTH];
list_link_t task_links[MAX_TASK_LENGTH];
list_t task_list;
task_fn_t idle_task;

uint8_t sort_tasks(task_t* a, task_t* b);

uint8_t sort_tasks(task_t* a, task_t* b) {
    // if task a is scheduled before b then return 1
    // if task a is scheduled at the same time or after b
    // then return 0
    if (a->time < b->time) return 1;
    return 0;
}

uint8_t identify_task(void * fn_ptr[], task_t * task);

uint8_t identify_task(void * fn_ptr[], task_t * task) {
    // check if the function pointer matches
    if (task->fn == fn_ptr[0]) {
        return 1;
    }
    return 0;
}

void Task_Init(void) {
    // use flag so module only gets initialized once
    static uint8_t init_flag = 0;
    if(init_flag) return;
    init_flag = 1;
    // make sure the timing module is initialized
    Timing_Init();
    // initialize the list, set the sort function pointer, set the identify function pointer
    List_Init(&task_list, sizeof(task_t), &task_array[0], MAX_TASK_LENGTH, (void*)&task_links);
    List_SetSortFunction(&task_list, (sort_fn_t)sort_tasks);
    List_SetIdentifyFunction(&task_list, (identify_fn_t)identify_task);
    // Reset the idle task
    idle_task = 0;
    // schedule the timer to be rolled over
    Task_Schedule(RollTimer, 0, TASK_ROLL_TIME, TASK_ROLL_TIME);
#ifdef _EVENT_H_
    Event_Init();
#endif
#ifdef _WATCHDOG_H_
    Watchdog_Init();
#endif
}

void SystemTick(void) {
    task_t * task_ptr;
    task_fn_t fn_ptr;
    // check if any tasks need to be linked into the queue
    List_Link(&task_list);

    task_ptr = List_GetFirst(&task_list);
    if (task_ptr) {
        if (task_ptr->time <= TimeNow()) {
            // Unlink the front of the list so if SystemTick is called from a task it wont re-run
            List_UnlinkFirst(&task_list);
            fn_ptr = task_ptr->fn; // backup fn pointer
            if(task_ptr->pointer) {
                // if a pointer is used cast to the correct fn pointer type and run
                ((task_fn_pointer_input_t)task_ptr->fn)(task_ptr->pointer);
            }else task_ptr->fn();
            // if the function pointer has changed then it must have removed
            // itself. In this case don't reschedule or remove again
            BlockInterrupts();
            if(task_ptr->fn == fn_ptr) {
                RestoreInterrupts();
                if (task_ptr->period) {
                    task_ptr->time += task_ptr->period;
                    List_LinkItem(&task_list, task_ptr);
                } else {
                    List_Remove(&task_list, task_ptr);
                }
            }else RestoreInterrupts();
        } else {
        	// Idle tasks to complete
        	if(idle_task){
        		idle_task();
        	}
        }
    } else {
    	// Idle tasks to complete
    	if(idle_task){
    		idle_task();
    	}
    }
#ifdef _EVENT_H_
    Event_Tick();
#endif
}

void Task_Queue(task_fn_t fn, void * pointer) {
    task_t * task_ptr;
    task_ptr = List_AddIndirect(&task_list);
    if (task_ptr == 0) return;

    task_ptr->fn = fn;
    task_ptr->time = TimeNow();
    task_ptr->period = 0;
    task_ptr->pointer = pointer;
}

int8_t Task_Schedule(task_fn_t fn, void * pointer,
        tint_t delay, tint_t period) {
    
    task_t * task_ptr;
    task_ptr = List_AddIndirect(&task_list);
    if (task_ptr == 0) {
#ifdef _SUBSYS_H_
        LogMsg(0x00, "E: Insufficient space in task buffer.");
#endif
        return -1;
    }

    // load the function, priority, next time to run, and period
    task_ptr->fn = fn;
    task_ptr->time = TimeNow() + delay;
    task_ptr->period = period;
    task_ptr->pointer = pointer;
    return 0;
}

int8_t Task_SetIdleTask(task_fn_t fn){
	idle_task = fn;
	//TODO: Consider how to handle if the idle task is already assigned.  Do you allow it? Return an error?
	return 0;
}

void Task_Remove(task_fn_t fn, void * pointer) {
    task_t* task_ptr;
    uint8_t i;
    // find the first task that matches in the entire task_array
    // this is because the task that is running may not be on the list and may
    // want to remove itself
    for(i = 0; i < MAX_TASK_LENGTH; i++) {
        if(task_array[i].fn == fn && (task_array[i].pointer == pointer || pointer ==0 )) {
            task_ptr = &task_array[i];
            break;
        }
    }
    // if no task matches then i would be MAX_TASK_LENGTH
    while (i < MAX_TASK_LENGTH) {
        // clear the function pointer so SystemTick will know if the task it is 
        // running was removed
        task_ptr->fn = 0; 
        List_Remove(&task_list, task_ptr);
        // don't keep looking if the function and pointer were specified
        // it will be the users responsibility to call Task_Remove if they
        // added multiple tasks with the same input pointer
        if(pointer) break;
        // look for the next match
        for(; i < MAX_TASK_LENGTH; i++) {
            if(task_array[i].fn == fn && (task_array[i].pointer == pointer || pointer ==0 )) {
                task_ptr = &task_array[i];
                break;
            }
        }
    }
}

void RollTimer(void) {
    tint_t time;
    task_t * task;
    // get the current time
    time = TimeNow();
    // for each task
    task = List_GetFirst(&task_list);
    while (task) {
        // if time is in the future then roll it
        // otherwise just let it be as TimeSince will handle the roll
        if (task->time > time) {
            // rolled task time = task time - current time
            // since current time is about to be set to 0
            task->time = task->time - time;
        }
        task = List_GetNext(&task_list, task);
    }

    // reset the system time and set the rollover time
    Timing_Roll();
}

// delay a set number of milliseconds but call SystemTick() while we wait so
// we will run system processes while we wait

void WaitMs(tint_t wait) {
    tint_t time;
    // get the current time
    time = TimeNow();
    // while time since time is less than or equal to wait
    while (TimeSince(time) <= wait) {
        // call SystemTick()
        SystemTick();
    }
}

uint8_t Task_IsScheduled(task_fn_t fn) {
    task_t * task;
    task = List_GetItem(&task_list, &fn);
    if(task) return 1;
    return 0;
}
