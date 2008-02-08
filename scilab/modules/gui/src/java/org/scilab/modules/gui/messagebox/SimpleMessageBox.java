
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.messagebox;

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
}
