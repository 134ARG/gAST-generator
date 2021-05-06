.extern scanf
.extern printf
.text
.globl main
.ent main
main:
.set noreorder
.frame $s8, 104, $ra
.cpload $t9
.set reorder
addi $sp, $sp, -104
.cprestore 24
sw $ra, 20($sp)
sw $s8, 28($sp)
move $s8, $sp
li $t1, 1
sw $t1, 32($s8)
li $t1, 1
sw $t1, 36($s8)
li $t1, 4
sw $t1, 40($s8)
li $t1, 0
sw $t1, 92($s8)
li $t2, 10
sw $t2, 88($s8)
li $t2, 1
add $t3, $t2, $zero
addi $t4, $zero, 4
mul $t3, $t3, $t4
addi $t3, $t3, 48
add $t3, $t3, $s8
li $t2, 1
sw $t2, 0($t3)
jal __read
add $t2, $v0, $zero
sw $t2, 36($s8)
lw $t2, 36($s8)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
A620284382655415:
lw $t2, 88($s8)
li $t1, 0
beq $t1, $t2, A620284382655662
li $t1, 0
lw $t1, 88($s8)
li $t2, 1
sub $t2, $t1, $t2
add $t3, $t2, $zero
addi $t4, $zero, 4
mul $t3, $t3, $t4
addi $t3, $t3, 48
add $t3, $t3, $s8
lw $t2, 88($s8)
sw $t2, 0($t3)
lw $t2, 88($s8)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
lw $t1, 88($s8)
li $t2, 1
sub $t2, $t1, $t2
add $t3, $t2, $zero
addi $t4, $zero, 4
mul $t3, $t3, $t4
addi $t3, $t3, 48
add $t3, $t3, $s8
lw $t2, 0($t3)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
lw $t1, 88($s8)
li $t2, 7
sne $t2, $t1, $t2
sw $t2, 96($s8)
lw $t1, 88($s8)
li $t2, 6
sne $t2, $t1, $t2
lw $t1, 96($s8)
or $t2, $t1, $t2
sw $t2, 100($s8)
lw $t1, 88($s8)
li $t2, 5
sne $t2, $t1, $t2
lw $t1, 100($s8)
or $t2, $t1, $t2
li $t1, 0
beq $t1, $t2, A620284382655696
li $t1, 0
lw $t2, 36($s8)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
A620284382655696:
lw $t1, 88($s8)
li $t2, 7
sne $t2, $t1, $t2
sw $t2, 96($s8)
lw $t1, 88($s8)
li $t2, 6
sne $t2, $t1, $t2
lw $t1, 96($s8)
and $t2, $t1, $t2
sw $t2, 100($s8)
lw $t1, 88($s8)
li $t2, 5
sne $t2, $t1, $t2
lw $t1, 100($s8)
and $t2, $t1, $t2
li $t1, 0
beq $t1, $t2, A620284382655889
li $t1, 0
lw $t2, 88($s8)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
lw $t1, 36($s8)
li $t2, 11
add $t2, $t1, $t2
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
A620284382655889:
lw $t1, 88($s8)
li $t2, 1
sub $t2, $t1, $t2
sw $t2, 88($s8)
lw $t1, 88($s8)
li $t2, 5
seq $t2, $t1, $t2
li $t1, 0
beq $t1, $t2, A620284382656219
li $t1, 0
li $t2, 99999
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
lw $t2, 88($s8)
add $t3, $t2, $zero
addi $t4, $zero, 4
mul $t3, $t3, $t4
addi $t3, $t3, 48
add $t3, $t3, $s8
lw $t2, 0($t3)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
A620284382656219:
j A620284382655415
A620284382655662:
li $t2, 123456789
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
li $t2, 0
sw $t2, 88($s8)
A620284382656367:
lw $t1, 88($s8)
li $t2, 10
slt $t2, $t1, $t2
li $t1, 0
beq $t1, $t2, A620284382656472
li $t1, 0
lw $t2, 88($s8)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
lw $t2, 88($s8)
add $t3, $t2, $zero
addi $t4, $zero, 4
mul $t3, $t3, $t4
addi $t3, $t3, 48
add $t3, $t3, $s8
lw $t2, 0($t3)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
lw $t1, 88($s8)
li $t2, 1
add $t2, $t1, $t2
sw $t2, 88($s8)
li $t2, 123456789
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
li $t1, 1
li $t2, 3
add $t2, $t1, $t2
sw $t2, 96($s8)
li $t1, 4
li $t2, 5
mul $t2, $t1, $t2
lw $t1, 96($s8)
sub $t2, $t1, $t2
sw $t2, 100($s8)
li $t1, 6
li $t2, 7
sub $t2, $t1, $t2
lw $t1, 100($s8)
sub $t2, $t1, $t2
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
j A620284382656367
A620284382656472:
li $t2, 123456789
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
lw $t2, 36($s8)
li $t1, 0
beq $t1, $t2, A620284382656588
li $t1, 0
lw $t2, 32($s8)
li $t1, 0
beq $t1, $t2, A620284382656650
li $t1, 0
li $t2, 1
sw $t2, 32($s8)
A620284382656650:
A620284382656588:
li $t2, 2
sw $t2, 36($s8)
A620284382657164:
lw $t2, 32($s8)
li $t1, 0
beq $t1, $t2, A620284382657677
li $t1, 0
li $t2, 0
sw $t2, 32($s8)
lw $t2, 32($s8)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
j A620284382657164
A620284382657677:
A620284382657720:
li $t2, 1
add $t3, $t2, $zero
addi $t4, $zero, 4
mul $t3, $t3, $t4
addi $t3, $t3, 48
add $t3, $t3, $s8
lw $t2, 0($t3)
sw $t2, 0($s8)
lw $a0, 0($s8)
jal __write
lw $t2, 32($s8)
li $t1, 0
beq $t1, $t2, A620284382657857
li $t1, 0
j A620284382657720
A620284382657857:
main_$$epilogue:
move $sp, $s8
lw $s8, 28($sp)
lw $ra, 20($sp)
addi $sp, $sp, 104
jr $ra
.end main
.rdata
data_section_$$0: 
.asciiz "%d"
.text
.globl __read
.ent __read
__read:
.set noreorder
.frame $s8, 48, $ra
.cpload $t9
.set reorder 
addi $sp, $sp, -48
.cprestore 32
sw $ra, 28($sp)
sw $s8, 36($sp)
move $s8, $sp
__read_$$branch_0:
li $t0, 40
add $t1, $t0, $s8
la $t0, data_section_$$0
sw $t0, 0($s8)
sw $t1, 4($s8)
lw $a0, 0($s8)
lw $a1, 4($s8)
jal scanf
lw $t1, 40($s8)
move $v0, $t1
__read_$$epilogue:
move $sp, $s8
lw $s8, 36($sp)
lw $ra, 28($sp)
addi $sp, $sp, 48
jr $ra
.end __read
.rdata
data_section_$$1: 
.asciiz "%d\n"
.text
.globl __write
.ent __write
__write:
.set noreorder
.frame $s8, 40, $ra
.cpload $t9
.set reorder 
addi $sp, $sp, -40
.cprestore 32
sw $ra, 28($sp)
sw $s8, 36($sp)
move $s8, $sp
__write_$$branch_0:
la $t0, data_section_$$1
sw $t0, 0($s8)
sw $a0, 4($s8)
lw $a0, 0($s8)
lw $a1, 4($s8)
jal printf
__write_$$epilogue:
# epilogue area
move $sp, $s8
lw $s8, 36($sp)
lw $ra, 28($sp)
addi $sp, $sp, 40
jr $ra
.end __write
