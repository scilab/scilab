
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.messagebox;

import org.scilab.modules.gui.bridge.messagebox.SwingScilabMessageBox;

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

}
