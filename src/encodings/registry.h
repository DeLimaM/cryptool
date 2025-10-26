#ifndef REGISTRY_H
#define REGISTRY_H

#include "../cryptool.h"
#include "encoding_interface.h"

void registry_init(void);

const Encoding *registry_get_encoding(EncodingType type);

void registry_list_encodings(void);

#endif // REGISTRY_H