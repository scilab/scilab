/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */package org.scilab.modules.gui.messagebox;

import org.scilab.modules.gui.tab.SimpleTab;

/**
* Interface for SimpleMessageBox the object associated to Scilab MessageBoxes
* @author Vincent COUVERT
*/
public interface SimpleMessageBox {


    /**
     * Set the element id for this MessageBox
     * @param id the id of the corresponding MessageBox object
     */
    void setElementId(int id);

    /**
     * Get the element id for this MessageBox
     * @return id the id of the corresponding MessageBox object
     */
    int getElementId();

    /**
     * Set the title of the MessageBox
     * @param title the title to set
     */
    void setTitle(String title);

    /**
     * Set the message of the MessageBox
     * @param message the message to set
     */
    void setMessage(String message);

    /**
     * Set the message of the MessageBox (multi-line)
     * @param message the message to set
     */
    void setMessage(String[] message);

    /**
     * Display this MessageBox and wait for user choice
     */
    void displayAndWait();

    /**
     * Get the index of the button clicked
     * @return the index of the button clicked
     */
    int getSelectedButton();

    /**
     * Set the indices of the default selected buttons (x_choices)
     * @param indices the indices of the default selected buttons
     */
    void setDefaultSelectedButtons(int[] indices);

    /**
     * Get the indices of the user selected buttons (x_choices)
     * @return the indices of the user selected buttons
     */
    int[] getUserSelectedButtons();

    /**
     * Set the labels of the buttons in the MessageBox
     * @param labels the labels of the buttons
     */
    void setButtonsLabels(String[] labels);

    /**
     * Set the initial values of the editable zone in the MessageBox
     * @param value the initial values
     */
    void setInitialValue(String[] value);

    /**
     * Get the value of the editable zone in the MessageBox
     * @return the value
     */
    String[] getValue();

    /**
     * Get the size of the value of the editable zone in the MessageBox
     * @return the value size
     */
    int getValueSize();

    /**
     * Set the items of the listbox in the MessageBox
     * @param items the items to set
     */
    void setListBoxItems(String[] items);

    /**
     * Get the index of the selected item in the listbox in the MessageBox
     * @return the index
     */
    int getSelectedItem();

    /**
     * Set the name of the lines labels in the editable zone in the MessageBox
     * @param labels the labels
     */
    void setLineLabels(String[] labels);

    /**
     * Set the name of the columns labels in the editable zone in the MessageBox
     * @param labels the labels
     */
    void setColumnLabels(String[] labels);

    /**
     * Set the default values of a multi-value editable zone in the MessageBox
     * @param values the values
     */
    void setDefaultInput(String[] values);

    /**
     * Set a MessageBox modal or not
     * @param status true to set the MessageBox modal and false else
     */
    void setModal(boolean status);

    /**
     * Set the MessageBox icon
     * @param name the name of the icon
     */
    void setIcon(String name);

    /**
     * Set the component used to set the location of the MessageBox (default is Scilab Console)
     * @param parent tab to be set as parent
     */
    void setParentForLocation(SimpleTab parent);
}
