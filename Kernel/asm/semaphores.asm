GLOBAL _xchgLock
GLOBAL _unlock

_xchgLock:
  mov rax, 1
  xchg rax, [rdi]    ; swap atomico (bloquea acceso al bus de memoria) entre el contenido del registro al y el lock pasado por parametro
  cmp rax, 0         
  jne _xchgLock      ; si el lock no era 0, ahora ya fue activado | si el lock era 0, entra en loop
  ret                ; retorna 0 si ocurre el lock


_unlock:
    mov byte [rdi], 0
    ret