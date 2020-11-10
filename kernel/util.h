#pragma once

#include "../cpu/types.h"

void memory_copy(u8 *source, u8 *dest, u32 nbytes);

char *int_to_string(int v, char *buff, int radix_base);
