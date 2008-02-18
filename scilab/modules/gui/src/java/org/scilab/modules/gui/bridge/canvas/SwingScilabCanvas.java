/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.canvas;

import java.awt.AWTEvent;
import java.awt.Dimension;

import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLJPanel;

import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
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

	private static final long serialVersionUID = 6101347094617535625L;

	/**
	 * Figure Index
	 * Used in Scilab to remember who is who.
	 */
	private int figureIndex;
	
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
		this.setLayout(null);
		this.addGLEventListener(new SciRenderer(figureIndex));
		this.figureIndex = figureIndex;
		
		/**
		 * FIXed : No more Magical Hack !!!
		 * {
		 */
		// Focusable in order to catch KeyEvents...
		this.setFocusable(true);
		// Enable mouse Events sensitivity...
		this.enableEvents(AWTEvent.MOUSE_EVENT_MASK);
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
		cap.setDoubleBuffered(true);
		
		SwingScilabCanvas newCanvas = new SwingScilabCanvas(cap, figureIndex);
		
		// I do this here and not in the ScilabCanvas because it is JOGL related stuff
		DrawableFigureGL correspondigFigure = FigureMapper.getCorrespondingFigure(figureIndex);
		correspondigFigure.setRenderingTarget(newCanvas);
		
		return newCanvas;
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
	/**
	 * Get the Figuer Index : the Scilab ID of the figure.
	 * 
	 * @return the ID.
	 */
	public int getFigureIndex() {
		return figureIndex;
	}
}
