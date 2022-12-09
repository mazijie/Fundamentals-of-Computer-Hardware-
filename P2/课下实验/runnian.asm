li $v0,5
syscall
li $t0,4
li $t1,100
li $t2,400
move $a1,$v0
div $a1,$t0
mfhi $a2
bgtz $a2,pingnian
div $a1,$t2
mfhi $a2
beqz $a2,runnian
div $a1,$t1
mfhi $a2
beqz $a2,pingnian
runnian:
  li $a0,1
  li $v0,1
  syscall
  li $v0,10
  syscall
pingnian:
  li $a0,0
  li $v0,1
  syscall
  li $v0,10
  syscall  
