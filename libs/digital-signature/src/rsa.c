#include <rsa.h>

/**
 * Operations are all defined over uints of 4096, even for smaller keysizes
 * The size restriction is done by checking the uint bit size + mod operations
 */
DEFINE_UINT(u4096, 64)
