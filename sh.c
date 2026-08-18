#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

//
// You should use the following functions to print information
// Do not modify these functions
//

// Track virtual environment state
static int env_active = 0;
static char* env_path = NULL;
static char* original_path = NULL;

void print_prompt() {
    if (env_active) {
        printf("(env) sh > ");
    } else {
        printf("sh > ");
    }
    fflush(stdout);
}

void print_invalid_syntax() {
    printf("Invalid Syntax\n");
    fflush(stdout);
}

void print_command_not_found() {
    printf("Command Not Found\n");
    fflush(stdout);
}

void print_execution_error() {
    printf("Execution Error\n");
    fflush(stdout);
}

void print_blocked_syscall(char* syscall_name, int count, ...) {
    va_list args;
    va_start(args, count);
    printf("Blocked Syscall: %s ", syscall_name);
    for (int i = 0; i < count; i++) {
        char* arg = va_arg(args, char*);
        printf("%s ", arg);
    }
    printf("\n");
    fflush(stdout);
}

// 
// You can add your own functions here
//
#define SUCCESS 0
#define FAILURE 1
#define MAX_INPUT_LENGTH 1024
#define SUPPORT_FGETS 0
#define SUPPORT_GETLINE 1


typedef enum Ret {
    SH_SUCCESS = 0,
    SH_FAILURE
} Ret;
#if SUPPORT_FGETS
char input_buffer[MAX_INPUT_LENGTH]; // Buffer to store user input
Ret get_input(char* buffer, size_t size);
#endif
#if SUPPORT_GETLINE
Ret get_input(char** buffer, size_t size);
#endif
int main() {
    while(1) {
        print_prompt();
        // TODO
        // Show your intelligence
#if SUPPORT_FGETS
        if (get_input(input_buffer, sizeof(input_buffer)) == SH_SUCCESS) {
            // Process the input
            printf("You entered: %s\n", input_buffer);
        }
#endif
#if SUPPORT_GETLINE
        char* input_buffer; // support for getline
        if(get_input(&input_buffer, 0) == SH_SUCCESS) {
            // Process the input
            printf("You entered: %s\n", input_buffer);
            free(input_buffer); // Free the allocated memory
        } // If the last character is a newline, remove it
#endif
        break;
    }
}
/*
*
 * Function: get_input
 * -------------------
 * Reads a line of input from the user and stores it in the provided buffer.
 *
 * @buffer: A pointer to the buffer where the input will be stored.
 * @size: The size of the buffer.
 *
 * @returns: SH_SUCCESS if input was read successfully, SH_FAILURE otherwise.
*/
#if SUPPORT_FGETS
Ret get_input(char* buffer, size_t size) {
    printf("Using fgets to read input\n");
    if (fgets(buffer, size, stdin) == NULL) {
        return SH_FAILURE;
    }
    // Remove trailing newline character
    buffer[strcspn(buffer, "\n")] = '\0';
    return SH_SUCCESS;

}
#endif
/*
* Function: get_input
* -------------------
* Reads a line of input from the user using getline and stores it in the provided buffer.
*
* @buffer: A pointer to the buffer where the input will be stored.
* @size: The size of the buffer (not used in this implementation).
*
* @returns: SH_SUCCESS if input was read successfully, SH_FAILURE otherwise.
*/
#if SUPPORT_GETLINE
/*
Note: the buffer must char** because getline will allocate memory for the buffer and update the pointer to point to the newly allocated memory.
if you pass char* buffer, getline will not be able to update the pointer and you will have a memory leak.
Also, you must free the buffer after you are done using it to avoid memory leaks.
Also, the buffer cannot be an array, because arrays are not modifiable lvalues and cannot be passed to functions that expect a pointer to a pointer.
*/
Ret get_input(char** buffer, size_t size) {
    printf("Using getline to read input\n");
    size_t len = 0;
    ssize_t read = getline(buffer, &len, stdin);
    if (read == -1) {
        return SH_FAILURE;
    }
    // Remove trailing newline character
    (*buffer)[strcspn(*buffer, "\n")] = '\0';
    return SH_SUCCESS;
}
#endif
