#ifndef FMU_SIM_UTILITY_H
#define FMU_SIM_UTILITY_H

#include "scicos_block4.h"

#if 0
#define FMU_SIM_PRINT(...) sciprint(__VA_ARGS__)
#else
#define FMU_SIM_PRINT(...) do { } while (0)
#endif

int fmiSet_parameters(scicos_block *, scicos_flag);
int fmiSet_values(scicos_block *, scicos_flag, int, int *, int *, size_t, uint32_t *, void** from);
int fmiGet_values(scicos_block *, int *, int*, size_t, uint32_t *, void** into);
void free_instance(scicos_block *, int, const char *);

#endif /* FMU_SIM_UTILITY_H */