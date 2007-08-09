/*------------------------------------------------------------------------*/
/* file: MarkDrawerGL.java                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Parent class for mark drawing algorithms                        */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.drawers;

import org.scilab.modules.renderer.DrawableObjectGL;

import org.scilab.modules.renderer.utils.MarkDrawing.MarkDrawer;

/**
 * Parent class for mark drawing algorithms
 * @author Jean-Baptiste Silvy
 */
public abstract class MarkDrawerGL extends DrawableObjectGL {
	
	/** index of background color */
	private MarkDrawer drawer;
	
	/**
	 * Default Constructor
	 */
	public MarkDrawerGL() {
		super();
		drawer = null;
	}
	
	
	/**
	 * @return The drawing object.
	 */
	public MarkDrawer getDrawer() {
		return drawer;
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	@Override
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		drawer = new MarkDrawer();
		drawer.initializeDrawing(parentFigureIndex);
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	@Override
	public void endDrawing() {
		drawer.endDrawing();
		super.endDrawing();
	}
	
	/**
	 * Set rectangle background color
	 * @param color index of background color
	 */
	public void setBackground(int color) {
		drawer.setBackground(color);
	}
	
	/**
	 * Set rectangle foreground color
	 * @param color index of foreground color
	 */
	public void setForeground(int color) {
		drawer.setForeground(color);
	}
	
	/**
	 * Set the marksize unit to use, ie poitn or tabulated
	 * @param markSizeUnit index of the kind of mark
	 */
	public void setMarkSizeUnit(int markSizeUnit) {
		if (markSizeUnit == 2) {
			drawer.setTabulatedUnit();
		} else {
			drawer.setPointUnit();
		}
	}
	
	/**
	 * Specify a new size for marks
	 * @param markSize new size
	 */
	public void setMarkSize(int markSize) {
		drawer.setMarkSize(markSize);
	}
	
	
	/**
	 * Specify the kind of mark to draw
	 * @param markStyleIndex index of the markStyle
	 */
	public void setMarkStyle(int markStyleIndex) {
		drawer.setMarkStyle(markStyleIndex);
	}
	
	
	/**
	 * A single mothod to set every parameters
	 * @param background index of background color
	 * @param foreground index of foreground color
	 * @param markSizeUnit index of markSize unit
	 * @param markSize markSize in pixels or tabulated
	 * @param markStyleIndex index of mark style
	 */
	public void setMarkParameters(int background, int foreground, int markSizeUnit, int markSize, int markStyleIndex) {
		setBackground(background);
		setForeground(foreground);
		setMarkSizeUnit(markSizeUnit);
		setMarkSize(markSize);
		setMarkStyle(markStyleIndex);
	}
	
	/**
	 * To be called when the cpp object is destroyed
	 * @param parentFigureIndex index of parent figure
	 */
	@Override
	public void destroy(int parentFigureIndex) {
		super.destroy(parentFigureIndex);
		drawer.destroy(parentFigureIndex);
	}
}
