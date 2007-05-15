
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.frame;

import org.scilab.modules.console.Console;
import org.scilab.modules.gui.bridge.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.SwingScilabConsole;
import org.scilab.modules.gui.bridge.SwingScilabFrame;
import org.scilab.modules.gui.canvas.Canvas;
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
	 * Creates a Scilab frame
	 * @return the created frame
	 */
	public static Frame createFrame() {
		return new SwingScilabFrame();
	}

	/**
	 * Draws a frame
	 * @param frame the frame we want to draw
	 */
	public static void draw(Frame frame) {
		frame.draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab frame
	 * @param frame the frame we want to get the size of
	 * @return the size of the frame
	 */
	public static Size getDims(Frame frame) {
		return frame.getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab frame
	 * @param frame the frame we want to get the position of
	 * @return the position of the frame
	 */
	public static Position getPosition(Frame frame) {
		return frame.getPosition();
	}

	/**
	 * Gets the visibility status of a frame
	 * @param frame the frame we want to get the visibility status of
	 * @return the visibility status of the frame (true is the frame is visible, false if not)
	 */
	public static boolean isVisible(Frame frame) {
		return frame.isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab frame
	 * @param frame the frame we want to set the dimensions of
	 * @param newFrameSize the size we want to set to the frame
	 */
	public static void setDims(Frame frame, Size newFrameSize) {
		frame.setDims(newFrameSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab frame
	 * @param frame the frame we want to set the position of
	 * @param newFramePosition the position we want to set to the frame
	 */
	public static void setPosition(Frame frame, Position newFramePosition) {
		frame.setPosition(newFramePosition);
	}

	/**
	 * Sets the visibility status of a Scilab frame
	 * @param frame the frame we want to set the visibility status of
	 * @param newVisibleState the visibility status we want to set (true to set the frame visible, false if not)
	 */
	public static void setVisible(Frame frame, boolean newVisibleState) {
		frame.setVisible(newVisibleState);
	}

	/**
	 * Add a member (dockable element) to a frame and returns the index of this member
	 * @param frame the frame where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Frame frame, Frame member) {
		return ((SwingScilabFrame) frame).addMember((SwingScilabFrame) member);
	}

	/**
	 * Add a member (dockable element) to a frame and returns the index of this member
	 * @param frame the frame where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Frame frame, Console member) {
		return ((SwingScilabFrame) frame).addMember((SwingScilabConsole) member);
	}

	/**
	 * Add a member (dockable element) to a frame and returns the index of this member
	 * @param frame the frame where we want to add the member
	 * @param member the member to add
 	 * @return index of member
 	 */
	public static int addMember(Frame frame, Canvas member) {
		return ((SwingScilabFrame) frame).addMember((SwingScilabCanvas) member);
	}
}
