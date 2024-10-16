#=========================================================================================================
# COURSEWORK 1: TASK 1
#=========================================================================================================
# Inf2C Computer Systems
# David Schall
# 6 October 2023

#=========================================================================================================
# DATA SEGMENT
#=========================================================================================================
.data

#---------------------------------------------------------------------------------------------------------
# CONSTANT STRINGS
#---------------------------------------------------------------------------------------------------------
input_file:             .asciiz "task1.txt"
newline:                .asciiz "\n"
space:                  .asciiz " "

#---------------------------------------------------------------------------------------------------------
# GLOBAL VARIABLES IN MEMORY
#---------------------------------------------------------------------------------------------------------
buffer:                 .space 1000                     # A buffer used for file reading. The maxiumum
                                                        # text length is 1000 character,
                                                        # 999 characters or spaces, and one terminating
                                                        # newline.

# Your memory variables go here


#=========================================================================================================
# TEXT SEGMENT
#=========================================================================================================
.text


#---------------------------------------------------------------------------------------------------------
# MAIN CODE BLOCK (DO NOT MODIFY)
#---------------------------------------------------------------------------------------------------------
.globl main                                             # Declare the main label as globally visible
main:                                                   # This is where the program will start executing

#---------------------------------------------------------------------------------------------------------
# READ INTEGER FILE FUNCTION (DO NOT MODIFY)
#---------------------------------------------------------------------------------------------------------
read_file:
        li $v0, 13                                      # Load the system call code for open file
        la $a0, input_file                              # Load the address of the input file name
        li $a1, 0                                       # Flag for reading
        li $a2, 0                                       # Mode is ignored
        syscall

        move $s0, $v0                                   # Save the file descriptor returned by syscall

	li $v0, 14                                      # Load the system call code for reading from file
        move $a0, $s0                                   # Load the file descriptor to read
        la $a1, buffer                                  # Load the address of the buffer to write into
        li $a2, 1000                                    # Read the entire file into the buffer
        syscall


        li $v0, 16                                      # Load the system call for close file
        move $a0, $s0                                   # Load the file descriptor to close
        syscall

        jal word_count                                	# word_count();

#---------------------------------------------------------------------------------------------------------
# EXIT PROGRAM (DO NOT MODIFY)
#---------------------------------------------------------------------------------------------------------
main_end:
        li $v0, 10
        syscall

#---------------------------------------------------------------------------------------------------------
# END OF MAIN CODE BLOCK
#---------------------------------------------------------------------------------------------------------

# All below is yours ;-)

#---------------------------------------------------------------------------------------------------------
# WORD COUNT FUNCTION
#---------------------------------------------------------------------------------------------------------

word_count:
# Initialize variables
    	li $t0, 0            # $t0 = 0 -> loop index
    	li $t1, 0            # $t1 = 0 -> space coun
    	la $t2, buffer       # buffer -> t2

loop:
    	lb $t4, 0($t2)              # Load the next character from the buffer
    	beq $t4, $zero, lastLoop    # If the character is null terminated -> lastLoop 
    	li $t7, 32                  # Space = T7
    	beq $t4, $t7, count         # If the character = space -> space count + 1 
    	addi $t2, $t2, 1            # Increment buffer pointer
    	addi $t0, $t0, 1            # Increment loop index
    	j loop

count:
    	addi $t1, $t1, 1     # space count + 1
    	addi $t2, $t2, 1     # Move to the next character in buffer
    	addi $t0, $t0, 1     # Increment loop index
    	j loop

lastLoop:
   	addi $t1, $t1, 1     # for the last word space count + 1

        # Display the word count
    	li $v0, 1            	  # Set $v0 to 1 (printing an integer)
    	add $a0, $t1, $zero        # Load spaceCount into $a0
    	syscall
    	jr $ra


