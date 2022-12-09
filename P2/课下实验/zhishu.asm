li $v0,5
syscall
move $s0,$v0  #将待处理数字放在s0
sub $s1,$s0,1  #循环上限为n-1
li $t0,2  #被除数初始值设置为2
blt $s0,2,heshu
beq $s0,2,zhishu
loop:
  div $s0,$t0
  mfhi $t1
  beqz $t1,heshu
  addi $t0,$t0,1
  ble $t0,$s1,loop
zhishu:
  li $v0,1
  li $a0,1
  syscall
  li $v0,10
  syscall
heshu:
  li $v0,1
  li $a0,0
  syscall
  li $v0,10
  syscall