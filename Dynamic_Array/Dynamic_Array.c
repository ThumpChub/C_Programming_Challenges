#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

/*
 * Complete the 'dynamicArray' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. 2D_INTEGER_ARRAY queries
 */

/*
 * To return the integer array from the function, you should:
 *     - Store the size of the array to be returned in the result_count variable
 *     - Allocate the array statically or dynamically
 *
 * For example,
 * int* return_integer_array_using_static_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     static int a[5] = {1, 2, 3, 4, 5};
 *
 *     return a;
 * }
 *
 * int* return_integer_array_using_dynamic_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     int *a = malloc(5 * sizeof(int));
 *
 *     for (int i = 0; i < 5; i++) {
 *         *(a + i) = i + 1;
 *     }
 *
 *     return a;
 * }
 *
 */
int* dynamicArray(int n, int queries_rows, int queries_columns, int** queries, int* result_count) {
    //Determine the number of type 2 queries, and size the answers array accordingly
    struct SecondDimension {
      int* innerArray;
      int size;
    };
    struct FirstDimension {
      struct SecondDimension outerArray[n];
      int size = n;
    };
    *result_count = 0;
    int lastAnswer = 0;
    for(int i = 0; i < queries_rows; i++)
    {
      if(queries[i][0] == '2')
      {
        *result_count += 1;
      }
    }

    struct FirstDimension arr;

    //int** arr = (int**)calloc(n, sizeof(int*));
    int* answers = (int*)malloc(*result_count * sizeof(int));
    int answerIndex = 0;

    //Determine if the current query is of type 1 or type 2
    for(int i = 0; i < queries_rows; i++)
    {
      int x = queries[i][1];
      int y = queries[i][2];
      int idx = ((x ^ lastAnswer) % n);

      if(queries[i][0] == '1')
      {
        //perform query type 1
        //Still building...
        //Check if arr[idx] exists...
        if(arr.size !> idx)
        {
          arr.size += 1;
          arr.outerArray = (struct SecondDimension*)realloc(arr.outerArray, arr.size * sizeof(struct SecondDimension));
          //int size = idx - 1;
          //arr = (int**)realloc(arr, size * sizeof(int*));
        }
        //int size = sizeof(arr[idx]) / sizeof(int);
        arr.outerArray[idx].size += 1;
        arr.outerArray[idx].innerArray = (int*)realloc(arr.outerArray[idx].innerArray, arr.outerArray[idx].size * sizeof(int));
        arr.outerArray[idx].innerArray[arr.outerArray[idx].size] = y;
      }
      else
      {
        //perform query type 2
        //Rewrite to use structs
        int temp = y % (sizeof(arr[idx]) / sizeof(arr[idx][0]));
        lastAnswer = arr[idx][temp];
        answers[answerIndex++] = lastAnswer;
      }
    }
    int size = sizeof(answers) / sizeof(answers[0]);
    for(int i = 0; i < size; i++)
    {
      printf("%d ", answers[i]);
    }
    return answers;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int q = parse_int(*(first_multiple_input + 1));

    int** queries = malloc(q * sizeof(int*));

    for (int i = 0; i < q; i++) {
        *(queries + i) = malloc(3 * (sizeof(int)));

        char** queries_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 3; j++) {
            int queries_item = parse_int(*(queries_item_temp + j));

            *(*(queries + i) + j) = queries_item;
        }
    }

    int result_count;
    int* result = dynamicArray(n, q, 3, queries, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));

        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}
