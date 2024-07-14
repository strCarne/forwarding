#ifndef FORWARDING_MACROS_H
#define FORWARDING_MACROS_H

#include <stdio.h>

#define CLR_DEF "\x1B[0m"
#define CLR_RED "\x1B[31m"
#define CLR_GRN "\x1B[32m"
#define CLR_YEL "\x1B[33m"
#define CLR_BLU "\x1B[34m"
#define CLR_MAG "\x1B[35m"
#define CLR_CYN "\x1B[36m"
#define CLR_WHT "\x1B[37m"

#define CLR_WRAP(clr, text) clr text CLR_DEF

#define INFO_TYPE CLR_BLU "INFO" CLR_DEF
#define DEBUG_TYPE CLR_YEL "DEBUG" CLR_DEF
#define ERROR_TYPE CLR_RED "ERROR" CLR_DEF

#define INFO_FILE stdout
#define DEBUG_FILE stdout
#define ERROR_FILE stderr

#define LOG(output_file, type, tag, ...)                                       \
    fprintf(output_file, "[%s]@%s => ", type, tag);                            \
    fprintf(output_file, __VA_ARGS__);                                         \
    fflush(output_file)

#define LOGI(tag, ...)                                                         \
    LOG(INFO_FILE, INFO_TYPE, CLR_WRAP(CLR_MAG, tag), __VA_ARGS__)

#define LOGD(tag, ...)                                                         \
    LOG(DEBUG_FILE, DEBUG_TYPE, CLR_WRAP(CLR_MAG, tag), __VA_ARGS__)

#define LOGE(tag, ...)                                                         \
    LOG(ERROR_FILE, ERROR_TYPE, CLR_WRAP(CLR_MAG, tag), __VA_ARGS__)

#define IS_EVEN(number) (((number) & 1) == 0)
#define IS_ODD(number) (((number) & 1) == 1)

#endif // !FORWARDING_MACROS_H
