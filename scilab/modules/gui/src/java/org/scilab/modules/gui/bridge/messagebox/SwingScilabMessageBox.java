
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.messagebox;

import javax.swing.JOptionPane;

import org.scilab.modules.gui.messagebox.SimpleMessageBox;

/**
 * Swing implementation of a Scilab MessageBox
 * @author Vincent COUVERT
 */
public class SwingScilabMessageBox extends JOptionPane implements SimpleMessageBox {

	private static final long serialVersionUID = 7939976395338222763L;
	
	private int elementId;
	
	private int selectedButton; 
	
	private String[] buttonsLabels;
	
	/**
	 * Default constructor
	 */
	public SwingScilabMessageBox() {
		super();
	}

	/**
	 * Set the element id for this MessageBox
	 * @param id the id of the corresponding MessageBox object
	 */
	public void setElementId(int id) {
		this.elementId = id;
	}
	
	/**
	 * Get the element id for MessageBox
	 * @return id the id of the corresponding MessageBox object
	 */
	public int getElementId() {
		return this.elementId;
	}
	
	/**
	 * Set the title of the MessageBox
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		getFrameForComponent(this).setTitle(title);
	}
	
	/**
	 * Set the message of the MessageBox
	 * @param message the message to set
	 */
	public void setMessage(String message) {
		super.setMessage(message);
	}

	/**
	 * Display this MessageBox and wait for user selection 
	 */
	public void displayAndWait() {
		if (buttonsLabels == null) {
		selectedButton = showConfirmDialog(this, getMessage(), getFrameForComponent(this).getTitle(), JOptionPane.PLAIN_MESSAGE);
		} else {
			selectedButton = showOptionDialog(this, getMessage(), getFrameForComponent(this).getTitle(),
					JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, buttonsLabels, buttonsLabels[0]);
		}
		selectedButton++; // Because Scilab indices begin at 1 and not 0
	}
	
	/**
	 * Get the index of the button clicked
	 * @return the index of the button clicked
	 */
	public int getSelectedButton() {
		return selectedButton;
	}
	
	/**
	 * Set the labels of the buttons in the MessageBox
	 * @param labels the labels of the buttons
	 */
	public void setButtonsLabels(String[] labels) {
		buttonsLabels = labels;
	}

}
