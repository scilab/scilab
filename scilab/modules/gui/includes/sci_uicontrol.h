/*------------------------------------------------------------------------*/
/* file: sci_uicontrol.h                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                              */
/*------------------------------------------------------------------------*/

#ifndef _SCI_UICONTROL_H_
#define _SCI_UICONTROL_H_

/**
* interface function for the uicontrol routine.
*
* @param[in] fname     name of the routine (ie uicontrol)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_uicontrol(char * fname, unsigned long fname_len) ;

#endif /* _SCI_UICONTROL_H_ */
