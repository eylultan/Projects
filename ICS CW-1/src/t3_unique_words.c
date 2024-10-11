#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORS 100
#define MAX_WORD_LENGTH 30


void unique_words(char input_text[]) {
 // Implement here 
}


// Driver code. You should not need to modify this.
int main() {
    FILE *file_pointer = fopen("task3.txt", "r");
    char *input = malloc(sizeof(char) * 1000);

    fgets(input, 1000, file_pointer);

    fclose(file_pointer);

    unique_words(input);
    free(input);
    return 0;
}
