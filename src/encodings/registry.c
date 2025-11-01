#include <stdio.h>
#include <stdlib.h>
#include "registry.h"
#include "base64.h"
#include "hex.h"

static Encoding encodings[3];
static int initialized = 0;

void registry_init(void)
{
    if (initialized)
    {
        return;
    }

    encodings[0] = (Encoding){
        .name = "BASE64",
        .description = "Base64 encoding",
        .encode = base64_encode,
        .decode = base64_decode};
    encodings[1] = (Encoding){
        .name = "HEX",
        .description = "Hexadecimal encoding",
        .encode = hex_encode,
        .decode = hex_decode};

    initialized = 1;
}

const Encoding *registry_get_encoding(EncodingType type)
{
    if (!initialized)
    {
        registry_init();
    }

    switch (type)
    {
    case BASE_64:
        return &encodings[0];
    case HEX:
        return &encodings[1];
    default:
        return NULL;
    }
}

void registry_list_encodings(void)
{
    if (!initialized)
    {
        registry_init();
    }

    for (int i = 0; i < 3; i++)
    {
        printf("  %-10s - %s\n", encodings[i].name, encodings[i].description);
    }
}