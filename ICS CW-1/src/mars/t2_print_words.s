#=========================================================================================================
# COURSEWORK 1: TASK 2
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
input_file:             .asciiz "task2.txt"
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

        jal print_words                                	# call print_words();

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
# PRINT WORDS FUNCTION
#---------------------------------------------------------------------------------------------------------

print_words:
   # Initialize registers
    li $t0, 0      # $t0 will be used to index the buffer
    li $t1, 0      # $t1 will be used to indicate if we are inside a word

loop:
    lb $t2, buffer($t0)  # Load the current character into $t2

    # Check for the end of the string
    beqz $t2, done

    # Check for space or newline
    li $t3, 32   # ASCII value for space
    li $t4, 10   # ASCII value for newline
    beq $t2, $t3, skip_space
    beq $t2, $t4, skip_space

    # Check if we are already inside a word
    beqz $t1, new_word

    # Not a new word, continue
    j continue

new_word:
    # We've found the start of a new word
    li $t1, 1  # Set the flag to indicate we're inside a word

continue:
    # Print the current character if we are inside a word
    beqz $t1, skip_char
    li $v0, 11
    move $a0, $t2
    syscall

skip_char:
    addi $t0, $t0, 1  # Move to the next character
    j loop

skip_space:
    # If it's a space or newline, we are not inside a word
    li $t1, 0

    # Print a newline character to separate words
    li $v0, 11
    li $a0, 10  # ASCII code for newline
    syscall

    addi $t0, $t0, 1
    j loop

done:
    # Exit the function
    jr $ra