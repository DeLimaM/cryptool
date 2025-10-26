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
        return 1;
    }

    if (!args.verbose)
    {
        printf("%s\n", result);
    }

    free(result);

    return 0;
}