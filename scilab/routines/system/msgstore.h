#ifndef MSGSTORE_ 
#define  MSGSTORE_ 

extern void  C2F(freemsgtable) __PARAMS((void));
extern int  C2F(errstore) __PARAMS((int *));
extern int  C2F(msgstore) __PARAMS((char *,int *));
extern int C2F(linestore) __PARAMS((int *));
extern int C2F(funnamestore) __PARAMS((char *,int *));
#endif 
