/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class used to draw different kinds of marks 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.MarkDrawing;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.glTools.GLTools;

import javax.media.opengl.GL;

/**
 * Utility class to draw marks
 * @author Jean-Baptiste Silvy
 */
public class MarkDrawer extends DrawableObjectGL {

	
	/**
	 * The two kinds of markSize unit
	 */
	public static enum MarkSizeUnit { POINT, TABULATED };
	
	/**
	 * Mapping between tabulated sizes and point sizes
	 */ 
	private static final int[] TABULATED_SIZE = {8, 10, 12, 14, 18, 24};
	
	/** To know the type of unit */
	private MarkSizeUnit unit; 
	/** background color of marks */
	private int markBackground;
	/** outline color of marks */
	private int markForeground;
	/** Size of the mark */
	private int markSize;
	/** drawer of dots */
	private MarkDrawingStrategy drawer;
	
	
	/**
	 * Default constructor
	 */
	public MarkDrawer() {
		unit = MarkSizeUnit.POINT;
		markBackground = -1;
		markForeground = -1;
		markSize       = -1;
		drawer         = null;
	}
	
	
	
	/**
	 * @param background index of new background color
	 */
	public void setBackground(int background) {
		markBackground = background;
	}
	
	/**
	 * @param foreground index of new background color
	 */
	public void setForeground(int foreground) {
		markForeground = foreground;
	}
	
	/**
	 * Specify that size unit is tabulated.
	 */
	public void setTabulatedUnit() {
		unit = MarkSizeUnit.TABULATED;
	}
	
	/**
	 * Specify that size unit is point.
	 */
	public void setPointUnit() {
		unit = MarkSizeUnit.POINT;
	}
	
	/**
	 * Specify a new size for marks
	 * @param markSize new size
	 */
	public void setMarkSize(int markSize) {
		this.markSize = markSize;
	}
	
	/**
	 * Specify the kind of mark to draw
	 * @param markStyleIndex index of the kind of mark
	 */
	public void setMarkStyle(int markStyleIndex) {
		drawer = MarkDrawingStrategy.create(markStyleIndex);
	}
	
	
	/**
	 * Get the size of marks in pixel wether it is tabulated or not.
	 * @return size in pixels
	 */
	protected int getMarkPixelSize() {
		int res = 0;
		// point
		if (unit == MarkSizeUnit.POINT) {
			res = markSize;
		} else {
			
			// tabulated
			if (markSize >= 0 && markSize < TABULATED_SIZE.length) {
				res = TABULATED_SIZE[markSize];
			} else if (markSize < 0) {
				res = 0;
			} else {
				res = TABULATED_SIZE[TABULATED_SIZE.length - 1];
			}
		}
		
		// devide by 2, becaus marks are drawn in interval [-1,1]x[1,1]
		// so a 2x2 square
		return (res / 2 + 1);
		
	}
	
	/**
	 * Unused
	 * @param parentFigureIndex unused
	 */
	public void show(int parentFigureIndex) { }
	
	/**
	 * End the recoding of a display list.
	 * Need to be called after a startRecordDL
	 */
	protected void endRecordDL() {
		// just finish to record the display list, don't draw it.
		getGL().glEndList();
	}
	
	/**
	 * 
	 */
	public void createDisplayList() {
		GL gl = getGL();
		
		
		startRecordDL();
		// drawer might be null if the mark index is not good
		if (drawer != null) {
			// enable offset factor to be sure that lines are drawn in font of polygons
			CoordinateTransformation transform = getCoordinateTransformation();
			
			transform.pushPolygonsBack(gl);
			double realMarkSize = getMarkPixelSize();
			gl.glScaled(realMarkSize, realMarkSize, 1.0);
      //set transparency
      drawer.setFrontTransparency(markForeground==-1);
      drawer.setBackTransparency(markBackground==-1);
			drawer.drawMark(gl, getColorMap().getColor(markBackground), getColorMap().getColor(markForeground));
			transform.endPushPolygonsBack(gl);
		}
		
		endRecordDL();
	}
	
	/**
	 * Draw a mark at the specified postion
	 * @param posX X coordinate of the mark
	 * @param posY Y coordinate of the mark
	 * @param posZ Z coordinate of the mark
	 */
	public void drawMark(double posX, double posY, double posZ) {
		if (drawer != null) {
			GL gl = getGL();
			gl.glPushMatrix();
			// put position into integer in order to center the mark on pixels
			gl.glTranslated(Math.round(posX), Math.round(posY), posZ);
			
			// draw the mark
			displayDL();
			
			gl.glPopMatrix();
		}
	}
	
	
}
