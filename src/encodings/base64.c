#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base64.h"
#include "../verbose.h"

static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int is_base64_char(char c)
{
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') ||
           c == '+' || c == '/' || c == '=';
}

static int base64_char_index(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c - 'A';
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 26;
    if (c >= '0' && c <= '9')
        return c - '0' + 52;
    if (c == '+')
        return 62;
    if (c == '/')
        return 63;
    return -1;
}

char *base64_encode(const char *input)
{
    if (!input)
    {
        return NULL;
    }

    size_t input_len = strlen(input);
    size_t output_len = ((input_len + 2) / 3) * 4;

    char *output = malloc(output_len + 1);
    if (!output)
    {
        fprintf(stderr, "[BASE64] Memory allocation failed\n");
        return NULL;
    }

    verbose_log("BASE64", "Splitting input into 3-byte chunks...");

    size_t i = 0, j = 0;
    size_t chunk_num = 0;

    while (i < input_len)
    {
        chunk_num++;

        unsigned char bytes[3] = {0};
        int bytes_in_chunk = 0;

        for (int k = 0; k < 3 && i < input_len; k++)
        {
            bytes[k] = (unsigned char)input[i++];
            bytes_in_chunk++;
        }

        unsigned char byte1 = bytes[0];
        unsigned char byte2 = bytes[1];
        unsigned char byte3 = bytes[2];

        if (verbose_is_enabled())
        {
            if (bytes_in_chunk == 3)
            {
                verbose_log("BASE64", "Processing chunk %zu/%zu: \"%c%c%c\" -> 0x%02X%02X%02X",
                            chunk_num, (input_len + 2) / 3, byte1, byte2, byte3, byte1, byte2, byte3);
            }
            else
            {
                verbose_log("BASE64", "Processing chunk %zu/%zu: \"%.*s\" (padding needed)",
                            chunk_num, (input_len + 2) / 3, bytes_in_chunk, input + i - bytes_in_chunk - 1);
            }
        }

        unsigned char index1 = byte1 >> 2;                           // shift right by 2 to get the first 6 bits
        unsigned char index2 = ((byte1 & 0x03) << 4) | (byte2 >> 4); // last 2 bits of byte1 with bitmask + first 4 bits of byte2
        unsigned char index3 = ((byte2 & 0x0F) << 2) | (byte3 >> 6); // last 4 bits of byte2 with bitmask + first 2 bits of byte3
        unsigned char index4 = byte3 & 0x3F;                         // last 6 bits of byte3 with bitmask

        output[j++] = base64_chars[index1];
        output[j++] = base64_chars[index2];
        output[j++] = (i - 1 < input_len) ? base64_chars[index3] : '=';
        output[j++] = (i < input_len) ? base64_chars[index4] : '=';

        if (verbose_is_enabled())
        {
            char chunk_output[5];
            strncpy(chunk_output, output + j - 4, 4);
            chunk_output[4] = '\0';
            verbose_log("BASE64", "Converting to 4 Base64 characters: \"%s\"", chunk_output);
        }
    }

    if (verbose_is_enabled())
    {
        int padding = 0;
        if (output[output_len - 1] == '=')
            padding++;
        if (output[output_len - 2] == '=')
            padding++;
        if (padding > 0)
        {
            verbose_log("BASE64", "Added %d padding character(s)", padding);
        }
    }

    output[output_len] = '\0';
    return output;
}

char *base64_decode(const char *input)
{
    if (!input)
    {
        return NULL;
    }

    size_t input_len = strlen(input);

    verbose_log("BASE64", "Validating input length...");
    if (input_len % 4 != 0)
    {
        fprintf(stderr, "[BASE64] ERROR: Invalid input length (must be multiple of 4)\n");
        return NULL;
    }
    verbose_log("BASE64", "Input length %zu is valid", input_len);

    verbose_log("BASE64", "Validating characters (A-Z, a-z, 0-9, +, /, =)...");
    for (size_t i = 0; i < input_len; i++)
    {
        if (!is_base64_char(input[i]))
        {
            fprintf(stderr, "[BASE64] ERROR: Invalid character '%c' at position %zu\n", input[i], i);
            return NULL;
        }
    }
    verbose_log("BASE64", "Validation complete: OK");

    size_t output_len = (input_len / 4) * 3;
    if (input[input_len - 1] == '=')
        output_len--;
    if (input[input_len - 2] == '=')
        output_len--;

    char *output = malloc(output_len + 1);
    if (!output)
    {
        fprintf(stderr, "[BASE64] Memory allocation failed\n");
        return NULL;
    }

    verbose_log("BASE64", "Processing %zu Base64 groups...", input_len / 4);

    size_t i = 0, j = 0;
    size_t group_num = 0;

    while (i < input_len)
    {
        group_num++;

        char c1 = input[i++];
        char c2 = input[i++];
        char c3 = input[i++];
        char c4 = input[i++];

        int idx1 = base64_char_index(c1);
        int idx2 = base64_char_index(c2);
        int idx3 = (c3 == '=') ? 0 : base64_char_index(c3);
        int idx4 = (c4 == '=') ? 0 : base64_char_index(c4);

        unsigned char byte1 = (idx1 << 2) | (idx2 >> 4);          // first 6 bits of idx1 and first 2 bits of idx2
        unsigned char byte2 = ((idx2 & 0x0F) << 4) | (idx3 >> 2); // last 4 bits of idx2 and first 4 bits of idx3
        unsigned char byte3 = ((idx3 & 0x03) << 6) | idx4;        // last 2 bits of idx3 and all 6 bits of idx4

        if (verbose_is_enabled())
        {
            verbose_log("BASE64", "Group %zu/%zu: \"%c%c%c%c\" -> 0x%02X%02X%02X",
                        group_num, input_len / 4, c1, c2, c3, c4, byte1, byte2, byte3);
        }

        output[j++] = byte1;

        if (c3 != '=')
        {
            output[j++] = byte2;
            if (verbose_is_enabled())
            {
                verbose_log("BASE64", "  Byte 1: 0x%02X -> '%c'", byte1, (byte1 >= 32 && byte1 <= 126) ? byte1 : '?');
                verbose_log("BASE64", "  Byte 2: 0x%02X -> '%c'", byte2, (byte2 >= 32 && byte2 <= 126) ? byte2 : '?');
            }
        }

        if (c4 != '=')
        {
            output[j++] = byte3;
            if (verbose_is_enabled() && c3 != '=')
            {
                verbose_log("BASE64", "  Byte 3: 0x%02X -> '%c'", byte3, (byte3 >= 32 && byte3 <= 126) ? byte3 : '?');
            }
        }
    }

    output[j] = '\0';
    return output;
}
