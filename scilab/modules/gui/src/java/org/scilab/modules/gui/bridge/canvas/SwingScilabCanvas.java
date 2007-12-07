/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.canvas;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.media.opengl.GL;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLJPanel;

import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.events.GlobalKeyEventWatcher;
import org.scilab.modules.gui.events.GlobalMouseEventWatcher;
import org.scilab.modules.gui.layout.LayoutManager;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.renderer.figureDrawing.SciRenderer;

/**
 * Swing implementation for Scilab Canvas in GUIs This implementation requires
 * JOGL
 * 
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 * @author Jean-Baptiste Silvy
 */
public class SwingScilabCanvas extends GLJPanel implements SimpleCanvas {

	/**
	 * Constructor
	 * 
	 * @param cap
	 *            GLCapabilities associated to the GLJPanel
	 * @param figureIndex
	 *            index of the displayed figure
	 */
	public SwingScilabCanvas(GLCapabilities cap, int figureIndex) {
		super(cap);
		// TODO to remove, just for testing
		// this.setLayout(null);
		this.addGLEventListener(new SciRenderer(figureIndex));
		
		/**
		 * FIXME : Magical Hack !!!
		 * Works as this... must investigate more to understand why...
		 * {
		 */
		// Opaque and Focusable in order to catch KeyEvents...
		this.setFocusable(true);
		this.setOpaque(true);
	
		MouseListener toto =  new MouseListener() {
			public void mouseClicked(MouseEvent arg0) {
				//System.out.println("*** Clicked on this fucking Canvas !!!");
			}
			public void mouseEntered(MouseEvent arg0) { }
			public void mouseExited(MouseEvent arg0) { }
			public void mousePressed(MouseEvent arg0) {	}
			public void mouseReleased(MouseEvent arg0) { }
		};
		
		super.addMouseListener(toto);
		//this.removeMouseListener(toto);
		/**
		 * }
		 * end Magical HACK
		 */
	}

	/**
	 * Create a Scilab Canvas
	 * 
	 * @param figureIndex
	 *            index of the displayed figure
	 * @return the created canvas
	 */
	public static SwingScilabCanvas createCanvas(int figureIndex) {
		GLCapabilities cap = new GLCapabilities();
		cap.setHardwareAccelerated(true);
		cap.setDoubleBuffered(true);
		return new SwingScilabCanvas(cap, figureIndex);
	}

	/**
	 * Draws a Scilab canvas
	 * 
	 * @see org.scilab.modules.gui.UIElement#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Canvas
	 * 
	 * @return the size of the canvas
	 * @see org.scilab.modules.gui.UIElement#getDims()
	 */
	public Size getDims() {
		return new Size(this.getWidth(), this.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * 
	 * @return the position of the canvas
	 * @see org.scilab.modules.gui.UIElement#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Canvas
	 * 
	 * @param newSize
	 *            the size we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		this.setSize(new Dimension(newSize.getWidth(), newSize.getHeight()));
	}

	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 * 
	 * @param newPosition
	 *            the position we want to set to the canvas
	 * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}


}
