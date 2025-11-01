#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encoder.h"
#include "verbose.h"
#include "encodings/registry.h"

char *encode_string(const char *input, EncodingType encoding)
{
    if (!input)
    {
        fprintf(stderr, "[ERROR] Input string is NULL\n");
        return NULL;
    }

    const Encoding *enc = registry_get_encoding(encoding);
    if (!enc)
    {
        fprintf(stderr, "[ERROR] Encoding type not found in registry\n");
        return NULL;
    }

    verbose_printf("[%s] Initializing %s encoder...\n", enc->name, enc->name);

    char *result = enc->encode(input);

    if (!result)
    {
        fprintf(stderr, "[ERROR] Encoding failed\n");
        return NULL;
    }

    return result;
}

char *decode_string(const char *input, EncodingType encoding)
{
    if (!input)
    {
        fprintf(stderr, "[ERROR] Input string is NULL\n");
        return NULL;
    }

    const Encoding *enc = registry_get_encoding(encoding);
    if (!enc)
    {
        fprintf(stderr, "[ERROR] Encoding type not found in registry\n");
        return NULL;
    }

    verbose_printf("[%s] Initializing %s decoder...\n", enc->name, enc->name);

    char *result = enc->decode(input);

    if (!result)
    {
        fprintf(stderr, "[ERROR] Decoding failed\n");
        return NULL;
    }

    return result;
}

char *encoder_process(const ParsedArgs *args)
{
    if (!args || !args->input)
    {
        fprintf(stderr, "[ERROR] Invalid arguments\n");
        return NULL;
    }

    verbose_set_enabled(args->verbose);

    if (args->verbose)
    {
        verbose_printf("[VERBOSE] Starting %s...\n",
                       args->mode == MODE_ENCRYPT ? "encoder" : "decoder");
        verbose_printf("[VERBOSE] Operation: %s\n",
                       args->mode == MODE_ENCRYPT ? "ENCODE" : "DECODE");
        verbose_printf("[VERBOSE] Input string: \"%s\"\n", args->input);
        verbose_printf("[VERBOSE] Input length: %zu bytes\n\n", strlen(args->input));
    }

    char *result = NULL;
    if (args->mode == MODE_ENCRYPT)
    {
        result = encode_string(args->input, args->encoding);
    }
    else
    {
        result = decode_string(args->input, args->encoding);
    }

    if (result && args->verbose)
    {
        verbose_printf("\n[VERBOSE] %s complete!\n",
                       args->mode == MODE_ENCRYPT ? "Encoding" : "Decoding");
        verbose_printf("[VERBOSE] Output length: %zu bytes\n", strlen(result));
    }

    return result;
}