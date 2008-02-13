
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.messagebox;

import org.scilab.modules.gui.uielement.UIElement;

/**
 * Interface for Message Boxes used by Scilab functions such as x_message 
 * @author Vincent COUVERT
 */
public interface MessageBox extends UIElement {
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleMessageBox getAsSimpleMessageBox();
	
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
}
