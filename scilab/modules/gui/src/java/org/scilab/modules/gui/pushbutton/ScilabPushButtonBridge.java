
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
	public static void draw(PushButton pushButton) {
		pushButton.getAsSimplePushButton().draw();
	}
	/**
	 * Gets the dimensions (width and height) of a Scilab pushButton
	 * @param pushButton the pushButton we want to get the dimensions of
	 * @return the dimensions of the pushButton
	 */
	public static Size getDims(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().getDims();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab pushButton
	 * @param pushButton the pushButton we want to set the dimensions of
	 * @param newSize the dimensions we want to set to the pushButton
	 */
	public static void setDims(PushButton pushButton, Size newSize) {
		pushButton.getAsSimplePushButton().setDims(newSize);
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab pushButton
	 * @param pushButton the pushButton we want to get the position of
	 * @return the position of the pushButton
	 */

	public static Position getPosition(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().getPosition();
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab pushButton
	 * @param pushButton the pushButton we want to set the position of
	 * @param newPosition the position to set to the pushButton
	 */
	public static void setPosition(PushButton pushButton, Position newPosition) {
		pushButton.getAsSimplePushButton().setPosition(newPosition);
	}

	/**
	 * Gets the visibility status of a Scilab pushButton
	 * @param pushButton the pushButton we want to getthe visibility status of
	 * @return the visibility status of the pushButton (true if the pushButton is visible, false if not)
	 */
	public static boolean isVisible(PushButton pushButton) {
		return pushButton.getAsSimplePushButton().isVisible();
	}

	/**
	 * Sets the visibility status of a Scilab pushButton
	 * @param pushButton the pushButton we want to set the visibility status of
	 * @param newVisibleState the visibility status we want to set to the pushButton (true to set the pushButton visible, false else)
	 */
	public static void setVisible(PushButton pushButton, boolean newVisibleState) {
		pushButton.getAsSimplePushButton().setVisible(newVisibleState);
	}
	
	/**
	 * Sets the Text of a Scilab PushButton
	 * @param pushButton the PushButton we want to set the Text of
	 * @param newText the Text we want to set to the PushButton
	 */
	public static void setText(PushButton pushButton, String newText) {
		pushButton.getAsSimplePushButton().setText(newText);
	}
	
	/**
	 * Sets the icon of a PushButton
	 * @param pushButton the PushButton we want to set the icon of
	 * @param filename the path to the icon image to set to the PushButton
	 */
	public static void setIcon(PushButton pushButton, String filename) {
		pushButton.getAsSimplePushButton().setIcon(filename);
	}

	/**
	 * Add a callback to the pushbutton, this callback is a Scilab command
	 * @param pushButton the PushButton we want to set the callback of
	 * @param command the Scilab command to execute when the pushbutton is activated
	 * @param commandType the type of the command that will be executed.
	 */
	public static void setCallback(PushButton pushButton, String command, int commandType) {
		pushButton.getAsSimplePushButton().setCallback(command, commandType);
	}
}

