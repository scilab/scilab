
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.canvas;

import org.scilab.modules.gui.bridge.SwingScilabCanvas;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Bridge for Scilab Canvas in GUIs
 * @author Vincent COUVERT
 */
public class ScilabCanvasBridge {

	/**
	 * Constructor
	 */
	protected ScilabCanvasBridge() {
		throw new UnsupportedOperationException(); /* Prevents calls from subclass */
	}

	/**
	 * Creates a Scilab Canvas
	 * @return the created canvas
	 */
	public static Canvas createCanvas() {
		return SwingScilabCanvas.createCanvas();
	}

	/**
	 * Draws a Scilab canvas
	 * @param canvas the canvas to draw
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public static void draw(Canvas canvas) {
		canvas.draw();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Canvas
	 * @param canvas the canvas we want to get the dimensions of
	 * @return the size of the canvas
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public static Size getDims(Canvas canvas) {
		return canvas.getDims();
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * @param canvas the canvas we want to get the position of
	 * @return the position of the canvas
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public static Position getPosition(Canvas canvas) {
		return canvas.getPosition();
	}

	/**
	 * Gets the visibility status of a Scilab Canvas
	 * @param canvas the canvas we want to get the visiblity status of
	 * @return the visibility status of the canvas (true if the canvas is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public static boolean isVisible(Canvas canvas) {
		return canvas.isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Canvas
	 * @param canvas the canvas we want to set the dimensions of
	 * @param newSize the size we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public static void setDims(Canvas canvas, Size newSize) {
		canvas.setDims(newSize);
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * @param canvas the canvas we want to set the position of
	 * @param newPosition the position we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public static void setPosition(Canvas canvas, Position newPosition) {
		canvas.setPosition(newPosition);
	}

	/**
	 * Sets the visibility status of a Scilab Canvas
	 * @param canvas the canvas we want to set the visiblity status of
	 * @param newVisibleState the visibility status we want to set to the canvas (true to set the canvas visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public static void setVisible(Canvas canvas, boolean newVisibleState) {
		canvas.setVisible(newVisibleState);
	};

}
