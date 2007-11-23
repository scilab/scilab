/*--------------------------------------------------------------------------*/
/* INRIA */
/*--------------------------------------------------------------------------*/
#ifndef SCI_VERSION_H 
#define SCI_VERSION_H 

#define SCI_VERSION_MAJOR 5
#define SCI_VERSION_MINOR 0
#define SCI_VERSION_MAINTENANCE 0
#define SCI_VERSION_STRING "scilab-trunk-SVN"
#define SCI_VERSION_REVISION 0

void disp_scilab_version(void);

/* for compatibility */
/* Deprecated */
#define SCI_VERSION SCI_VERSION_STRING
#define DEFAULT_SCI_VERSION_MESSAGE "scilab-trunk-SVN (INRIA,ENPC)"


#endif 
/*--------------------------------------------------------------------------*/
