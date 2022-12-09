li $v0,5
syscall
move $s0,$v0
li $v0,5
syscall
move $s1,$v0
bgt $s0,$s1,skipreverse
move $t0,$s0
move $s0,$s1
move $s1,$t0
skipreverse:
div $s0,$s1
mfhi $t0
beqz $t0,output
loop:
  move $s0,$s1
  move $s1,$t0
  div $s0,$s1
  mfhi $t0
  bnez $t0,loop
output: 
  li $v0,1
  move $a0,$s1
  syscall
  li $v0,10
  syscall