/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class for mark drawing algorithms 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.drawers;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.MarkDrawing.MarkDrawer;
import org.scilab.modules.renderer.utils.MarkDrawing.MarkDrawingStrategy;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.geom3D.GeomAlgos;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Parent class for mark drawing algorithms
 * @author Jean-Baptiste Silvy
 */
public abstract class MarkDrawerGL extends DrawableObjectGL {
	
	/** index of background color */
	private MarkDrawer drawer;
	
	private Vector3D[] markPos;
	
	private int nbMarks;

	/**
	 * Default Constructor
	 */
	public MarkDrawerGL() {
		super();
		drawer = new MarkDrawer();
		markPos = null;
		nbMarks = 0;
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
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		drawer.initializeDrawing(parentFigureIndex);
		drawer.setMarkDrawingStrategy(getParentFigureGL().getMarkDrawingStrategy());
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
	 * Specify a new size and style for marks
	 * @param markStyleIndex index of the markStyle
	 * @param markSize new size
	 * @param markSizeUnit index of the kind of mark
	 */
	public void setMarkSizeAndStyle(int markStyleIndex, int markSize, int markSizeUnit) {
		// Special case for mark_size!
		// for compatibility with Scilab 4
		// dot mark with tabulated size of 0 is a single pixel.
		// This mean it is drawn in pixel size instead of tabulated size
		if (MarkDrawingStrategy.getMarkStyle(markStyleIndex) == MarkDrawingStrategy.MarkStyle.DOT
		    && markSize == 0 && markSizeUnit == 2) {
			setMarkSizeUnit(1);
			setMarkSize(0);
			setMarkStyle(markStyleIndex);
		} else {
			// standard case
			setMarkSizeUnit(markSizeUnit);
			setMarkSize(markSize);
			setMarkStyle(markStyleIndex);
		}
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
		setMarkSizeAndStyle(markStyleIndex, markSize, markSizeUnit);
	}

	/**
	 * Draw marks from already precomputed data
	 */
	public void redrawMarks() {
		GL gl = getGL();
		
		CoordinateTransformation transform = getCoordinateTransformation();
		
		// need to perform this before switching to pixel coordinates
		Vector3D[] pixCoords = new Vector3D[nbMarks];
		for (int i = 0; i < nbMarks; i++) {
			pixCoords[i] = transform.getCanvasCoordinates(gl, markPos[i]);
		}
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		
		// create the display list using pixel coordinates
		startRecordDL();
		// marks are drawn with a line width of 1.
		gl.glLineWidth(1.0f);
		for (int i = 0; i < nbMarks; i++) {
      // Test if mark is drawable before perform drawing.
      if (GeomAlgos.isVector3DRepresentable(markPos[i])) {
  			getDrawer().drawMark(pixCoords[i].getX(), pixCoords[i].getY(), pixCoords[i].getZ());
  	  }
		}
		endRecordDL();
		// we recreate the dl each time
		//getDrawer().clearDisplayList();
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
	}
	
	/**
	 * Show the compute display list
	 */
	public void showMarks() {
		GL gl = getGL();
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		displayDL();
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
	}
	
	/**
	 * Display the marks at the positions specified by marksPosition
	 * @param marksPosition positions in 3D of marks
	 */
	public void drawMarks(Vector3D[] marksPosition) {
		drawMarks(marksPosition, marksPosition.length);
	}
	
	/**
	 * Display the marks at the positions specified by marksPosition
	 * @param marksPosition positions in 3D of marks
	 * @param nbMarks number of marks to display
	 */
	public void drawMarks(Vector3D[] marksPosition, int nbMarks) {
		this.markPos = marksPosition;
		this.nbMarks = nbMarks;
		// mark object has changed, we may need to recreate the display list
		getDrawer().createDisplayList();
		redrawMarks();
	}
	
	/**
	 * To be called when the cpp object is destroyed
	 * @param parentFigureIndex index of parent figure
	 */
	@Override
	public void destroy(int parentFigureIndex) {
		drawer.destroy(parentFigureIndex);
		super.destroy(parentFigureIndex);
	}
}
