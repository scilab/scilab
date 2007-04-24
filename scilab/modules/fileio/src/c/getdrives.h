/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __GETDRIVES_H__
#define __GETDRIVES_H__

/**
* Get the drive letters of all mounted filesystems on the computer.
* @param[out] nbDrives
* @return List of Drives
*/
char **getdrives(int *nbDrives);

#endif /* __GETDRIVES_H__ */
