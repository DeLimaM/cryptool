#include <stdio.h>
#include <string.h>
#include "args_parser.h"

ParsedArgs args_init(void)
{
    ParsedArgs args = {
        .input = NULL,
        .mode = MODE_ENCRYPT,
        .encoding = BASE_64,
        .verbose = 0,
        .show_help = 0,
        .error = 0};
    return args;
}

static int is_flag(const char *arg)
{
    return arg[0] == '-';
}

static int handle_input_arg(ParsedArgs *args, int i, int argc, char *argv[])
{
    if (i + 1 < argc && !is_flag(argv[i + 1]))
    {
        args->input = argv[i + 1];
        return 1;
    }
    else
    {
        fprintf(stderr, "Error: No input string specified after %s\n", argv[i]);
        args->error = 1;
        return 0;
    }
}

static int handle_encoding_arg(ParsedArgs *args, int i, int argc, char *argv[], OperatingMode mode)
{
    args->mode = mode;

    if (i + 1 < argc && is_valid_encoding_type(argv[i + 1]))
    {
        args->encoding = string_to_encoding_type(argv[i + 1]);
        return 1;
    }
    else
    {
        fprintf(stderr, "Error: No valid encoding type specified after %s\n", argv[i]);
        args->error = 1;
        return 0;
    }
}

ParsedArgs args_parse(int argc, char *argv[])
{
    ParsedArgs args = args_init();

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--input") == 0 || strcmp(argv[i], "-i") == 0)
        {
            i += handle_input_arg(&args, i, argc, argv);
        }
        else if (strcmp(argv[i], "--encode") == 0 || strcmp(argv[i], "-e") == 0)
        {
            i += handle_encoding_arg(&args, i, argc, argv, MODE_ENCRYPT);
        }
        else if (strcmp(argv[i], "--decode") == 0 || strcmp(argv[i], "-d") == 0)
        {
            i += handle_encoding_arg(&args, i, argc, argv, MODE_DECRYPT);
        }
        else if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0)
        {
            args.verbose = 1;
        }
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            args.show_help = 1;
        }
        else
        {
            if (i > 0 && !is_flag(argv[i]) && is_flag(argv[i - 1]))
            {
                continue;
            }
            fprintf(stderr, "Warning: Unknown argument '%s'\n", argv[i]);
        }
    }

    return args;
}

void args_print_help(const char *program_name)
{
    printf("Usage: %s [OPTIONS]\n\n", program_name);
    printf("OPTIONS:\n");
    printf("  -i, --input <string>      Input string to encode/decode\n");
    printf("  -e, --encode <type>       Encode using specified type (base64, hex, uu)\n");
    printf("  -d, --decode <type>       Decode using specified type (base64, hex, uu)\n");
    printf("  -v, --verbose             Enable verbose output\n");
    printf("  -h, --help                Show this help message\n");
}