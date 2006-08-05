#ifndef __LINK_H__
#define __LINK_H__

/* Copyright INRIA */
extern void GetDynFunc __PARAMS(( int ii, void (**realop)()));
extern int SearchInDynLinks __PARAMS((char *op, void (**realop)()));
extern void SciLinkInit __PARAMS((void));
extern void  ShowDynLinks __PARAMS((void));
extern void RemoveInterf __PARAMS((int));
extern void SciLink __PARAMS((int iflag,int *rhs,int *ilib,char *files[],
			     char *en_names[],char *strf));

extern void C2F(iislink) __PARAMS((   char *buf,   integer *irep));

#endif /*  __LINK_H__ */

