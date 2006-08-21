/*------------------------------------------------------------------------*/
/* file: sci_driver.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for driver routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_DRIVER_H_
#define _INT_DRIVER_H_

/**
* interface function for the driver routine :
* driver(driver_name) or  current_driver=driver()
*
* @param[in] fname      name of the routine (ie driver)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_driver( char * fname, unsigned long fname_len ) ;

#endif /* _INT_DRIVER_H_ */
