typedef char *ht_key_t;
typedef int ht_value_t;
#define HT(x) ht_ ## x
#include "ht1.h"
#undef HT
