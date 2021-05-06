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
