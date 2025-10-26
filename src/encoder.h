#ifndef ENCODER_H
#define ENCODER_H

#include "args_parser.h"
#include "cryptool.h"

char *encoder_process(const ParsedArgs *args);

char *encode_string(const char *input, EncodingType encoding);

char *decode_string(const char *input, EncodingType encoding);

#endif // ENCODER_H