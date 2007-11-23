/*--------------------------------------------------------------------------*/
/**
* @author Allan CORNET INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#ifndef __SETLOOKANDFEEL_H__
#define __SETLOOKANDFEEL_H__

#include "machine.h" /* BOOL */

/**
* set current look and feel
* "javax.swing.plaf.metal.MetalLookAndFeel"
* "com.sun.java.swing.plaf.motif.MotifLookAndFeel"
* "com.sun.java.swing.plaf.windows.WindowsLookAndFeel"
* "com.sun.java.swing.plaf.windows.WindowsClassicLookAndFeel"
* @param a string
* @return BOOL
*/
BOOL setlookandfeel(char *lookandfeekstr);

/*
* set system default look and feel
* @return BOOL
*/
BOOL setsystemlookandfeel(void);

#endif /* __SETLOOKANDFEEL_H__ */
/*--------------------------------------------------------------------------*/
