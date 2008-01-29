#ifndef __ADDINTER_H__
#define  __ADDINTER_H__

/** 
* the first dynamic interface is at position 500+1 
*/
#define DynInterfStart 500

/**
* Remove interface in scilab
* @param[in] id of interface
*/
void RemoveInterf(int id);

/**
* Add a interface in Scilab
* @param[in] filenamelib dynamic libraries filename
* @param[in] spname Name of interface routine entry point 
* @param[in] fcts Name of new Scilab function implemented in the new interface 
* @param[in] sizefcts size of fcts array
* @return a int id of interface (-1 failed)
*/
int AddInterfaceToScilab(char *filenamelib,char *spname,char **fcts,int sizefcts);

#endif /*  __ADDINTER_H__ */
