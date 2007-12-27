#ifndef __CALLFILECHOOSER_H__
#define __CALLFILECHOOSER_H__

/* Copyright INRIA */
/* Vincent COUVERT 2007 */

/**
 * @TODO add comment
 *
 * @param void  
 * @return <ReturnValue>
 */
int createFileChooser(void);

/**
 * @TODO add comment
 *
 * @param ID    
 * @param title 
 */
void setFileChooserTitle(int ID, char * title);

/**
 * @TODO add comment
 *
 * @param ID    
 * @param mask  
 */
void setFileChooserMask(int ID, char * mask);

/**
 * @TODO add comment
 *
 * @param ID    
 * @param path  
 */
void setFileChooserInitialDirectory(int ID, char * path);

/**
 * @TODO add comment
 *
 * @param ID    
 */
void fileChooserDisplayAndWait(int ID);

/**
 * @TODO add comment
 *
 * @param ID    
 * @return <ReturnValue>
 */
char** getFileChooserSelection(int ID);

/**
 * @TODO add comment
 *
 * @param ID    
 * @return <ReturnValue>
 */
int getFileChooserSelectionSize(int ID);

/**
 * @TODO add comment
 *
 * @param ID    
 */
void setFileChooserDirectorySelectionOnly(int ID);

/**
 * @TODO add comment
 *
 * @param ID    
 */
void setFileChooserFileSelectionOnly(int ID);

#endif

