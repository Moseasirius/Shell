#define SUCCESS 0
#define FAILURE 1
#define MAX_INPUT_LENGTH 1024
#define SUPPORT_FGETS 1
#define SUPPORT_GETLINE 0


typedef enum Ret {
    SH_SUCCESS = 0,
    SH_FAILURE
} Ret;

#if SUPPORT_FGETS
// char input_buffer[MAX_INPUT_LENGTH]; // Buffer to store user input
extern char input_buffer[MAX_INPUT_LENGTH]; // Buffer to store user input
Ret get_input(char* buffer, size_t size);
#endif
#if SUPPORT_GETLINE
Ret get_input(char** buffer, size_t size);
#endif

Ret parse_input(char* input, char*** args, int* arg_count);
