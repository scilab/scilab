#ifndef __CALLJUIGETFILE_H__
#define __CALLJUIGETFILE_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - DIGITEO - Sylvestre KOUMAR
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
