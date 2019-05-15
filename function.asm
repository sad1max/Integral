section .data
    a1 dq 4.0
    a2 dq -1.0
    
section .text

global f1
global _f1
f1:
_f1: ;  1 + 4 / (x*x + 1)
    push ebp
    mov ebp, esp
    finit
    fld1 ; 1
    fld qword[a1] ; 1, 4
    fld qword[ebp+8] ; 1, 4, x
    fld qword[ebp+8] ; 1, 4, x, x
    fmulp ; 1, 4, x*x
    fld1 ; 1, 4, x*x, 1
    faddp ; 1, 4, x*x + 1
    fdivp ; 1, 4 / (x*x + 1)
    faddp ; 1 + 4 / (x*x + 1)
    leave 
    ret
    

global f2
global _f2
f2:
_f2: ;  x*x*x
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp+8] ; x
    fld qword[ebp+8] ; x, x
    fmulp ; x*x
    fld qword[ebp+8] ; x*x, x
    fmulp ; x*x*x
    leave
    ret
    

global f3
global _f3
f3:
_f3: ; 2^(-x)
    push ebp
    mov ebp, esp
    finit
    fld qword[ebp+8] ; x
    fld1 ; 1, x
    fscale ; 2^(x)
    fld1 ; 2^x, 1
    fxch ; 1, 2^x
    fdivp ; 2^(-x)
    leave
    ret