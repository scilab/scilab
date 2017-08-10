#ifndef __CALLMESSAGEBOX_H__
#define __CALLMESSAGEBOX_H__

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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

#include "BOOL.h"

/**
 * Create a new MessageBox
 *
 * @param void
 * @return the ID of the created MessageBox
 */
int createMessageBox(void);

/**
 * Set the title of the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @param title the title to set
 */
void setMessageBoxTitle(int ID, char * title);

/**
 * Set the message of the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @param message the message to set
 */
void setMessageBoxMessage(int ID, char * message);

/**
 * Set the message of the MessageBox (multi-line)
 *
 * @param ID the ID of the MessageBox
 * @param message the message to set
 * @param nbLines number of lines in message
 */
void setMessageBoxMultiLineMessage(int ID, char **message, int nbLines);

/**
 * Display the MessageBox and Wait for a user action
 *
 * @param ID the ID of the MessageBox
 */
void messageBoxDisplayAndWait(int ID);

/**
 * Get the index of the selected button
 *
 * @param ID the ID of the MessageBox
 * @return the index of the selected button
 */
int getMessageBoxSelectedButton(int ID);

/**
 * Set the indices of the default selected buttons (x_choices)
 *
 * @param ID the ID of the MessageBox
 * @param indices the indices of the default selected buttons
 * @param nbIndices the number of indices
 */
void setMessageBoxDefaultSelectedButtons(int ID, int* indices, int nbIndices);

/**
 * Get the indices of the user selected buttons (x_choices)
 *
 * @param ID the ID of the MessageBox
 * @return the indices of the user selected buttons
 */
int* getMessageBoxUserSelectedButtons(int ID);

/**
 * Set the labels of the buttons
 *
 * @param ID the ID of the MessageBox
 * @param labels the labels of the buttons
 * @param nbLabels the number of labels in labels parameter
 */
void setMessageBoxButtonsLabels(int ID, char** labels, int nbLabels);

/**
 * Set the initial value of the editable text zone of the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @param value the value
 * @param nbLines the number of lines in value parameter
 */
void setMessageBoxInitialValue(int ID, char** value, int nbLines);

/**
 * Get the value of the editable text zone of the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @return the value
 */
char ** getMessageBoxValue(int ID);

/**
 * Get the value size of the editable text zone of the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @return the value size
 */
int getMessageBoxValueSize(int ID);

/**
 * Set the items of the listbox of the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @param items the items to set
 * @param nbItems the number of items
 */
void setMessageBoxListBoxItems(int ID, char** items, int nbItems);

/**
 * Get the selected item in the listbox of the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @return the selected item index (0 if canceled)
 */
int getMessageBoxSelectedItem(int ID);

/**
 * Set the name of the lines labels in the editable zone in the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @param labels the labels
 * @param nbItems the number of labels
 */
void setMessageBoxLineLabels(int ID, char** labels, int nbLabels);

/**
 * Set the name of the columns labels in the editable zone in the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @param labels the labels
 * @param nbItems the number of labels
 */
void setMessageBoxColumnLabels(int ID, char** labels, int nbLabels);

/**
 * Set the default values of a multi-value editable zone in the MessageBox
 *
 * @param ID the ID of the MessageBox
 * @param values the values
 * @param nbItems the number of values
 */
void setMessageBoxDefaultInput(int ID, char** values, int nbValues);

/**
 * Set a MessageBox modal or not
 *
 * @param ID the ID of the MessageBox
 * @param status TRUE to set the MessageBox modal and FALSE else
 */
void setMessageBoxModal(int ID, BOOL status);

/**
 * Set the MessageBox icon
 *
 * @param ID the ID of the MessageBox
 * @param name the name of the icon
 */
void setMessageBoxIcon(int ID, char *name);

#endif /* !__CALLMESSAGEBOX_H__ */

