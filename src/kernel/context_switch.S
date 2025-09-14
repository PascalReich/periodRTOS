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
 * @param R0: Pointer to current task's TCB
 * @param R1: Pointer to next task's TCB
 */
    .global vContextSwitch
    .type vContextSwitch, %function
vContextSwitch:
    /* Save current task context */
    push {r4-r11}                    /* Save callee-saved registers */
    push {lr}                        /* Save return address */
    
    /* Save current stack pointer to current task's TCB */
    str sp, [r0, #0]                 /* Store SP to pxTopOfStack */
    
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
    ldr r0, =pxCurrentTaskTCB
    ldr r0, [r0]
    
    /* Save current stack pointer to current task's TCB */
    str sp, [r0, #0]                 /* Store SP to pxTopOfStack */
    
    /* Call scheduler to get next task */
    bl vSchedulerGetNextTask
    
    /* Get next task TCB pointer */
    ldr r1, =pxCurrentTaskTCB
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
