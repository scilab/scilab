#ifndef __ADDINTER_H__
#define  __ADDINTER_H__

/** 
* the first dynamic interface is at position 500+1 
*/
#define DynInterfStart 500

/**
*
*/
void RemoveInterf(int Nshared);

/**
*
*/
int AddInterfaceToScilab(char *filenamelib,char *spname,char **fcts,int sizefcts);

#endif /*  __ADDINTER_H__ */
