.data
score1:.space 1000
score2:.space 1000
progress:.space 1000
max:.asciiz "max:"
min:.asciiz ",min:"
space:.asciiz " "
dot:.asciiz ","
.text
li $v0,5
syscall
move $s1,$v0
li $a1,100
li $a2,-100
li $t0,0
li $t1,0
loop1:
  li $v0,5
  syscall  #读取成绩
  sw $v0,score1($t1)  #将成绩存入数组
  addi,$t0,$t0,1  #序号增加1
  addi,$t1,$t1,4  #偏移位置增加4
  blt $t0,$s1,loop1
li $t0,0
li $t1,0
loop2:
  li $v0,5
  syscall  #读取成绩
  sw $v0,score2($t1)  #将成绩存入数组
  addi,$t0,$t0,1  #序号增加1
  addi,$t1,$t1,4  #偏移位置增加4
  blt $t0,$s1,loop2
li $t0,0
li $t1,0
loop3:
  lw $t2,score1($t1)
  lw $t3,score2($t1)
  sub $t3,$t3,$t2
  sw $t3,progress($t1)
  addi $t1,$t1,4
  addi $t0,$t0,1
  blt $t3,$a1,storemin
  bgt $t3,$a2,storemax
  j none
  storemin:
    move $a1,$t3
    bgt $t3,$a2,storemax
    j none
  storemax:
    move $a2,$t3
    blt $t0,$s1,loop3
    j output
  none:
    blt $t0,$s1,loop3
output:
  li $v0,4
  la $a0,max
  syscall
  li $v0,1
  move $a0,$a2
  syscall
  li $v0,4
  la $a0,min
  syscall
  li $v0,1
  move $a0,$a1
  syscall
  li $v0,4
  la $a0,space
  syscall
  li $t0,0
  li $t1,0
  loop4:
    li $v0,1
    lw $a0,progress($t1)
    syscall
    addi $t1,$t1,4
    addi $t0,$t0,1
    bge $t0,$s1,finish
    li $v0,4
    la $a0,dot
    syscall
    j loop4
finish:
  li $v0,10
  syscall