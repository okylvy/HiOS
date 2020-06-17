; ipl.asm
; IPL(Initial Program Loader)
;   The size of ipl.bin must be 512 byte.

        CYLS    equ     10

        org     0x7c00

; Description for FAT12 format floppy disc
        jmp     entry
        db      0x90
        db      "HELLOIPL"
        dw      512
        db      1
        dw      1
        db      2
        dw      224
        dw      2880
        db      0xf0
        dw      9
        dw      18
        dw      2
        dd      0
        dd      2880
        db      0, 0, 0x29
        dd      0xffffffff
        db      "HELLO-OS   "
        db      "FAT12   "
        resb     18

; Main program
entry:
        mov     ax, 0
        mov     ss, ax
        mov     sp, 0x7c00
        mov     ds, ax

        ; Read a disc
        mov     ax, 0x0820
        mov     es, ax
        mov     ch, 0       ; Cylinder
        mov     dh, 0       ; Head
        mov     cl, 2       ; Sector

readloop:
        ; Count the num of fails.
        mov     si, 0

retry:
        mov     ah, 0x02    ; Function code
        mov     al, 1       ; 1 sector
        mov     bx, 0
        mov     dl, 0x00    ; A drive
        int     0x13
        jnc     next

        ; Retry to boot.
        add     si, 1
        cmp     si, 5       ; Give up to boot at 5th error.
        jae     error
        mov     ah, 0x00
        mov     dl, 0x00
        int     0x13
        jmp     retry

next:
        mov     ax, es      ; Move the address forward 0x200.
        add     ax, 0x0020
        mov     es, ax
        add     cl, 1
        cmp     cl, 18      ; 18 sectors per 1 cylinder.
        jbe     readloop

        mov     cl, 1
        add     dh, 1
        cmp     dh, 2
        jb      readloop

        mov     dh, 0
        add     ch, 1
        cmp     ch, CYLS
        jb      readloop

        mov     [0x0ff0], ch
        jmp     0xc200

error:
        mov     si, msg

putloop:
        mov     al, [si]
        add     si, 1
        cmp     al, 0
        je      fin
        mov     ah, 0x0e
        mov     bx, 15
        int     0x10
        jmp     putloop

fin:
        hlt
        jmp     fin

msg:
        db      0x0a, 0x0a
        db      "load error"
        db      0x0a
        db      0

        resb    0x1fe-($-$$)

        ; Indicate that the device is bootable.
        db      0x55, 0xaa
