  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # edited 2022/2 by Yannick Knoll
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.global delay
.global time2string 
.global hexasc
.global enable_interrupts
.data
.align 2
mytime: .word 0x5957
timstr: .ascii "text more text lots of text\0"
.text

# enables interrupts globally
enable_interrupts:
	ei
	jr $ra

main:
    # print timstr
    la  $a0,timstr
    li  $v0,4
    syscall
    nop
    # wait a little
    li  $a0, 1
    jal delay
    nop
    # call tick
    la  $a0,mytime
    jal tick
    nop
    # call your function time2string
    la  $a0,timstr
    la  $t0,mytime
    lw  $a1,0($t0)
    jal time2string
    nop
    # print a newline
    li  $a0,10
    li  $v0,11
    syscall
    nop
    # go back and do it all again
    j   main
    nop
# tick: update time pointed to by $a0
tick:   lw  $t0,0($a0)  # get time
    addiu   $t0,$t0,1   # increase
    andi    $t1,$t0,0xf # check lowest digit
    sltiu   $t2,$t1,0xa # if digit < a, okay
    bnez    $t2,tiend
    nop
    addiu   $t0,$t0,0x6 # adjust lowest digit
    andi    $t1,$t0,0xf0    # check next digit
    sltiu   $t2,$t1,0x60    # if digit < 6, okay
    bnez    $t2,tiend
    nop
    addiu   $t0,$t0,0xa0    # adjust digit
    andi    $t1,$t0,0xf00   # check minute digit
    sltiu   $t2,$t1,0xa00   # if digit < a, okay
    bnez    $t2,tiend
    nop
    addiu   $t0,$t0,0x600   # adjust digit
    andi    $t1,$t0,0xf000  # check last digit
    sltiu   $t2,$t1,0x6000  # if digit < 6, okay
    bnez    $t2,tiend
    nop
    addiu   $t0,$t0,0xa000  # adjust last digit
tiend:  sw  $t0,0($a0)  # save updated result
    jr  $ra     # return
    nop
  # you can write your code for subroutine "hexasc" below this line
  #
  
hexasc:
    andi    $a0,$a0,0x0000000f  # ignores everything but last 4 bits
    slti    $t0,$a0,10  # if input is 0 - 9
    bne $t0,$0,num
    nop
    slti    $t0,$a0,16  # if input is 10 - 15
    bne $t0,$0,letter
    nop
    j   end_hexasc  # doesnt change anything if input > 15, REDUNDANT
    nop
 num:
    addi    $v0,$a0,0x30    # turns 0 into 0x30 (0 in ASCII)
    j   end_hexasc
    nop
 letter:
    addi    $v0,$a0,55  # turns 10 into 0x41 (A in ASCII)
 end_hexasc:
    jr  $ra
    nop
    
delay:
    ble $a0,$0,end_delay    # never execute loop if ms <= 0
    nop
    li  $t0,0       # i = 0
    li  $t1,2000    # t1 = 4711
loop1:
    addi    $a0,$a0,-1  # ms--
    li  $t0,0       # i = 0
    loop2:
        addi    $t0,$t0,1   # i++
        blt $t0,$t1,loop2   # (i < 4711) ?
        nop
    bnez    $a0,loop1   # (ms > 0) ?
    nop
end_delay:
    jr  $ra
    nop
    
# inputs: $a0 = output address
#     $a1 = time info (16 lsb) 
# output: none
time2string:
# filling up stack
    PUSH    $ra
    PUSH    $s0
    PUSH    $s1
# preparing s registers
    move    $s0,$a0     # saving the address from $a0 to $s0 
    move    $s1,$a1     # saving the time in $s1 s1 = a1
# digit 1
    srl $a0,$s1,12  # calling first digit
    jal hexasc      # calling hexasc
    nop
    sb  $v0,0($s0)  # saving ascii value of the digit to s0
    addi    $s0,$s0,1   # updating address s0
# digit 2
    srl $a0,$s1,8   # calling 2nd
    jal hexasc      # calling hexasc
    nop
    sb  $v0,0($s0)  # saving ascii value of the digit to s0
    addi    $s0,$s0,1   # updating address s0
# column
    li  $t0,0x3A
    sb  $t0,0($s0)
    addi    $s0,$s0,1
# digit 3
    srl $a0,$s1,4   # calling 3rd
    jal hexasc      # calling hexasc
    nop
    sb  $v0,0($s0)  # saving ascii value of the digit to s0
    addi    $s0,$s0,1   # updating address s0
# digit 4
    srl $a0,$s1,0   # calling last digit
    jal hexasc      # calling hexasc
    nop
    sb  $v0,0($s0)  # saving ascii value of the digit to s0
    addi    $s0,$s0,1   # updating address s0
# surprise assignment: check if last digits both equal zero
	srl $t0, $s1, 4
	andi $t0, 0xf
	
	move $t1, $s1
	andi $t1, 0xf
	
	bnez  $t0, skip
	nop
	bnez $t1, skip
	nop
	
	# append X
	li $t0, 0x58
	sb $t0, 0($s0)
	addi $s0, $s0, 1
	skip:
# zeros
    sb  $0,0($s0)   # saving 00 at the end

# clearing stack
    POP $s1
    POP $s0
    POP $ra
    jr  $ra
    nop
	