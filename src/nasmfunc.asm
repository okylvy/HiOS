; nasmfunc
; TAB=4

bits 32

; NOTE:
;   No need to add prefix '_' like _io_hlt.
;   In sample codes of the book, it's defined as
;   _io_hlt.
global io_hlt, io_cli, io_sti, io_stihlt
global io_in8, io_in16, io_in32
global io_out8, io_out16, io_out32
global io_load_eflags, io_store_eflags

section .text

; void_io_hlt(void);
io_hlt:
        hlt
        ret

; void io_cli(void);
io_cli:
        cli
        ret

; void io_sti(void);
io_sti:
        sti
        ret

; void io_stihlt(void);
io_stihlt:
        sti
        hlt
        ret

; int io_in8(int port);
io_in8:
        mov     edx, [esp+4]    ; port
        mov     eax, 0
        in      al, dx
        ret

; int io_in16(int port);
io_in16:
        mov     edx, [esp+4]    ; port
        mov     eax, 0
        in      ax, dx
        ret

; int io_in32(int port);
io_in32:
        mov     edx, [esp+4]    ; port
        mov     eax, 0
        in      eax, dx
        ret

; int io_out8(int port, int data);
io_out8:
        mov     edx, [esp+4]    ; port
        mov     al, [esp+8]     ; data
        out     dx, al
        ret

; int io_out16(int port, int data);
io_out16:
        mov     edx, [esp+4]    ; port
        mov     eax, [esp+8]    ; data
        out     dx, ax
        ret

; int io_out32(int port, int data);
io_out32:
        mov     edx, [esp+4]    ; port
        mov     eax, [esp+8]    ; data
        out     dx, eax
        ret
; int io_load_eflags(void);
io_load_eflags:
        pushfd
        pop     eax
        ret

; void io_store_eflags(int eflags)
io_store_eflags:
        mov     eax, [esp+4]
        push    eax
        popfd
        ret
