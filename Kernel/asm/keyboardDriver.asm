GLOBAL keyActivated
GLOBAL getKeyCode

section .text

keyActivated:
  push rbp
  mov rbp, rsp

  xor rax, rax
  in al, 64h      ;lee el puerto 64
  and ax, 0x01    ;dejo solo el ultimo bit prendido
              ;Si es 0, no hay tecla prendida
              ;Si es 1, hay tecla apretada
  mov rsp, rbp
  pop rbp
  ret

getKeyCode:
  push rbp
  mov rbp, rsp

  xor rax, rax
  in al, 60h

  mov rsp, rbp
  pop rbp
  ret
