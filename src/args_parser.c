#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "args_parser.h"

ParsedArgs args_init(void)
{
    ParsedArgs args = {
        .input = NULL,
        .output_file = NULL,
        .mode = MODE_ENCRYPT,
        .encoding = BASE_64,
        .verbose = 0,
        .show_help = 0,
        .error = 0,
        .is_file_input = 0};
    return args;
}

static int file_exists(const char *path)
{
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

static char *read_file_contents(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(file_size + 1);
    if (!content)
    {
        fprintf(stderr, "Error: Memory allocation failed for file content\n");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(content, 1, file_size, file);
    content[read_size] = '\0';

    if (read_size > 0 && content[read_size - 1] == '\n')
    {
        content[read_size - 1] = '\0';
    }

    fclose(file);
    return content;
}

static int is_flag(const char *arg)
{
    return arg[0] == '-';
}

static int handle_input_arg(ParsedArgs *args, int i, int argc, char *argv[])
{
    if (i + 1 < argc && !is_flag(argv[i + 1]))
    {
        const char *input_arg = argv[i + 1];

        if (file_exists(input_arg))
        {
            char *file_content = read_file_contents(input_arg);
            if (!file_content)
            {
                args->error = 1;
                return 0;
            }
            args->input = file_content;
            args->is_file_input = 1;
        }
        else
        {
            args->input = argv[i + 1];
            args->is_file_input = 0;
        }
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

static int handle_output_arg(ParsedArgs *args, int i, int argc, char *argv[])
{
    if (i + 1 < argc && !is_flag(argv[i + 1]))
    {
        args->output_file = argv[i + 1];
        return 1;
    }
    else
    {
        fprintf(stderr, "Error: No output file specified after %s\n", argv[i]);
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
        else if (strcmp(argv[i], "--output") == 0 || strcmp(argv[i], "-o") == 0)
        {
            i += handle_output_arg(&args, i, argc, argv);
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
    printf("  -i, --input <string|file>      Input string or file path to encode/decode\n");
    printf("  -o, --output <file>            Output file path (optional, prints to stdout if not specified)\n");
    printf("  -e, --encode <type>            Encode using specified type (base64)\n");
    printf("  -d, --decode <type>            Decode using specified type (base64)\n");
    printf("  -v, --verbose                  Enable verbose output\n");
    printf("  -h, --help                     Show this help message\n");
}