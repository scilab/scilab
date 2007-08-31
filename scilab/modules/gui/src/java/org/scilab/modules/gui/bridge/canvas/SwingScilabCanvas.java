
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.canvas;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.media.opengl.GL;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLJPanel;

import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.renderer.figureDrawing.SciRenderer;

/**
 * Swing implementation for Scilab Canvas in GUIs
 * This implementation requires JOGL
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 * @author Jean-Baptiste Silvy
 */
public class SwingScilabCanvas extends GLJPanel implements SimpleCanvas {

	/**
	 * Constructor
	 * @param cap GLCapabilities associated to the GLJPanel
	 * @param figureIndex index of the displayed figure
	 */
	public SwingScilabCanvas(GLCapabilities cap, int figureIndex) {
		super(cap);
		// TODO to remove, just for testing
		this.setLayout(new BorderLayout());
	    this.addGLEventListener(new SciRenderer(figureIndex));
	}

	/**
	 * Create a Scilab Canvas
	 * @param figureIndex index of the displayed figure
	 * @return the created canvas
	 */
	public static SimpleCanvas createCanvas(int figureIndex) {
		GLCapabilities cap = new GLCapabilities();
		cap.setHardwareAccelerated(true);
		cap.setDoubleBuffered(true);
		return new SwingScilabCanvas(cap, figureIndex);
	}

	/**
	 * Draws a Scilab canvas
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Canvas
	 * @return the size of the canvas
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(this.getWidth(), this.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * @return the position of the canvas
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Gets the visibility status of a Scilab Canvas
	 * @return the visibility status of the canvas (true if the canvas is visible, false if not)
	 * @see org.scilab.modules.gui.UIElement#isVisible()
	 */
	public boolean isVisible() {
		return super.isVisible();
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Canvas
	 * @param newSize the size we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		this.setSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * @param newPosition the position we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}

	/**
	 * Sets the visibility status of a Scilab Canvas
	 * @param newVisibleState the visibility status we want to set to the canvas (true to set the canvas visible, false else)
	 * @see org.scilab.modules.gui.UIElement#setVisible(boolean)
	 */
	public void setVisible(boolean newVisibleState) {
		super.setVisible(newVisibleState);
	}
	
	/**
	 * Force the canvas to render itself immediately.
	 */
	@Override
	public void display() {
		super.display();
	}
	 
	/**
	 * Get the GL pipeline used by the canvas
	 * @return GL pipeline
	 */
	@Override
	public GL getGL() {
		return super.getGL();
	}
	
	/**
	 * @param onOrOff set wether the swap buffer mode is on or off.
	 */
	public void setAutoSwapBufferMode(boolean onOrOff) {
		super.setAutoSwapBufferMode(onOrOff);
	}
	
	/**
	 * @return set wether the swap buffer mode is on or off.
	 */
	public boolean getAutoSwapBufferMode() {
		return super.getAutoSwapBufferMode();
	}
	 
	/**
	 * Force the canvas to render itself with synchronisation with its OpenGL process.
	 */
	@Override
	public void repaint() {
		super.repaint();
	}

}
