
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.messagebox;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.uielement.ScilabUIElement;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * Interface for Message Boxes used by Scilab functions such as x_message 
 * @author Vincent COUVERT
 */
public class ScilabMessageBox extends ScilabUIElement implements MessageBox {

	private SimpleMessageBox component;
	
	//private int chosenButton;
	
	/**
	 * Constructor
	 */
	protected ScilabMessageBox() {
		component = ScilabBridge.createMessageBox();
		component.setElementId(UIElementMapper.add(this));
	}

	/**
	 * Creates a Scilab MessageBox
	 * @return the created MessageBox
	 */
	public static MessageBox createMessageBox() {
		return new ScilabMessageBox();
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleMessageBox getAsSimpleMessageBox() {
		return component;
	}

	/**
	 * Set the title of the file chooser
	 * @param title the title to set
	 */
	public void setTitle(String title) {
		ScilabBridge.setTitle(this, title);
	}

	/**
	 * Set the message of the MessageBox
	 * @param message the message to set
	 */
	public void setMessage(String message) {
		ScilabBridge.setMessage(this, message);
	}

	/**
	 * Set the message of the MessageBox (multi-line)
	 * @param message the message to set
	 */
	public void setMessage(String[] message) {
		ScilabBridge.setMessage(this, message);
	}

	/**
	 * Get the index of the button clicked
	 * @return the index of the button clicked
	 */
	public int getSelectedButton() {
		return ScilabBridge.getSelectedButton(this);
		
	}

	/**
	 * Set the labels of the buttons in the MessageBox
	 * @param labels the labels of the buttons
	 */
	public void setButtonsLabels(String[] labels) {
		ScilabBridge.setButtonsLabels(this, labels);
	}
	
	/**
	 * Display this MessageBox and wait for user selection 
	 */
	public void displayAndWait() {
		ScilabBridge.displayAndWait(this);
	}
	
	/**
	 * Set the initial values of the editable zone in the MessageBox
	 * @param value the initial values
	 */
	public void setInitialValue(String[] value) {
		ScilabBridge.setInitialValue(this, value);
	}

	/**
	 * Get the value of the editable zone in the MessageBox
	 * @return the value
	 */
	public String[] getValue() {
		return ScilabBridge.getValue(this);
	}

	/**
	 * Get the size of the value of the editable zone in the MessageBox
	 * @return the value size
	 */
	public int getValueSize() {
		return ScilabBridge.getValueSize(this);
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#getDims()
	 * @return nothing
	 */
	public Size getDims() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @return nothing
	 */
	public Position getPosition() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @return nothing
	 */
	public boolean isVisible() {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newSize is never used
	 */
	public void setDims(Size newSize) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newPosition is never used
	 */
	public void setPosition(Position newPosition) {
		throw new UnsupportedOperationException();
	}

	/**
	 * Should not be used, just here to implemeent org.scilab.modules.gui.UIElement#draw()
	 * @param newVisibleState is never used
	 */
	public void setVisible(boolean newVisibleState) {
		throw new UnsupportedOperationException();
	}

}
