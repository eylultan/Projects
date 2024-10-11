# COURSEWORK 1: TASK 3
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
input_file:             .asciiz "task3.txt"
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

original_words:      .space 3000    # array to store unique words

words:               .space 30      #current word

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

        jal unique_words                                # call unique_words();

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
unique_words:

# Initializing all the variables: 

    li $t0, 1                    # Initialize order of occurrence to 1
    la $t1, words                # $t1 = pointer to current words
    la $t2, buffer               # $t2 = pointer to the buffer
    la $t9, original_words       # $t9 = pointer to unique words array

# Loop: Process character by character from the buffer and store each word to  words(current word) one by one  
loop:
    lb $t4, 0($t2)               # Load the next character from the buffer
    beq $t4, $zero, endloop      # If end of buffer, finish
    addi $t2, $t2, 1             # Increment buffer pointer

    # Check if the character is a space, newline, or null-terminator
    beq $t4, 10, process_word    # If newline, process the word
    beq $t4, 32, process_word    # If space, process the word
 
    
    beq $t4, $zero, endloop      # If end of buffer, finish
    # Store the character and move to the next position
    sb $t4, 0($t1)
    addi $t1, $t1, 1
    j loop



# If the next character of the buffer is space/newline code jumps here
process_word:
    sb $zero, 0($t1)             # Null-terminate the current word

# Prepaing for the uniqueness-check of the word

    la $t1, words                # Reset pointer to the start of the current word
    la $t9, original_words       # Reset pointer to the start of the unique word lıst
    jal unique_word_check        # with jal it will return back to here and will return v0= flag of uniquness(0= not unique, 1=unique) 

    # If word is not unique -> skip to the next word in buffer
    beq $v0, 0, find_next_word_in_buffer
    la $t1, words

    # If word is unique -> store it to the unique words array
    #la $t8, original_words       # Reset pointer to the start of the current word
    
store_loop:
    lb $s1, 0($t1)               # Next character of the current word loaded to s1
    beq $s1, $zero, done_storing # If end of current word, finish storing
    sb $s1, 0($t9)               # Store the character in the unique words list
    addi $t1, $t1, 1             # Increment current word(words) pointer
    addi $t9, $t9, 1             # Increment original words(unique word array) list pointer
    j store_loop                 # Continue storing characters

done_storing:
    lb $t7, space	       # store space to t7
    sb $t7, 0($t9)             # Add space as a terminator to the unique word (original_words)
    addi $t9, $t9, 1           # Increment original_words (unique words array) pointer

    # Print order of occurrence
    move $a0, $t0                # Load order of occurrence
    li $v0, 1                    # Set system call for print integer
    syscall

    # Print space
    la $a0, space                # Load address of space character
    li $v0, 4                    # Set system call for print string
    syscall

    # Print words
    la $a0, words                # Load address of words
    li $v0, 4                    # Set system call for print string
    syscall

    # Print newline
    la $a0, newline              # Load address of newline character
    li $v0, 4                    # Set system call for print string
    syscall

    # Increment order of occurrence
    addi $t0, $t0, 1

    # Reset words
    la $t1, words

    # Move to the next word in the buffer
find_next_word_in_buffer:
    j loop                       # Go to the main loop

endloop:
    j main_end                   # Return from the function
unique_word_check:
    # Input: $t9 - address of unique words list
    #        $t1 - address of the current word
    # Output: $v0 - 0 = not unique, 1 =s unique
    li $v0, 1                    # initialize word as uniquw word is unique

    # Loop through each unique word
check_unique_word:
    lb $s1, 0($t9)               # Load first character of unique word
    beq $s1, $zero, return       # If unique words array is null-terminates -> return but check ıf the current word is still has characters left 
    la $t1, words                # initialize pointer to the current word

compare_chars:
       
    lb $t7, space                		# Store space to t7
    lb $s1, 0($t9)               		# Load character from unique word
    lb $s2, 0($t1)               		# Load character from current word
    beq $s1, $zero, store_unique 		# If unique word null-termminated -> store new word
    beq $s1, $t7, store_unique 			# If  space -> check if there is current word(s2) is also finished
    beq $s2, $zero, find_end_of_unique_word     # If end of current word -> check next unique word
    bne $s1, $s2, find_end_of_unique_word       # If characters not equal ->check next unique word

    # Move to the next character in both words
    addi $t9, $t9, 1             # Increment pointer to unique word
    addi $t1, $t1, 1             # Increment pointer to current word

    j compare_chars              # Repeat for next character
    
 find_end_of_unique_word:
 #To check the next unique word stored in original_words
       addi $t9, $t9, 1		  # Increment the pointer of the original_words
       lb $s1, 0($t9)		  # Load next character from unique words array to s1 
       beq $s1, $t7, found_end	  # If unique words char = space -> found_end
       beqz $s1, found_end	  # If unique words char = zero(null-terminated) -> found_end
        j find_end_of_unique_word # loop until end of the unique word is found
        
        found_end:,
       	 # Move to the next unique word
        addi $t9, $t9, 1 # Increment the pointer of the unique words array(original_words) by 1
        
    j check_unique_word    # Repeat for next unique word

    store_unique:
    beq $s1, $zero, skipflagchange
    bnez $s2, find_end_of_unique_word
    li $v0, 0                    # Set flag to to 0 = not unique
    la $t1, words		 # reset the words pointer
    
    skipflagchange:
    jr $ra                       # Return from function

return:
    jr $ra                     # Return from function
