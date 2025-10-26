#ifndef ENCODING_INTERFACE_H
#define ENCODING_INTERFACE_H

typedef char *(*encode_func)(const char *input);
typedef char *(*decode_func)(const char *input);

typedef struct {
  const char *name;
  const char *description;
  encode_func encode;
  decode_func decode;
} Encoding;

#endif // ENCODING_INTERFACE_H