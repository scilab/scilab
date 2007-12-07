#ifndef __CALLFILECHOOSER_H__
#define __CALLFILECHOOSER_H__

/* Copyright INRIA */
/* Vincent COUVERT 2007 */

int createFileChooser();

void setFileChooserTitle(int ID, char * title);

void setFileChooserMask(int ID, char * mask);

void setFileChooserInitialDirectory(int ID, char * path);

void fileChooserDisplayAndWait(int ID);

char** getFileChooserSelection(int ID);

int getFileChooserSelectionSize(int ID);
#endif

