/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/

#ifndef __ISNOINTERACTIVEMODEWINDOW_H__
#define __ISNOINTERACTIVEMODEWINDOW_H__

/*
 * Used to know if we are in window mode (returned value is 1)
 * check if it is -NWNI mode (obsolete)
 * or in console mode (returned value is 0) 
 * @return 1 if we are in Windows mode & 0 if not
 */
int IsNoInteractiveWindow(void);

#endif /* __ISNOINTERACTIVEMODEWINDOW_H__ */
/*-----------------------------------------------------------------------------------*/
