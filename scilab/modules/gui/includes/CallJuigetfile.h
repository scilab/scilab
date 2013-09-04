#ifndef __CALLJUIGETFILE_H__
#define __CALLJUIGETFILE_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - DIGITEO - Sylvestre KOUMAR
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/** uigetfile **/
void CallJuigetfile(char **mask, char **description, int maskSize, char *initialDirectory, char *titleBox, bool multipleSelection);

void CallJuigetfileWithoutInput();

void CallJuigetfileOnlyWithMask(char **mask, char **description, int maskSize);

void CallJuigetfileWithMaskAndInitialdirectory(char **mask, char **description, int maskSize, char *initialDirectory);

void CallJuigetfileWithoutMultipleSelection(char **mask, char **description, int maskSize, char *initialDirectory, char * titleBox);

/** uiputfile **/
void CallJuiputfileWithoutInput();

void CallJuiputfileOnlyWithMask(char **mask, char **description, int maskSize);

void CallJuiputfileWithMaskAndInitialdirectory(char **mask, char **description, int maskSize, char *initialDirectory);

void CallJuiputfileWithoutMultipleSelection(char **mask, char **description, int maskSize, char *initialDirectory, char * titleBox);

/** generic functions **/
char **getJuigetfileSelection(void);

char **getJuigetfileSelectionFileNames(void);

char *getJuigetfileSelectionPathName(void);

int getJuigetfileSelectionSize(void);

bool getJuigetfileMultipleSelection(void);

int getJuigetfileFilterIndex(void);

char *getJuigetfileMenuCallback(void);

/** uigetdir **/
void CallJuigetfileForDirectoryWithoutInput();

void CallJuigetfileForDirectoryWithInitialdirectory(char *initialDirectory);

void CallJuigetfileForDirectoryWithInitialdirectoryAndTitle(char *initialDirectory, char *title);

#endif
