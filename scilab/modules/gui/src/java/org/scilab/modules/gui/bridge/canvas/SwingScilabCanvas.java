
/* Copyright INRIA 2007 */

package org.scilab.modules.gui.bridge.canvas;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLJPanel;

import com.sun.opengl.util.Animator;

import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.container.SimpleContainer;
import org.scilab.modules.gui.menubar.SimpleMenuBar;
import org.scilab.modules.gui.toolbar.SimpleToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Canvas in GUIs
 * This implementation requires JOGL
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabCanvas extends GLJPanel implements SimpleCanvas {

	/**
	 * Constructor
	 * @param cap GLCapabilities associated to the GLJPanel
	 */
	public SwingScilabCanvas(GLCapabilities cap) {
		super(cap);
		// TODO to remove, just for testing
		this.setLayout(new BorderLayout());
	    //this.addGLEventListener(new Renderer());
	    //this.addMouseListener(new GLCanvasMouseListener(this));
	    //Animator animator = new Animator(this);
	    //animator.start();
		//this.setIgnoreRepaint(true);
		//this.setAutoSwapBufferMode(true);
	}

	/**
	 * Create a Scilab Canvas
	 * @return the created canvas
	 */
	public static SimpleCanvas createCanvas() {
		GLCapabilities cap = new GLCapabilities();
		cap.setHardwareAccelerated(true);
		cap.setDoubleBuffered(true);
		return new SwingScilabCanvas(cap);
	}

	/**
	 * Sets a MenuBar to an element
	 * @param newMenuBar the MenuBar to set to the element
	 */
	public void addMenuBar(SimpleMenuBar newMenuBar) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException();
	}

	/**
	 * Sets a ToolBar to an element
	 * @param newToolBar the ToolBar to set to the element
	 */
	public void addToolBar(SimpleToolBar newToolBar) {
		// TODO Auto-generated method stub
		throw new UnsupportedOperationException();
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
	@Override
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
	@Override
	public void setVisible(boolean newVisibleState) {
		super.setVisible(newVisibleState);
	}

	/**
	 * Add this as member (dockable element) to the Scilab container and returns its index
	 * @param container the container in which we add this
	 * @return index of this in container components
	 * @see org.scilab.modules.gui.container.Container#addMember(org.scilab.modules.gui.dockable.Dockable)
	 * @see org.scilab.modules.gui.dockable.Dockable#addAsMemberTo(org.scilab.modules.gui.container.Container)
	 */
	public int addAsMemberTo(SimpleContainer container) {
		// delegate to the container but also adding info on how to handle me (Canvas)
		// Interface Container must describe methode: int addMember(Canvas member);
		return container.addMember((SimpleCanvas) this);
	}

}
