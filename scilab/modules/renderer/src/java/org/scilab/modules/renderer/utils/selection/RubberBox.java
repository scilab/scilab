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


package org.scilab.modules.renderer.utils.selection;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class drawing a selection square on top of the graphic
 * The rectangle is given by 2 points and the rectangle is drawn within the 2
 * @author Jean-Baptiste Silvy
 */
public class RubberBox {

	private DrawableFigureGL parentFigure;
	
	private Vector3D firstPoint;
	private Vector3D secondPoint;
	
	private Vector3D previousFirstPoint;
	private Vector3D previousSecondPoint;
	
	private boolean isActive;
	
	/**
	 * Default constructor
	 * @param parentFigure figure on which the rubberBox will be draw
	 */
	public RubberBox(DrawableFigureGL parentFigure) {
		super();
		firstPoint = new Vector3D();
		secondPoint = new Vector3D();
		previousFirstPoint = null;
		previousSecondPoint = null;
		isActive = false;
		this.parentFigure = parentFigure;
		parentFigure.setRubberBox(this);
	}

	/**
	 * Set the coordinate of the fist corner of the rectangle
	 * @param firstPoint new value for first point in pixels
	 */
	public void setFirstPoint(Vector3D firstPoint) {
		this.firstPoint.setValues(firstPoint);
	}
	
	/**
	 * Set the coordinate of the fist corner of the rectangle
	 * @param secondPoint new value for the second point in pixels
	 */
	public void setSecondPoint(Vector3D secondPoint) {
		this.secondPoint.setValues(secondPoint);
	}
	
	/**
	 * Set the second point with same values as the first point.
	 * This result in an empty rubber box
	 */
	public void setEmptySelection() {
		this.secondPoint.setValues(firstPoint);
	}
	
	/**
	 * Set the coordinate of the first corner of the rectangle
	 * @param xCoord X coordinate of the first point in pixels
	 * @param yCoord Y coordinate of the first point in pixels
	 */
	public void setFirstPoint(int xCoord, int yCoord) {
		this.setFirstPoint(new Vector3D(xCoord, yCoord, 0.0));
	}
	
	/**
	 * Set the coordinate of the second corner of the rectangle
	 * @param xCoord X coordinate of the second point in pixels
	 * @param yCoord Y coordinate of the second point in pixels
	 */
	public void setSecondPoint(int xCoord, int yCoord) {
		this.setSecondPoint(new Vector3D(xCoord, yCoord, 0.0));
	}
	
	/**
	 * @return X coordinate of the first point in pixel
	 */
	protected int getFirstPointX() {
		return (int) firstPoint.getX();
	}
	
	/**
	 * @return X coordinate of the second point in pixel
 	 */
	protected int getSecondPointX() {
		return (int) secondPoint.getX();
	}
	
	/**
	 * @return Y coordinate of the first point in pixel
	 */
	protected int getFirstPointY() {
		return (int) firstPoint.getY();
	}
	
	/**
	 * @return Y coordinate of the second point in pixel
	 */
	protected int getSecondPointY() {
		return (int) secondPoint.getY();
	}
	
	/**
	 * Activate rubber box drawing
	 */
	public void activate() {
		isActive = true;
	}
	
	/**
	 * Draw the rubber box on the canvas
	 */
	public void draw() {
		// chack if the rubber box has been activated
		if (parentFigure != null && isActive) {
			parentFigure.drawCanvas();
		}
	}
	
	/**
	 * Disable the drawing of the rubberbox
	 */
	public void deactivate() {
		if (parentFigure != null) {
			parentFigure.removeRubberBox();
			
			// redraw the figure to get rid of the rubberbox
			// improvement. We may just need to erase the rubber box
			if (isActive) {
				parentFigure.drawCanvas();
			}
			parentFigure = null;
			isActive = false;
		}
	}
	
	/**
	 * Actually draw the 
	 */
	public void drawInContext() {
		GL gl = parentFigure.getGL();
		
		gl.glClear(GL.GL_DEPTH_BUFFER_BIT);
		gl.glClearDepth(1.0);
		// set logical op to equiv
		gl.glEnable(GL.GL_COLOR_LOGIC_OP); // to use pixel drawing mode
	    gl.glLogicOp(GL.GL_EQUIV);
		
	    // Set pixel coordinates view
	    GLTools.usePixelCoordinates(gl, parentFigure);
	    
	    // use black so will invert color with GL_EQUIV
	    gl.glColor3d(0.0, 0.0, 0.0);
	    
	    // trick here
	    // we erase the previous rectangle by redrawing it once
	    // since we use equiv mode, it will restore the previous color
	    if (previousFirstPoint != null) {
	    	erasePreviousBox(gl);
	    }
	    
		// draw the new one
	    drawNewBox(gl);
	    
	    previousFirstPoint = new Vector3D(firstPoint);
	    previousSecondPoint = new Vector3D(secondPoint);
	    
	    GLTools.endPixelCoordinates(gl, parentFigure);
	}
	
	/**
	 * Next call to draw won't erase the current box.
	 * To be called if the canvas size has changed
	 */
	public void doNotErase() {
		previousFirstPoint = null;
		previousSecondPoint = null;
	}
	
	/**
	 * Erase the previous box.
	 * The previous rectangle is erased by redrawing it once
	 * since we use equiv mode, it will restore the previous color
	 * @param gl current GL pipeline
	 */
	private void erasePreviousBox(GL gl) {
		// we need to invert y coordinate
    	double previousFirstY = parentFigure.getCanvasHeight() - previousFirstPoint.getY();
    	double previousSecondY = parentFigure.getCanvasHeight() - previousSecondPoint.getY();
	    gl.glBegin(GL.GL_LINE_LOOP);
	    gl.glVertex3d(previousFirstPoint.getX(), previousFirstY, previousFirstPoint.getZ());
	    gl.glVertex3d(previousFirstPoint.getX(), previousSecondY, previousFirstPoint.getZ());
	    gl.glVertex3d(previousSecondPoint.getX(), previousSecondY, previousFirstPoint.getZ());
	    gl.glVertex3d(previousSecondPoint.getX(), previousFirstY, previousFirstPoint.getZ());
	    gl.glEnd();
	}
	
	/**
	 * Draw the new box on the screen
	 * @param gl current GL pipeline
	 */
	private void drawNewBox(GL gl) {
		 // we need to invert y coordinate
    	double firstY = parentFigure.getCanvasHeight() - firstPoint.getY();
    	double secondY = parentFigure.getCanvasHeight() - secondPoint.getY();
	    
	    gl.glBegin(GL.GL_LINE_LOOP);
	    gl.glVertex3d(firstPoint.getX(), firstY, firstPoint.getZ());
	    gl.glVertex3d(firstPoint.getX(), secondY, firstPoint.getZ());
	    gl.glVertex3d(secondPoint.getX(), secondY, firstPoint.getZ());
	    gl.glVertex3d(secondPoint.getX(), firstY, firstPoint.getZ());
	    gl.glEnd();
	}
	
	/**
	 * Cancel the rubberbox
	 */
	public void cancelRubberbox() {
		deactivate();
		setEmptySelection();
	}
	
	/**
	 * @return true if the rubberBox is currently being drawn
	 */
	public boolean isActive() {
		return isActive;
	}
	
}
