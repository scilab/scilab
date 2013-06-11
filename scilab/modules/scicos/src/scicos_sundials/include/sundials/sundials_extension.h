/*
 * -----------------------------------------------------------------
 * Allan CORNET - 2009
 * For details, see sundials/shared/LICENSE.
 * -----------------------------------------------------------------
 */
#ifndef __SUNDIALS_EXTENSION_H__
#define __SUNDIALS_EXTENSION_H__

#include "sundials/sundials_config.h" /* SUNDIALS_EXPORT definition */

#define BOOL int
#define TRUE 1
#define FALSE 0

/**
* checks if sundials extension is enabled
* @return TRUE or FALSE
*/
SUNDIALS_EXPORT BOOL is_sundials_with_extension(void);

/**
* set extension mode for sundials
* @param[in] _mode TRUE is enabled , FALSE use default mode of sundials
* @return new mode
*/
SUNDIALS_EXPORT BOOL set_sundials_with_extension(BOOL _mode);

#endif /* __SUNDIALS_EXTENSION_H__ */
