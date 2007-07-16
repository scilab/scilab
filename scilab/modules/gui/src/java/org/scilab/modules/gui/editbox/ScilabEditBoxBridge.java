
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.editbox;

import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab EditBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabEditBoxBridge {
	
	/**
	 * Constructor
	 */
	protected ScilabEditBoxBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab EditBox
	 * @return the created EditBox
	 */
	public static EditBoxBridge createEditBox() {
		return new SwingScilabEditBox();
	}
	
	/**
	 * Sets the Text of a Scilab EditBox
	 * @param editBox the EditBox we want to set the Text of
	 * @param newText the Text we want to set to the EditBox
	 */
	public static void setText(EditBoxBridge editBox, String newText) {
		editBox.setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab EditBox
	 * @param editBox the EditBox we want to get the Text of
	 * @return the text of the EditBox
	 */
	public static String getText(EditBoxBridge editBox) {
		return editBox.getText();
	}
	
	/**
	 * Draws a Scilab editBox
	 * @param editBox the editBox to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(EditBoxBridge editBox) {
		editBox.draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab EditBox
	 * @param editBox the editBox we want to get the dimensions of
	 * @return the size of the editBox
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(EditBoxBridge editBox) {
		return editBox.getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab editBox
	 * @param editBox the editBox we want to get the position of
	 * @return the position of the editBox
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(EditBoxBridge editBox) {
		return editBox.getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab EditBox
	 * @param editBox the editBox we want to get the visiblity status of
	 * @return the visibility status of the editBox (true if the editBox is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(EditBoxBridge editBox) {
		return editBox.isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab EditBox
	 * @param editBox the editBox we want to set the dimensions of
	 * @param newSize the size we want to set to the editBox
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(EditBoxBridge editBox, Size newSize) {
		editBox.setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab editBox
	 * @param editBox the editBox we want to set the position of
	 * @param newPosition the position we want to set to the editBox
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(EditBoxBridge editBox, Position newPosition) {
		editBox.setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab EditBox
	 * @param editBox the editBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the editBox (true to set the editBox visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(EditBoxBridge editBox, boolean newVisibleState) {
		editBox.setVisible(newVisibleState);
	}
}
