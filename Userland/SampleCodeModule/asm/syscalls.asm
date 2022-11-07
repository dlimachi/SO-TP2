GLOBAL sys_getChar
GLOBAL sys_putCharWC
GLOBAL sys_getTime
GLOBAL sys_getRegistersInfo
GLOBAL sys_clear
GLOBAL sys_PrintMem
GLOBAL sys_printCharInPos
GLOBAL sys_raiseInvOpCodeExc
GLOBAL sys_getDeciseconds
GLOBAL sys_saveRegs
GLOBAL sys_ps
GLOBAL sys_getPid
GLOBAL sys_togglePsState
GLOBAL sys_killPs
GLOBAL sys_getSeconds
GLOBAL sys_mem
GLOBAL sys_createProcess
GLOBAL sys_sem
GLOBAL sys_nice
GLOBAL sys_yield
GLOBAL sys_malloc
GLOBAL sys_free
GLOBAL sys_sem_open
GLOBAL sys_sem_close
GLOBAL sys_sem_wait
GLOBAL sys_sem_post
GLOBAL sys_block
GLOBAL sys_unblock
GLOBAL sys_pipe
GLOBAL sys_wait
GLOBAL sys_exists
GLOBAL sys_pipeOpen
GLOBAL sys_pipeClose
GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_get_fdin
GLOBAL sys_get_fdout
GLOBAL sys_putChar
GLOBAL sys_print
GLOBAL sys_printWithColor
GLOBAL sys_writeChar
GLOBAL sys_getProcessMode

SECTION .text

%macro pushState 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro syscall 1
	pushState

	mov	r10, r9 	; arg[5]
	mov	r9, r8 		; arg[4]
	mov r8, rcx  	; arg[3]
    mov rcx, rdx 	; arg[2]
	mov rdx, rsi 	; arg[1]
	mov rsi, rdi 	; arg[0]
	mov rdi, %1 	; Syscall #	
    int 80h

	popState
    ret     
%endmacro

sys_getChar:
    syscall 0
sys_putCharWC:
	syscall 1
sys_getTime:
	syscall 2
sys_clear:
	syscall 3
sys_saveRegs:
	syscall 4
sys_PrintMem:
	syscall 5
sys_printCharInPos:
	syscall 6
sys_getDeciseconds:
	syscall 7
sys_getRegistersInfo:
	syscall 8
sys_ps:
	syscall 9
sys_getPid:
	syscall 10
sys_togglePsState:
	syscall 11
sys_killPs:
	syscall 12
sys_getSeconds:
	syscall 13
sys_mem:
	syscall 14
sys_createProcess:
	syscall 15
sys_sem:
	syscall 16
sys_nice:
	syscall 17
sys_yield:
	syscall 18
sys_malloc:
	syscall 19	
sys_free:
	syscall 20
sys_sem_open:
	syscall 21
sys_sem_close:
	syscall 22
sys_sem_wait:
	syscall 23
sys_sem_post:
	syscall 24
sys_block:
	syscall 25
sys_unblock:
	syscall 26
sys_pipe:
	syscall 27
sys_wait:
	syscall 28
sys_exists:
	syscall 29
sys_pipeOpen:
	syscall 30
sys_pipeClose:
	syscall 31
sys_get_fdin:
	syscall 32
sys_get_fdout:
	syscall 33
sys_read:
	syscall 34
sys_write:
	syscall 35
sys_putChar:
	syscall 36
sys_print:
	syscall 37
sys_printWithColor:
	syscall 38
sys_writeChar:
	syscall 39
sys_getProcessMode:
	syscall 40

;Fuente: https://www.felixcloutier.com/x86/ud
sys_raiseInvOpCodeExc:
	ud2

