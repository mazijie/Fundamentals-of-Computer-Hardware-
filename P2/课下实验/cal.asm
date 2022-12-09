li $v0,5
syscall
move $s0,$v0
li $v0,5
syscall
move $s1,$v0
li $v0,5
syscall
move $s2,$v0
beqz $s2,jiafa
beq $s2,1,jianfa
beq $s2,2,chengfa
beq $s2,3,chufa
jiafa:
add $a0,$s0,$s1
j output
jianfa:
sub $a0,$s0,$s1
j output
chengfa:
mul $a0,$s0,$s1
j output
chufa:
div $a0,$s0,$s1
output:
li $v0,1
syscall
li $v0,10
syscall