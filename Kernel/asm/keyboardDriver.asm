GLOBAL keyboardActivated
GLOBAL getPressedKey

section .text

keyboardActivated:
    mov rax, 0
    in al, 64h
    and al, 01h 
    ret

getPressedKey:
    mov rax, 0
    in al, 60h
    ret