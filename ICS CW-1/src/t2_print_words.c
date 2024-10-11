#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void print_words(char input_text[]) {
    int c = 0;

    while (input_text[c] != '\0') {
        // Skip spaces
        while (input_text[c] == ' ') {
            c++;
        }

        // Print the  word
        while (input_text[c] != '\0' && input_text[c] != ' ') {
            putchar(input_text[c]);
            c++;
        }

        // Print a newline character to separate words
        putchar('\n');
    }
}

// Driver code. You should not need to modify this.
int main() {
    FILE *file_pointer = fopen("task2.txt", "r");
    char *input = malloc(sizeof(char) * 1000);

    fgets(input, 1000, file_pointer);

    fclose(file_pointer);

    print_words(input);
    free(input);
    return 0;
}
