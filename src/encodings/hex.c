#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hex.h"
#include "../verbose.h"

char *hex_encode(const char *input)
{
    if (!input)
    {
        return NULL;
    }

    size_t input_len = strlen(input);
    size_t output_len = input_len * 2;

    char *output = malloc(output_len + 1);
    if (!output)
    {
        fprintf(stderr, "[HEX] Memory allocation failed\n");
        return NULL;
    }

    for (size_t i = 0; i < input_len; i++)
    {
        sprintf(&output[i * 2], "%02x", (unsigned char)input[i]);
        verbose_log("HEX", "Byte %zu: '%c' -> 0x%02X", i, input[i], (unsigned char)input[i]);
    }

    output[output_len] = '\0';
    return output;
}

char *hex_decode(const char *input)
{
    if (!input)
    {
        return NULL;
    }

    size_t input_len = strlen(input);

    verbose_log("HEX", "Validating input length...");
    if (input_len % 2 != 0)
    {
        fprintf(stderr, "[HEX] ERROR: Invalid input length (must be even)\n");
        return NULL;
    }
    verbose_log("HEX", "Input length %zu is valid", input_len);

    size_t output_len = input_len / 2;
    char *output = malloc(output_len + 1);
    if (!output)
    {
        fprintf(stderr, "[HEX] Memory allocation failed\n");
        return NULL;
    }

    for (size_t i = 0; i < output_len; i++)
    {
        unsigned int byte;
        if (sscanf(&input[i * 2], "%2x", &byte) != 1)
        {
            fprintf(stderr, "[HEX] ERROR: Invalid hexadecimal character at position %zu\n", i * 2);
            free(output);
            return NULL;
        }
        output[i] = (char)byte;
        verbose_log("HEX", "Byte %zu: 0x%02X -> '%c'", i, byte, output[i]);
    }

    output[output_len] = '\0';
    return output;
}