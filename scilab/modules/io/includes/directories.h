/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __CREATEDIR_H__
#define __CREATEDIR_H__
/*-----------------------------------------------------------------------------------*/ 
#include "machine.h"
#include "string.h"
/*-----------------------------------------------------------------------------------*/ 

/**
 * Create a directory
 *
 * @param path the path of the futur directory
 * @return the result of the operation
 */
BOOL CreateDir(const char *path);

/**
 * Check if a directory exists or not
 *
 * @param path the directory
 * @return if the directory exists (true) or not (false)
 */
BOOL isdir(const char *path);

/**
 * Remove a directory
 *
 * @param path the directory
 * @return the result of the operation
 */
BOOL RemoveDir(char *path);
/*-----------------------------------------------------------------------------------*/ 

#define MAX_PATH_LONG 32767
#define MAX_FILENAME_LEN 512

#endif /* __CREATEDIR_H__ */
/*-----------------------------------------------------------------------------------*/ 
