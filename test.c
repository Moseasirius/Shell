#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

int main() {

    // test_get_input();
    test_parse_input();

    return 0;          
}
void test_get_input() {

#if SUPPORT_FGETS
    // Test get_input with fgets
    if (get_input(input_buffer, sizeof(input_buffer)) == SH_SUCCESS) {
        printf("You entered: %s\n", input_buffer);
    } else {
        printf("Failed to read input using fgets\n");
    }
#endif
#if SUPPORT_GETLINE
    // Test get_input with getline
    char* input_buffer = NULL;
    if (get_input(&input_buffer, 0) == SH_SUCCESS) {
        printf("You entered: %s\n", input_buffer);
        free(input_buffer); // Free the allocated memory
    } else {
        printf("Failed to read input using getline\n");
    }
#endif
}
void test_parse_input() {
    // char* input = "arg1 arg2 arg3"; // 错误写法
    // 传入字符串字面量（只读内存）给 parse_input，会直接段错误 segfault
    // "arg1 arg2 arg3" 存放在只读常量段 (.rodata)，不允许写修改。
    // token = strtok(input, " ");
    // strtok() 会原地修改传入字符串，把分隔符改写为 \0，所以传入的字符串必须是可写的。
    // 尝试修改只读字符串 → 未定义行为，大概率直接崩溃段错误
    // char input[] = "arg1 arg2 arg3"; // 正确写法，传入可写的字符数组

    char* input = strdup("arg1 arg2 arg3"); // 正确写法，传入可写的字符数组
    if (input == NULL) {
        perror("Failed to allocate memory for input");
        return;
    }
    char** args = NULL;
    int arg_count = 0;

    if (parse_input(input, &args, &arg_count) == SH_SUCCESS) {
        printf("Parsed %d arguments:\n", arg_count);
        for (int i = 0; i < arg_count; i++) {
            printf("Argument %d: %s\n", i + 1, args[i]);
            free(args[i]); // Free each argument
        }
        free(args); // Free the array of arguments
    } else {
        printf("Failed to parse input\n");
    }
    free(input); // Free the allocated memory
}