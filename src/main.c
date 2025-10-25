#include <stdio.h>
#include "cryptool.h"
#include "args_parser.h"

int main(int argc, char *argv[])
{
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

    if (args.verbose)
    {
        printf("Input string: %s\n", args.input);
        printf("Operating mode: %s\n", args.mode == MODE_ENCRYPT ? "Encrypt" : "Decrypt");
        printf("Encoding type: %s\n",
               args.encoding == BASE_64 ? "Base64" : args.encoding == HEX ? "Hex"
                                                                          : "UU");
    }

    return 0;
}