
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
	
	private static final String SEMI_COLON = ";";
	
	private int elementId;
	
	private int selectedButton; 
	
	private String[] buttonsLabels;
	
	/**
	 * Used for x_dialog
	 */
	private String[] initialValue;
	private String userValue;
	
	/**
	 * Used for x_choose
	 */
	private String[] listboxItems;
	private int selectedItem;
	
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
	 * Set the message of the MessageBox (multi-line)
	 * @param message the message to set
	 */
	public void setMessage(String[] message) {
		super.setMessage(message);
	}

	/**
	 * Display this MessageBox and wait for user selection 
	 */
	public void displayAndWait() {

		if (buttonsLabels == null && initialValue == null && listboxItems == null) {
			selectedButton = showConfirmDialog(this, getMessage(), getFrameForComponent(this).getTitle(), JOptionPane.PLAIN_MESSAGE);
			selectedButton++; // Because Scilab indices begin at 1 and not 0
		} else if (buttonsLabels != null && listboxItems == null) {
			selectedButton = showOptionDialog(this, getMessage(), getFrameForComponent(this).getTitle(),
					JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE, null, buttonsLabels, buttonsLabels[0]);
			selectedButton++; // Because Scilab indices begin at 1 and not 0
		} else if (listboxItems != null) {
			String selection = (String) showInputDialog(this,
					getMessage(),
                    getFrameForComponent(this).getTitle(),
                    JOptionPane.PLAIN_MESSAGE,
                    null,
                    listboxItems,
                    listboxItems[0]);
			
			/* Compute index to be returned */
			selectedItem = 0;
			for (int k = 0; k < listboxItems.length; k++) {
				if (listboxItems[k] == selection) {
					selectedItem = k + 1;
					break;
				}
			}
		} else {
			// Convert to a single line string
			String initialValueAsString = "";
			int k = 0;
			for (k = 0; k < initialValue.length - 1; k++) {
				initialValueAsString +=  initialValue[k];
				initialValueAsString +=  SEMI_COLON;
			}
			initialValueAsString += initialValue[k];
			userValue = showInputDialog(this, getMessage(), getFrameForComponent(this).getTitle(), JOptionPane.QUESTION_MESSAGE,
						null, null, initialValueAsString).toString();
		}
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
	
	/**
	 * Set the initial values of the editable zone in the MessageBox
	 * @param values the initial values
	 */
	public void setInitialValue(String[] values) {
		initialValue = values;
	}

	/**
	 * Set the value of the editable zone in the MessageBox
	 * @return the value
	 */
	public String[] getValue() {
		return userValue.split(SEMI_COLON);
	}
	
	/**
	 * Get the size of the value of the editable zone in the MessageBox
	 * @return the value size
	 */
	public int getValueSize() {
		if (userValue == null) {
			return 0;
		} else {
			return userValue.split(SEMI_COLON).length;
		}
	}

	/**
	 * Set the items of the listbox in the MessageBox
	 * @param items the items to set
	 */
	public void setListBoxItems(String[] items) {
		listboxItems = items;
	}
	
	/**
	 * Get the index of the selected item in the listbox in the MessageBox
	 * @return the index
	 */
	public int getSelectedItem() {
		return selectedItem;
	}

}
