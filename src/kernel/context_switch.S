/**
 * @file context_switch.S
 * @brief ARM Cortex-M4 context switching assembly code
 */

    .syntax unified
    .cpu cortex-m4
    .fpu softvfp
    .thumb

    .section .text
    .align 2

/**
 * @brief Save current task context and restore next task context
 * @param R0: Pointer to TCB
 * @param LR: Pointer to function
 *
    .global vTaskWrapper
    .type vTaskWrapper, %function
vTaskWrapper:

    
    /* Call task *
    blx lr

    /* Cyclic executive tasks will never return, and thats ok. */

    /* But if it did, we mark the task as fresh (needs to be called from entry)
            and yield the task. *
    mov r0, #1
    str r0, [r0, #16]

    b vTaskYield*/
    

/**
 * @brief Save current task context and restore next task context
 * @param R0: Pointer to next task's TCB
 */
    .global vInitialContextSwitch
    .type vInitialContextSwitch, %function
vInitialContextSwitch:
    
    /* Load next task's stack pointer */
    //ldr sp, [r0, #8]                 /* Load SP from pxStackMax */
    
    /* Restore next task context */ // TODO consider necessary?
    //ldr lr, [r0, #16]                         /* Restore return address */

    //mov r7, #0
    
    //bx lr                            /* Return to next task */
    //b TaskWrapper

    //LDR     r0, =current_tcb     /* r0 = &current_tcb        */
    LDR     r0, [r0]             /* r0 = current_tcb pointer */
    LDR     r0, [r0]             /* r0 = current_tcb->stack_ptr */
    MSR     psp, r0              /* set PSP to task stack    */

    /* Switch Thread mode to use PSP */
    MOV     r0, #2               /* CONTROL.SPSEL = 1 (use PSP) */
    MSR     CONTROL, r0
    ISB                           /* sync instruction stream */

    /* Restore callee-saved registers R4–R11 from task stack */
    POP     {r4-r11}

    /* Perform an exception return.
       LR = 0xFFFFFFFD => return to Thread mode, use PSP */
    //LDR     r0, =0xFFFFFFFD
    MOV     lr, 0xFFFFFFFD
    BX      lr  



/**
 * @brief Save current task context and restore next task context
 * @param R0: Pointer to current task's TCB
 * @param R1: Pointer to next task's TCB
 */
    .global vContextSwitch
    .type vContextSwitch, %function
vContextSwitch:
    // TODO we are going to overflow this way.

    ldr r3, [r0, #20] //load status word of old task
    cmp r3, #1        //compare to 1 (dirty)
    beq skipRegisterSave // if it is, we will be resetting the stack anyway, so we can skip the register stack save

    /* Save current task context */
    push {r4-r11}                    /* Save callee-saved registers */
    push {lr}                        /* Save return address */
    
skipRegisterSave:
    /* Save current stack pointer to current task's TCB */
    str sp, [r0, #0]                 /* Store SP to pxTopOfStack */


    ldr r4, [r1, #20] //load status word
    cmp r4, #1        //compare to 1 (dirty)
    mov r0, r1
    beq vInitialContextSwitch // if so, jump to initial setup for new task (or instance)
    
    /* Load next task's stack pointer */
    ldr sp, [r1, #0]                 /* Load SP from pxTopOfStack */
    
    /* Restore next task context */
    pop {lr}                         /* Restore return address */
    pop {r4-r11}                     /* Restore callee-saved registers */
    
    bx lr                            /* Return to next task */

/**
 * @brief PendSV handler for context switching
 *
    .global PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
    /* Disable interrupts during context switch *
    cpsid i
    
    /* Save current task context *
    push {r4-r11}                    /* Save callee-saved registers *
    push {lr}                        /* Save return address *
    
    /* Get current task TCB pointer *
    ldr r0, =xCurrentTask
    ldr r0, [r0]
    
    /* Save current stack pointer to current task's TCB *
    str sp, [r0, #0]                 /* Store SP to pxTopOfStack *
    
    /* Call scheduler to get next task *
    bl vSchedulerGetNextTask
    
    /* Get next task TCB pointer *
    ldr r1, =xCurrentTask
    str r0, [r1]                     /* Update current task pointer *
    
    /* Load next task's stack pointer *
    ldr sp, [r0, #0]                 /* Load SP from pxTopOfStack *

    ldr r4, [r0, #20] //load status word
    cmp r4, #1        //compare to 1 (dirty)
    beq freshState
    
restoreState:
    /* Restore next task context *
    pop {lr}                         /* Restore return address *
    pop {r4-r11}                     /* Restore callee-saved registers *

    b converge

freshState:

    ldr lr, [r0, #16]                         /* Restore return address *

converge:
    
    /* Re-enable interrupts *
    cpsie i
    
    bx lr                            /* Return to next task */

    .global PendSV_Handler
    .type   PendSV_Handler, %function
    .extern xCurrentTask
    .extern vScheduleNextTask
    .extern first_switch_done        /* 0 until first task starts */
    .extern vInitializeTaskStack

PendSV_Handler:
    //CPSID   i                         /* optional: mask IRQs while switching */

    /* -------- Save context of current task -------- */
    MRS     r0, psp                    /* r0 = current PSP                   */

    LDR     r1, =xCurrentTask          /* r1 = &xCurrentTask                 */
    LDR     r2, [r1]                   /* r2 = xCurrentTask (TCB pointer)    */
    CBZ     r2, skip_save              /* if NULL, it's the very first switch */

/* 
    ldr     r2, [r1, #20]                  //load status word
    cmp     r2, #1                      //compare to 1 (true if instance-based, already set up)
    beq     skip_save
*/
    STMDB   r0!, {r4-r11}              /* push r4-r11 onto outgoing stack    */
    STR     r0, [r2]                   /* save updated PSP into TCB          */

skip_save:
    /* -------- Pick next task -------- */
    MOV     r12, lr                    /* preserve EXC_RETURN around BL      */
    BL      vScheduleNextTask          /* sets xCurrentTask = next task      */
    MOV     lr, r12                    /* restore EXC_RETURN                 */

    /* -------- Restore context of next task -------- */
    LDR     r1, =xCurrentTask
    LDR     r2, [r1]                   /* r2 = new xCurrentTask pointer      */
 
    /* check if instance based */
    ldr r3, [r2, #20] //load status word
    cmp r3, #0        //compare to 0 (= means normal resume)
    beq resuming_task

    // need to refresh task stack.
    push {r0-r3}
    push {lr}

    mov r0, r2

    bl vInitializeTaskStack

    pop {lr}
    pop {r0-r3}


resuming_task:
    LDR     r0, [r2]                   /* r0 = saved PSP of next task        */
    LDMIA   r0!, {r4-r11}              /* pop r4-r11 from next task stack    */
    MSR     psp, r0                     /* set PSP to next task’s top        */

    /* -------- First-task special handling -------- */
    LDR     r3, =first_switch_done
    LDR     r2, [r3]
    CBZ     r2, set_psp_once
    B       switch_done

set_psp_once:
    MOVS    r2, #1
    STR     r2, [r3]                   /* mark that we’ve switched once     */
    MOVS    r2, #2                     /* CONTROL.SPSEL = 1 (use PSP)       */
    MSR     CONTROL, r2
    ISB                                /* ensure CONTROL update is visible  */

switch_done:
    //CPSIE   i                          /* optional: unmask IRQs             */

    /* -------- Return from exception -------- */
    /* Force EXC_RETURN for Thread mode + PSP */
    LDR     lr, =0xFFFFFFFD
    BX      lr                          /* hardware restores rest of context */


/**
 * @brief Trigger PendSV exception for context switch
 */
    .global vTriggerContextSwitch
    .type vTriggerContextSwitch, %function
vTriggerContextSwitch:
    /* Set PendSV pending bit */
    ldr r0, =0xE000ED04              /* ICSR register address */
    ldr r1, =0x10000000              /* PendSV set bit */
    str r1, [r0]
    
    bx lr

    .end
