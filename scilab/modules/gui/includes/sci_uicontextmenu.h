/*------------------------------------------------------------------------*/
/* file: sci_uicontextmenu.h                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Vincent COUVERT                                              */
/*------------------------------------------------------------------------*/

#ifndef __SCI_UICONTEXTMENU_H__
#define __SCI_UICONTEXTMENU_H__

/**
 * Interface function for the uicontextmenu routine.
 *
 * @param[in] fname     name of the routine (ie uicontextmenu)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
 */
int sci_uicontextmenu(char * fname, unsigned long fname_len) ;

#endif /* !__SCI_UICONTEXTMENU_H__ */
