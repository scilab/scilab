/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.messagebox;

import org.scilab.modules.gui.bridge.messagebox.SwingScilabMessageBox;
import org.scilab.modules.gui.tab.SimpleTab;

/**
 * Bridge dedicated to Scilab Message Boxes
 * @author Vincent COUVERT
 */
public class ScilabMessageBoxBridge {

    /**
     * Constructor
     */
    protected ScilabMessageBoxBridge() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Creates a new MessageBox
     * @return the created MessageBox
     */
    public static SimpleMessageBox createMessageBox() {
        return new SwingScilabMessageBox();
    }

    /**
     * Set the title of the MessageBox
     * @param messageBox the MessageBox we want to set the title of
     * @param title the title to set
     */
    public static void setTitle(MessageBox messageBox, String title) {
        messageBox.getAsSimpleMessageBox().setTitle(title);
    }

    /**
     * Set the message of the MessageBox
     * @param messageBox the MessageBox we want to set the message of
     * @param message the message to set
     */
    public static void setMessage(MessageBox messageBox, String message) {
        messageBox.getAsSimpleMessageBox().setMessage(message);
    }

    /**
     * Set the message of the MessageBox (multi-line)
     * @param messageBox the MessageBox we want to set the message of
     * @param message the message to set
     */
    public static void setMessage(MessageBox messageBox, String[] message) {
        messageBox.getAsSimpleMessageBox().setMessage(message);
    }

    /**
     * Display this MessgeBox and wait for user selection
     * @param messageBox the MessageBox we want to display
     */
    public static void displayAndWait(MessageBox messageBox) {
        messageBox.getAsSimpleMessageBox().displayAndWait();
    }


    /**
     * Get the index of the button clicked
     * @param messageBox the MessageBox we want to get the answer of
     * @return the index of the button clicked
     */
    public static int getSelectedButton(MessageBox messageBox) {
        return messageBox.getAsSimpleMessageBox().getSelectedButton();
    }

    /**
     * Set the indices of the default selected buttons (x_choices)
     * @param messageBox the MessageBox we want to set the defaults of
     * @param indices the indices of the default selected buttons
     */
    public static void setDefaultSelectedButtons(MessageBox messageBox, int[] indices) {
        messageBox.getAsSimpleMessageBox().setDefaultSelectedButtons(indices);
    }

    /**
     * Get the indices of the user selected buttons (x_choices)
     * @param messageBox the MessageBox we want to get the indices of
     * @return the indices of the default selected buttons
     */
    public static int[] getUserSelectedButtons(MessageBox messageBox) {
        return messageBox.getAsSimpleMessageBox().getUserSelectedButtons();
    }

    /**
     * Set the labels of the buttons in the MessageBox
     * @param messageBox the MessageBox we want to set the buttons labels of
     * @param labels the labels of the buttons
     */
    public static void setButtonsLabels(MessageBox messageBox, String[] labels) {
        messageBox.getAsSimpleMessageBox().setButtonsLabels(labels);
    }

    /**
     * Set the initial values of the editable zone in the MessageBox
     * @param messageBox the MessageBox we want to set the initial value of
     * @param value the initial values
     */
    public static void setInitialValue(MessageBox messageBox, String[] value) {
        messageBox.getAsSimpleMessageBox().setInitialValue(value);
    }

    /**
     * Set the value of the editable zone in the MessageBox
     * @param messageBox the MessageBox we want to get the value of
     * @return the value
     */
    public static String[] getValue(MessageBox messageBox) {
        return messageBox.getAsSimpleMessageBox().getValue();
    }

    /**
     * Set the value size of the editable zone in the MessageBox
     * @param messageBox the MessageBox we want to get the value of
     * @return the value size
     */
    public static int getValueSize(MessageBox messageBox) {
        return messageBox.getAsSimpleMessageBox().getValueSize();
    }

    /**
     * Set the items of the listbox in the MessageBox
     * @param messageBox the MessageBox we want to set the items of
     * @param items the items to set
     */
    public static void setListBoxItems(MessageBox messageBox, String[] items) {
        messageBox.getAsSimpleMessageBox().setListBoxItems(items);
    }

    /**
     * Get the index of the selected item in the listbox in the MessageBox
     * @param messageBox the MessageBox we want to get the selected item of
     * @return the index
     */
    public static int getSelectedItem(MessageBox messageBox) {
        return messageBox.getAsSimpleMessageBox().getSelectedItem();
    }

    /**
     * Set the name of the lines labels in the editable zone in the MessageBox
     * @param messageBox the MessageBox we want to set the labels
     * @param labels the labels
     */
    public static void setLineLabels(MessageBox messageBox, String[] labels) {
        messageBox.getAsSimpleMessageBox().setLineLabels(labels);
    }

    /**
     * Set the name of the columns labels in the editable zone in the MessageBox
     * @param messageBox the MessageBox we want to set the labels
     * @param labels the labels
     */
    public static void setColumnLabels(MessageBox messageBox, String[] labels) {
        messageBox.getAsSimpleMessageBox().setColumnLabels(labels);
    }

    /**
     * Set the default values of a multi-value editable zone in the MessageBox
     * @param messageBox the MessageBox we want to set the default values
     * @param values the values
     */
    public static void setDefaultInput(MessageBox messageBox, String[] values) {
        messageBox.getAsSimpleMessageBox().setDefaultInput(values);
    }

    /**
     * Set a MessageBox modal or not
     * @param messageBox the MessageBox we want to set the modality of
     * @param status true to set the MessageBox modal and false else
     */
    public static void setModal(MessageBox messageBox, boolean status) {
        messageBox.getAsSimpleMessageBox().setModal(status);
    }

    /**
     * Set the MessageBox icon
     * @param messageBox the MessageBox we want to set the icon of
     * @param name the name of the icon
     */
    public static void setIcon(MessageBox messageBox, String name) {
        messageBox.getAsSimpleMessageBox().setIcon(name);
    }

    /**
     * Set the component used to set the location of the MessageBox (default is Scilab Console)
     * @param messageBox the MessageBox we want to set the parent of
     * @param parent the tab to be set as parent
     */
    public static void setParentForLocation(MessageBox messageBox, SimpleTab parent) {
        //messageBox.getAsSimpleMessageBox().setParentForLocation(parent);
    }

}
