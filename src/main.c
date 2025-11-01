#include <stdio.h>
#include <stdlib.h>
#include "cryptool.h"
#include "args_parser.h"
#include "encoder.h"
#include "encodings/registry.h"

int main(int argc, char *argv[])
{
    registry_init();

    ParsedArgs args = args_parse(argc, argv);

    if (args.show_help)
    {
        args_print_help(argv[0]);
        return 0;
    }

    if (args.error)
    {
        fprintf(stderr, "\nUse --help for usage information.\n");
        return 1;
    }

    if (!args.input)
    {
        fprintf(stderr, "Error: No input string provided.\n");
        args_print_help(argv[0]);
        return 1;
    }

    char *result = encoder_process(&args);

    if (!result)
    {
        fprintf(stderr, "\n[ERROR] Operation failed!\n");
        if (args.is_file_input && args.input)
        {
            free(args.input);
        }
        return 1;
    }

    if (args.output_file)
    {
        FILE *output = fopen(args.output_file, "w");
        if (!output)
        {
            fprintf(stderr, "Error: Cannot open output file '%s' for writing\n", args.output_file);
            free(result);
            if (args.is_file_input && args.input)
            {
                free(args.input);
            }
            return 1;
        }

        fprintf(output, "%s\n", result);
        fclose(output);

        printf("Output written to: %s\n", args.output_file);
    }
    else
    {
        printf("%s\n", result);
    }

    free(result);

    if (args.is_file_input && args.input)
    {
        free(args.input);
    }

    return 0;
}