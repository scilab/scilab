
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.pushbutton;

import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
* Bridge for Scilab PushButtons in GUIs
* @author Marouane BEN JELLOUL
*/
public class ScilabPushButtonBridge {

	/**
	 * Constructor
	 */
	protected ScilabPushButtonBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a new Scilab PushButton
	 * @return the created PushButton
	 */
	public static SimplePushButton createPushButton() {
		return new SwingScilabPushButton();
	}

	/**
	 * Draws a Scilab pushButton
	 * @param pushButton the pushButton to draw
	 */
	public static void draw(SimplePushButton pushButton) {
		pushButton.draw();
	}
	/**
	 * Gets the dimensions (width and height) of a Scilab pushButton
	 * @param pushButton the pushButton we want to get the dimensions of
	 * @return the dimensions of the pushButton
	 */
	public static Size getDims(SimplePushButton pushButton) {
		return pushButton.getDims();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab pushButton
	 * @param pushButton the pushButton we want to set the dimensions of
	 * @param newSize the dimensions we want to set to the pushButton
	 */
	public static void setDims(SimplePushButton pushButton, Size newSize) {
		pushButton.setDims(newSize);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab pushButton
	 * @param pushButton the pushButton we want to get the position of
	 * @return the position of the pushButton
	 */

	public static Position getPosition(SimplePushButton pushButton) {
		return pushButton.getPosition();
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab pushButton
	 * @param pushButton the pushButton we want to set the position of
	 * @param newPosition the position to set to the pushButton
	 */
	public static void setPosition(SimplePushButton pushButton, Position newPosition) {
		pushButton.setPosition(newPosition);
	}

	/**
	 * Gets the visibility status of a Scilab pushButton
	 * @param pushButton the pushButton we want to getthe visibility status of
	 * @return the visibility status of the pushButton (true if the pushButton is visible, false if not)
	 */
	public static boolean isVisible(SimplePushButton pushButton) {
		return pushButton.isVisible();
	}

	/**
	 * Sets the visibility status of a Scilab pushButton
	 * @param pushButton the pushButton we want to set the visibility status of
	 * @param newVisibleState the visibility status we want to set to the pushButton (true to set the pushButton visible, false else)
	 */
	public static void setVisible(SimplePushButton pushButton, boolean newVisibleState) {
		pushButton.setVisible(newVisibleState);
	}
	
	/**
	 * Sets the Text of a Scilab PushButton
	 * @param pushButton the PushButton we want to set the Text of
	 * @param newText the Text we want to set to the PushButton
	 */
	public static void setText(SimplePushButton pushButton, String newText) {
		pushButton.setText(newText);
	}
	
}

