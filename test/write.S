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
