.model tiny
.data

newline db 0dh,0ah,'$'
TRUE db 'TRUE$'
FALSE db 'FALSE$'
a1 db ?
a2 db ?
b1 db ?
b2 db ?
avar db ?
bvar db ?
str7 db 'b1 is less than b2',0ah,0dh,'$'
str8 db 'b1 is greater that b20',0ah,0dh,'$'

.code
.startup

lea si,avar 
 mov cl,1 
 mov [si],cl 

lea si,bvar 
 mov cl,1 
 mov [si],cl 

;Multiple Assignment blocks start here 

lea si,a1 
 mov cl,1 
 mov [si],cl 

lea si,a2 
 mov cl,2 
 mov [si],cl 


lea si,b1 
 mov cl,1 
 mov [si],cl 


lea si,b2 
 mov cl,2 
 mov [si],cl 



;Multiple Assignment blocks ends here 

lea si,avar 
 mov al,[si]
 cmp al,1 
 jz x1 
 jnz x2

x1: 

lea si,bvar 
 mov al,[si]
 cmp al,1 
 jz x3 
 jnz x4

x3: 

mov ah,09h 
 lea dx,str7 
 int 21h
 jmp x2 

x4: 

mov ah,09h 
 lea dx,str8 
 int 21h
 jmp x2 

x2: 


.exit
end
