#=========================================================================================================
# COURSEWORK 1: TASK 4
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
input_file:             .asciiz "task4.txt"
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

original_words:       .space 3000                 #Stores the unique words
words: 		  .space 30 		      # Current Word
occurence:              .space 4000                # Stores the order number for each unique word in the unique_words_array

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

        jal compress                                	# call compress();

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
# COMPRESS FUNCTION
#---------------------------------------------------------------------------------------------------------

compress:
	li $s1, 0                       # Initialize the order to 0 in register s1 -> ORDER = S1   
	la $t1, words             	# $t1 = pointer to current words
	la $t2, buffer                  # $t2 = pointer to the buffer
	la $t9, original_words       	# $t9 = pointer to unique words array
	la $s6, occurence
	lb $t7, space                    # SPACE = $T7
	lb $t8, newline                  # NEWLINE = $T8                 
                


# Loop: Process character by character from the buffer and store each word to  words(current word) one by one  
loop:	
	lb $t4, ($t2)              # Load the next character from the buffer
 	beq $t4, 32, process_word  # If space, process the word
 	beq $t4, 10, process_word  # If newline, process the word
 	# Store the character and move to the next position
 	sb  $t4, ($t1)		 
 	addi $t1, $t1, 1           # Increment 
 	addi $t2, $t2, 1           # Increment 
 	lb $t4,  ($t2)             # Load the next character from the buffer
 	j loop		           
 	

# If the next character of the buffer is space/newline code jumps here
process_word:
	li $s4, 1
	sb $t7, ($t1)               # If a space has been encountered in the buffer it means the word has ended 
				    # and then it has jumped here without storing the space therefore we should store the space at the end of the current word
	la $t1, words               # Reset pointer to the start of the current word
	la $t9, original_words 	    # Reset pointer to the start of the unique word lıst
	j compare_chars 
	
compare_chars:
	lb $s1, ($t9)             	# array character 
	lb $s2, ($t1)             	# current words character 
	beqz $s1, store_unique          # if the unique words array is empty add the current word to the array
	beq $s1, $t7, index             # if s1 = t7 -> index
	addi $t1, $t1,1                 # increment current words character pointer
	addi $t9, $t9,1	                # increment unique words array pointer
	bne $s2, $s1, find_end_of_unique_word   # if the words are not equal go to the next word
	j compare_chars


print_not_unique: 
    # get the order
    move $a0, $s4                # ORDER 
    li $v0, 1
    syscall                      # Print order

    la $a0, space                # SPACE
    li $v0, 4
    syscall                      # Print space

    # move to next word
    j move_on

 move_on:

 	la, $t1, words    # reset the pointer of the current word
 	j next_word
 
 next_word:
 	lb $s2, ($t1) 	# load the chars of the current word to s2 

 	beqz $s2, checkpoint 
 	sb $zero, ($t1)      # fill words with 0's
 	addi $t1, $t1, 1     # t1+1
 	j next_word
 	
	
index:
	
	beq $s2, $t7, determine_index  # reset the current_word variable to s2 before moving to the next word   
	j find_end_of_unique_word

determine_index: 
      	la $s5, original_words
        sub $s3, $t9, $s5     # t9- t5 = s3  
        li $t5, 30            # t5 = 30 
        div $s3, $t5          # s3/t5
        mflo $s3              # reminder of s3

        # s3 => index of the word -> will be used to print order

        j print_not_unique
        




find_end_of_unique_word:
	
	lb $s1, ($t9)    	# Load next character from unique words array to s1 
	beqz $s1, store_unique  # If unique words char = zero(null-terminated) -> store_unique
	beq $s1, $t7, found_end # If unique words char = space -> found_end
	 
	addi $t9, $t9, 1 	# Increment the pointer of the original_words
	
	j find_end_of_unique_word
	
found_end:
 	addi $t9, $t9, 1 # Increment the pointer of the unique words array(original_words) by 1
 	addi $s4, $s4, 1
 	la $t1, words 	 # Initializing the current word
 	j compare_chars 

store_unique: 
        #Store order number for the new unique word 
        addi $t0,$t0, 1
        
        sw  $t0 , ($s6)     #  order = order_array 
 
        addi $s6, $s6, 4    # address for the order 
        
        move $a0, $t0
        li $v0, 1
        syscall              # Printing order 
        
        la $a0, space
        li $v0, 4
        syscall              # Print space
        
        la $t1, words       # reset the words pointer
        j store_loop
 
 store_loop:
 	lb $s2, ($t1)      # current word
 	beqz $s2, move_on  # If it is null-terminated print newline
 	sb $s2, ($t9)       # store the unique word to the unique words array
 	addi $t1, $t1, 1
        addi $t9, $t9, 1  
        j store_loop

 

checkpoint:
	addi $t2, $t2, 1   # increment the pointer of the buffer
	la $t1, words      #intitialize the address of the current_word
	beq $t4, $t8, return
	lb $t4, ($t2)
	j loop
	
return:
	jr $ra