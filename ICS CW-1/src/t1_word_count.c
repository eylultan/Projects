#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void count_words(char input_text[]) {
    int count = 0;
    
    for (int i = 0; input_text[i] != '\0'; i++) {
        if (input_text[i] == ' ') {
            count++;
        
        }
    }
    count++;  // for the last word 
    printf("%d",count );
}


// Driver code. You should not need to modify this.
int main() {
    FILE *file_pointer = fopen("task1.txt", "r");
    char *input = malloc(sizeof(char) * 1000);

    fgets(input, 1000, file_pointer);

    fclose(file_pointer);

    count_words(input);
    free(input);
    return 0;
}
