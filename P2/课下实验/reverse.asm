.data 
.eqv n $s0
.eqv x $s1
.eqv y $s2
.eqv i $s3
.eqv k $s4
.eqv temp1 $s5
.eqv temp2 $s6
str: .space 10000
enter: .asciiz "\n"
.text 
li $v0,5
syscall
move n,$v0
li $v0,5
syscall
move x,$v0
li $v0,5
syscall
move y,$v0
addi $a1,n,1
li $v0,8
la $a0,str
syscall
move i,x
move k,y
loop:
lb temp1,str(i)
lb temp2,str(k)
sb temp2,str(i)
sb temp1,str(k)
addi i,i,1
addi k,k,-1
ble i,k,loop
la $a0,enter
li $v0,4
syscall
la $a0,str
li $v0,4
syscall
li $v0,10
syscall