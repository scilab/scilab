
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab frames in GUIs
 * @author Vincent COUVERT
 */
public class ScilabFrameBridge {

	/**
	 * Constructor
	 */
	protected ScilabFrameBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	};

	/**
	 * Creates a Scilab Frame
	 * @return the created Frame
	 */
	public static SimpleFrame createFrame() {
		return new SwingScilabFrame();
	}
	
	/**
	 * Sets the Text of a Scilab Frame
	 * @param frame the Frame we want to set the Text of
	 * @param newText the Text we want to set to the Frame
	 */
	public static void setText(Frame frame, String newText) {
		frame.getAsSimpleFrame().setText(newText);
	}
	
	/**
	 * Sets the Text of a Scilab Frame
	 * @param frame the Frame we want to get the Text of
	 * @return the text of the Frame
	 */
	public static String getText(Frame frame) {
		return frame.getAsSimpleFrame().getText();
	}
	
	/**
	 * Draws a Scilab frame
	 * @param frame the frame to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(Frame frame) {
		frame.getAsSimpleFrame().draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Frame
	 * @param frame the frame we want to get the dimensions of
	 * @return the size of the frame
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(Frame frame) {
		return frame.getAsSimpleFrame().getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab frame
	 * @param frame the frame we want to get the position of
	 * @return the position of the frame
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(Frame frame) {
		return frame.getAsSimpleFrame().getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab Frame
	 * @param frame the frame we want to get the visiblity status of
	 * @return the visibility status of the frame (true if the frame is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(Frame frame) {
		return frame.getAsSimpleFrame().isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Frame
	 * @param frame the frame we want to set the dimensions of
	 * @param newSize the size we want to set to the frame
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(Frame frame, Size newSize) {
		frame.getAsSimpleFrame().setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab frame
	 * @param frame the frame we want to set the position of
	 * @param newPosition the position we want to set to the frame
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(Frame frame, Position newPosition) {
		frame.getAsSimpleFrame().setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab Frame
	 * @param frame the frame we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the frame (true to set the frame visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(Frame frame, boolean newVisibleState) {
		frame.getAsSimpleFrame().setVisible(newVisibleState);
	}
	
	/**
	 * Add a callback to the Frame
	 * @param frame the Frame we want to set the callback of
	 * @param command the Scilab command to execute when the contents of the frame is validated
	 * @param commandType the type of the command that will be executed.
	 */
	public static void setCallback(Frame frame, String command, int commandType) {
		frame.getAsSimpleFrame().setCallback(command, commandType);
	}
	
	/**
	 * To set the Background color of the Frame
	 * @param frame the Frame we want to set the background of
	 * @param color the Color
	 */
	public static void setBackground(Frame frame, Color color) {
		frame.getAsSimpleFrame().setBackground(color);
	}

	/**
	 * Get the Background color of the Frame
	 * @param frame the Frame we want to get the background of
	 * @return the Color
	 */
	public static Color getBackground(Frame frame) {
		return frame.getAsSimpleFrame().getBackground();
	}

	/**
	 * To set the Foreground color of the frame
	 * @param frame the Frame we want to set the foreground of
	 * @param color the Color
	 */
	public static void setForeground(Frame frame, Color color) {
		frame.getAsSimpleFrame().setForeground(color);
	}

	/**
	 * Get the Foreground color of the Frame
	 * @param frame the Frame we want to get the foreground of
	 * @return the Color
	 */
	public static Color getForeground(Frame frame) {
		return frame.getAsSimpleFrame().getForeground();
	}

	/**
	 * Set the font of the Frame.
	 * @param frame the Frame we want to set the font of
	 * @param font the font
	 */
	public static void setFont(Frame frame, Font font) {
		frame.getAsSimpleFrame().setFont(font);
	}
	
	/**
	 * Get the font of the Frame.
	 * @param frame the Frame we want to get the font of
	 * @return the font
	 */
	public static Font getFont(Frame frame) {
		return frame.getAsSimpleFrame().getFont();
	}
	
	/**
	 * Set if the Frame is enabled or not
	 * @param frame the Frame we want to set the status of
	 * @param status true if the Frame is enabled
	 */
	public static void setEnabled(Frame frame, boolean status) {
		frame.getAsSimpleFrame().setEnabled(status);
	}
	
	/**
	 * Set the Relief of the Frame
	 * @param frame the Frame we want to set the Relief of
	 * @param reliefType the type of the relief to set (See ScilabRelief.java)
	 */
	public static void setRelief(Frame frame, String reliefType) {
		frame.getAsSimpleFrame().setRelief(reliefType);
	}

	/**
	 * Delete the Frame
	 * @param frame the Frame to be destroyed
	 */
	public static void destroy(Frame frame) {
		frame.getAsSimpleFrame().destroy();
	}
}
