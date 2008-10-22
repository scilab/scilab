/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.canvas;

import java.awt.Dimension;
import java.awt.Rectangle;

import javax.media.opengl.GLCapabilities;
import javax.swing.Scrollable;

import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

/**
 * Class adding defining scrolling behaviour for a canvas
 * @author Jean-Baptiste Silvy
 */
public class ScrollabeSwingScilabCanvas extends SwingScilabCanvas implements
		Scrollable {
	
	private static final int SMALL_DISPLACEMENT = 2;
	private static final int LARGE_DISPLACEMENT = 20;
	
	private boolean autoResizeMode;


	/**
	 * Constructor
	 * 
	 * @param cap
	 *            GLCapabilities associated to the GLJPanel
	 * @param figureIndex
	 *            index of the displayed figure
	 */
	public ScrollabeSwingScilabCanvas(GLCapabilities cap, int figureIndex) {
		super(cap, figureIndex);
		autoResizeMode = true;
	}
	
	/**
	 * Create a Scilab Canvas
	 * 
	 * @param figureIndex index of the displayed figure
	 * @return the created canvas
	 */
	public static ScrollabeSwingScilabCanvas createCanvas(int figureIndex) {
		GLCapabilities cap = new GLCapabilities();
		// to enable anti aliasing
		//cap.setSampleBuffers(true);
	    //cap.setNumSamples(4);
		
		ScrollabeSwingScilabCanvas newCanvas = new ScrollabeSwingScilabCanvas(cap, figureIndex);
		
		// I do this here and not in the ScilabCanvas because it is JOGL related stuff
		DrawableFigureGL correspondigFigure = FigureMapper.getCorrespondingFigure(figureIndex);
		correspondigFigure.setRenderingTarget(newCanvas);
		
		return newCanvas;
	}
	
	/**
	 * Override setSize to be sure that setPreferred size is called
	 * @param newSize new size to apply
	 */
	public void setSize(Dimension newSize) {
		super.setSize(newSize);
		super.setPreferredSize(newSize);
	}
	
	/**
	 * Specify wether the canvas should fit the parent tab size
	 * (and consequently the scrollpane size) or not
	 * @param onOrOff true to enable autoresize mode
	 */
	public void setAutoResizeMode(boolean onOrOff) {
		this.autoResizeMode = onOrOff;
	}
	
	/**
	 * @return wether the resize mode is on or off
	 */
	public boolean getAutoResizeMode() {
		return autoResizeMode;
	}
	
	/**
	 * Get the size of the viewport viewing the canvas
	 * @return wanted size
	 */
	public Dimension getPreferredScrollableViewportSize() {
		return getPreferredSize();
	}

	/**
	 * Get the displacement in pixels when user click on the scrollbar track
	 * @param visibleRect The view area visible within the viewport
	 * @param orientation Either SwingConstants.VERTICAL or SwingConstants.HORIZONTAL.
	 * @param direction Less than zero to scroll up/left, greater than zero for down/right.
	 * @return displacement in pixel
	 */
	public int getScrollableBlockIncrement(Rectangle visibleRect, int orientation, int direction) {
		return LARGE_DISPLACEMENT;
	}

	/**
	 * Specify wether the canvas height must match the viewport height
	 * @return true if the canvas must match the viewport height
	 */
	public boolean getScrollableTracksViewportHeight() {
		return autoResizeMode;
	}

	/**
	 * Specify wether the canvas height must match the viewport width
	 * @return true if the canvas must match the viewport width
	 */
	public boolean getScrollableTracksViewportWidth() {
		return autoResizeMode;
	}

	/**
	 * Get the displacement in pixels when user click on the scrollbar buttons
	 * @param visibleRect The view area visible within the viewport
	 * @param orientation Either SwingConstants.VERTICAL or SwingConstants.HORIZONTAL.
	 * @param direction Less than zero to scroll up/left, greater than zero for down/right.
	 * @return displacement in pixel
	 */
	public int getScrollableUnitIncrement(Rectangle visibleRect, int orientation, int direction) {
		return SMALL_DISPLACEMENT;
	}

}
