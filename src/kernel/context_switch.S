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
    ldr sp, [r0, #8]                 /* Load SP from pxStackMax */
    
    /* Restore next task context */ // TODO consider necessary?
    //ldr lr, [r0, #16]                         /* Restore return address */

    mov r7, #0
    
    //bx lr                            /* Return to next task */
    b TaskWrapper

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
 */
    .global PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
    /* Disable interrupts during context switch */
    cpsid i
    
    /* Save current task context */
    push {r4-r11}                    /* Save callee-saved registers */
    push {lr}                        /* Save return address */
    
    /* Get current task TCB pointer */
    ldr r0, =xCurrentTask
    ldr r0, [r0]
    
    /* Save current stack pointer to current task's TCB */
    str sp, [r0, #0]                 /* Store SP to pxTopOfStack */
    
    /* Call scheduler to get next task */
    bl vSchedulerGetNextTask
    
    /* Get next task TCB pointer */
    ldr r1, =xCurrentTask
    str r0, [r1]                     /* Update current task pointer */
    
    /* Load next task's stack pointer */
    ldr sp, [r0, #0]                 /* Load SP from pxTopOfStack */
    
    /* Restore next task context */
    pop {lr}                         /* Restore return address */
    pop {r4-r11}                     /* Restore callee-saved registers */
    
    /* Re-enable interrupts */
    cpsie i
    
    bx lr                            /* Return to next task */

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
