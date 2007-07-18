
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.editbox;

import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab EditBox in GUIs
 * @author Marouane BEN JELLOUL
 */
public class ScilabEditBoxBridge {


	/**
	 * Creates a Scilab EditBox
	 * @return the created EditBox
	 */
	public static SimpleEditBox createEditBox() {
		// FIXME : UI CONTROL must be developped last !!!!!
		return null;	
		//return new SwingScilabEditBox();
	}
	
	/**
	 * Sets the Text of a Scilab EditBox
	 * @param editBox the EditBox we want to set the Text of
	 * @param newText the Text we want to set to the EditBox
	 */
	public static void setText(EditBox editBox, String newText) {
		editBox.getAsSimpleEditBox().setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab EditBox
	 * @param editBox the EditBox we want to get the Text of
	 * @return the text of the EditBox
	 */
	public static String getText(EditBox editBox) {
		return editBox.getAsSimpleEditBox().getText();
	}
	
	/**
	 * Draws a Scilab editBox
	 * @param editBox the editBox to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(EditBox editBox) {
		// FIXME : UI CONTROL MUST BE LAST !!!!
		// editBox.getAsSimpleEditBox().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab EditBox
	 * @param editBox the editBox we want to get the dimensions of
	 * @return the size of the editBox
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(EditBox editBox) {
		// FIXME : UI CONTROL MUST BE LAST !!!!
		// return editBox.getAsSimpleEditBox().getDims();
		return null;
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab editBox
	 * @param editBox the editBox we want to get the position of
	 * @return the position of the editBox
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(EditBox editBox) {
		// FIXME : UI CONTROL MUST BE LAST !!!!
		// return editBox.getAsSimpleEditBox().getPosition();
		return null;
	}

	/**
	 * Gets the visibility status of a Scilab EditBox
	 * @param editBox the editBox we want to get the visiblity status of
	 * @return the visibility status of the editBox (true if the editBox is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(EditBox editBox) {
		// FIXME : UI CONTROL MUST BE LAST !!!!
		//return editBox.getAsSimpleEditBox().isVisible();
		return false;
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab EditBox
	 * @param editBox the editBox we want to set the dimensions of
	 * @param newSize the size we want to set to the editBox
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(EditBox editBox, Size newSize) {
		// FIXME : UI CONTROL MUST BE LAST !!!!
		// editBox.getAsSimpleEditBox().setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab editBox
	 * @param editBox the editBox we want to set the position of
	 * @param newPosition the position we want to set to the editBox
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(EditBox editBox, Position newPosition) {
		// FIXME : UI CONTROL MUST BE LAST !!!!
		//editBox.getAsSimpleEditBox().setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab EditBox
	 * @param editBox the editBox we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the editBox (true to set the editBox visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(EditBox editBox, boolean newVisibleState) {
		// FIXME : UI CONTROL MUST BE LAST !!!!
		//editBox.getAsSimpleEditBox().setVisible(newVisibleState);
	}
}
