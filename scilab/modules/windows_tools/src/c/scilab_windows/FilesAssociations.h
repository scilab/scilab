/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __FILEASSOCIATION_H__
#define __FILEASSOCIATION_H__

#include "machine.h" /* BOOL */
/*-----------------------------------------------------------------------------------*/
/**
* check if filename exists (windows only)
* @param[in] a filename
* @return TRUE or FALSE
*/
BOOL IsAFile(char *chainefichier);

/**
* check if it is a .bin or .sav
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsABinOrSavFile(char *chainefichier);

/**
* check if it is a .graph or .graphb
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAGraphFile(char *chainefichier);

/**
* check if it is a .graph
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAGraphFilegraph(char *chainefichier);

/**
* check if it is a .graphb
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAGraphFilegraphb(char *chainefichier);

/**
* check if it is a scicos file
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAScicosFile(char *chainefichier);

/**
* check if it is a .cos
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAScicosFileCOS(char *chainefichier);

/**
* check if it is a .cosf
* @param[in]
* @return TRUE or FALSE
*/
BOOL IsAScicosFileCOSF(char *chainefichier);

/**
* get command to do by file extension
* @param[in]
* @param[in]
* @param[out]
* @return TRUE or FALSE
*/
int CommandByFileExtension(char *fichier,int OpenCode,char *Cmd);

/**
* convert (lower cases) extension
* @param[in]
* @return TRUE or FALSE
*/
void ExtensionFileIntoLowerCase(char *fichier);

#endif /*  __FILEASSOCIATION_H__ */
/*-----------------------------------------------------------------------------------*/
