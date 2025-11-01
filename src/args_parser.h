#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include "cryptool.h"

typedef struct
{
  char *input;
  char *output_file;
  OperatingMode mode;
  EncodingType encoding;
  int verbose;
  int show_help;
  int error;
  int is_file_input;
} ParsedArgs;

ParsedArgs args_init(void);

ParsedArgs args_parse(int argc, char *argv[]);

#define string_to_encoding_type(str)    \
  (strcmp(str, "base64") == 0 ? BASE_64 \
   : strcmp(str, "hex") == 0  ? HEX     \
                              : -1)

#define is_valid_encoding_type(arg) \
  (strcmp(arg, "base64") == 0 || strcmp(arg, "hex") == 0)

void args_print_help(const char *program_name);

#endif // ARGS_PARSER_H