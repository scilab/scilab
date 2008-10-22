#ifndef __CALLFILECHOOSER_H__
#define __CALLFILECHOOSER_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

