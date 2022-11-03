GLOBAL _xchgLock
GLOBAL _unlock

_xchgLock:
  mov rax, 1
  xchg rax, [rdi]    
  cmp rax, 0         
  jne _xchgLock      ; cuando el lock no es 0, se activa
  ret                ; retorna 0 si ocurre el lock


_unlock:
    mov byte [rdi], 0
    ret