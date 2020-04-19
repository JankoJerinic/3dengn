ideal
p386n
model flat,cpp
stack 200h
align 4

codeseg

global M13_setvmode              :          proc
global M13_setmcga               :          proc
global M13_settext               :          proc
global M13_clrscr                :          proc
global M13_fillb_                :          proc
global M13_fillw_                :          proc
global M13_filld_                :          proc
global M13_putpixel_             :          proc
global M13_getpixel_             :          proc
global M13_waitvretrace          :          proc
global M13_hline_                :          proc
global M13_vline_                :          proc
global M13_line_                 :          proc
global M13_clearbuffer           :          proc
global M13_showbuffer            :          proc
global M13_getesc                :          proc
global M13_keypressed            :          proc


proc M13_setvmode
     int 10h
     ret
endp

proc M13_setmcga
     mov ax,13h
     int 10h
     ret
endp


proc M13_settext
     mov ax,3
     int 10h
     ret
endp

proc M13_clrscr
     mov edi,0A0000h
     mov ecx,4000h
     xor eax,eax
     rep stosd
     ret
endp

proc M13_fillb_
     rep stosb
     ret
endp

proc M13_fillw_
     shr ecx,1
     rep stosw
     ret
endp

proc M13_filld_
     shr ecx,2
     rep stosd
     ret
endp

proc M13_putpixel_
     add edi,eax
     shl ebx,8
     add edi,ebx
     shr ebx,2
     add edi,ebx
     mov [edi],dl
     ret
endp

proc M13_getpixel_
     add edi,eax
     shl ebx,8
     add edi,ebx
     shr ebx,2
     add edi,ebx
     mov dl,[edi]
     ret
endp

proc M13_waitvretrace
     mov dx,3DAh
     l1:
     in al,dx
     test al,8
     jz l1
     l2:
     in al,dx
     test al,8
     jnz l2
     ret
endp

proc M13_hline_
	add edi,edx
	shl ebx,8
	add edi,ebx
	shr ebx,2
	add edi,ebx
	sub ecx,edx
	mov ah,al
	push ax
	shl eax,16
	pop ax
	shr ecx,1
	jnc fillw
	stosb
	fillw:
	shr ecx,1
	jnc filldw
	stosw
	filldw:
	rep stosd
        ret
endp

proc M13_vline_
	add edi,eax
	sub ecx,ebx
	shl ebx,8
	add edi,ebx
	shr ebx,2
	add edi,ebx
	_loop:
	mov [edi],dl
	dec ecx
	add edi,140h
	or ecx,ecx
	jnz _loop
        ret
endp

proc M13_clearbuffer
	xor eax,eax
	shr ecx,2
	rep stosd
        ret
endp

proc M13_showbuffer
	shr ecx,2
	rep movsd
        ret
endp

proc M13_getesc
	in al,60H
        sub al,1
	setz al
        ret
endp

proc M13_keypressed
	mov di,41AH
	mov ax,[di]
	cmp ax,[di+2]
	setne al
        ret
endp

proc M13_line_
      push edx
      cmp ebx,eax
      jge x2gx1
      xchg eax,ebx
      xchg ecx,esi
      x2gx1:
      add edi,eax
      push ecx
      shl ecx,8
      add edi,ecx
      shr ecx,2
      add edi,ecx
      pop ecx
      neg eax
      add eax,ebx
      mov ebx,esi
      sub ebx,ecx
      jg deltay
      neg ebx
      deltay:
      cmp eax,ebx
      jng dxldy
      cmp ecx,esi
      jng v1
      mov ecx,eax
      inc ecx
      mov edx,ebx
      shl edx,1
      sub edx,eax
      mov esi,ebx
      shl esi,1
      sub ebx,eax
      shl ebx,1
      pop eax
      loop1:
            or ecx,ecx
            je fin
            dec ecx
            mov [edi],al
            or edx,edx
            jge vece1
            add edx,esi
            inc edi
            jmp endloop1
            vece1:
            add edx,ebx
            sub edi,319
            endloop1:
            jmp loop1
      v1:
      mov ecx,eax
      inc ecx
      mov edx,ebx
      shl edx,1
      sub edx,eax
      mov esi,ebx
      shl esi,1
      sub ebx,eax
      shl ebx,1
      pop eax
      loop2:
            or ecx,ecx
            je fin
            dec ecx
            mov [edi],al
            or edx,edx
            jge vece2
            add edx,esi
            inc edi
            jmp endloop2
            vece2:
            add edx,ebx
            add edi,321
            endloop2:
            jmp loop2
      dxldy:
      cmp ecx,esi
      jng v2
      mov ecx,ebx
      inc ecx
      mov edx,eax
      shl edx,1
      sub edx,ebx
      mov esi,eax
      shl esi,1
      sub eax,ebx
      shl eax,1
      mov ebx,eax
      pop eax
      loop3:
            or ecx,ecx
            je fin
            dec ecx
            mov [edi],al
            or edx,edx
            jge vece3
            add edx,esi
            sub edi,320
            jmp endloop3
            vece3:
            add edx,ebx
            sub edi,319
            endloop3:
            jmp loop3
      v2:
      mov ecx,ebx
      inc ecx
      mov edx,eax
      shl edx,1
      sub edx,ebx
      mov esi,eax
      shl esi,1
      sub eax,ebx
      shl eax,1
      mov ebx,eax
      pop eax
      loop4:
            or ecx,ecx
            je fin
            dec ecx
            mov [edi],al
            or edx,edx
            jge vece4
            add edx,esi
            add edi,320
            jmp endloop4
            vece4:
            add edx,ebx
            add edi,321
            endloop4:
            jmp loop4
      fin:
		ret
endp

end

