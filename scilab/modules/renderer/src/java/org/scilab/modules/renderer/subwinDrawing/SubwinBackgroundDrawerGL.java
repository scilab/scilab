/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class for drawing different kind of axes box 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class dedicated at drawing background of filled axes
 * @author Jean-Baptiste Silvy
 */
public class SubwinBackgroundDrawerGL extends SubwinBoxDrawer {

	private int backgroundColor;
	
	/**
	 * Default constructor
	 */
	public SubwinBackgroundDrawerGL() {
		super();
		backgroundColor = 0;
	}
	
	/**
	 * An other seetting for parameters since line colro is not needed
	 * @param backgroundColor color of the background
	 */
	public void setBoxParameters(int backgroundColor) {
		setBackgroundColor(backgroundColor);
	}
	
	/**
	 * Draw the box specified by its bounds.
	 * @param concealedCornerIndex index of the concealed corner
	 */
	protected void drawBox(int concealedCornerIndex) {
		drawBackground(concealedCornerIndex);
	}
	
	/**
	 * Set the background color of the subwindow (facets color of the back trihedron).
	 * @param colorIndex colormap index of the color.
	 */
	public void setBackgroundColor(int colorIndex) {
		this.backgroundColor = colorIndex;
	}
	
	/**
	 * @return Array of size 3 containing the 3 channels of the background color.
	 */
	public double[] getBackgroundColor() {
		return getColorMap().getColor(backgroundColor);
	}
	
	/**
	 * Draw the background of the box
	 * @param concealedCornerIndex index of the concealed corner
	 */
	protected void drawBackground(int concealedCornerIndex) {
		GL gl = getGL();
		double[] backColor = getBackgroundColor();
		gl.glColor3d(backColor[0], backColor[1], backColor[2]);
		
		// push back polygons from the box lines
		// just draw them in the back otherwise it will cut objects
		// which are drawn outside axes box
		//gl.glDisable(GL.GL_DEPTH_TEST);
		CoordinateTransformation transform = getParentFigureGL().getCoordinateTransformation();
		
		transform.drawBack(gl);
		
		// otherwise Z fighting with grid
		GLTools.pushPolygonsBack(gl);
		gl.glBegin(GL.GL_QUADS);
		for (int i = 0; i < BACK_FACETS[concealedCornerIndex].length; i++) {
			for (int j = 0; j < BACK_FACETS[concealedCornerIndex][i].length; j++) {
				Vector3D curVertex = getBoxCorner(BACK_FACETS[concealedCornerIndex][i][j]);
				gl.glVertex3d(curVertex.getX(), curVertex.getY(), curVertex.getZ());
			}
		}
		gl.glEnd();
		GLTools.endPushPolygonsBack(gl);
		
		// back to default mode
		
		//gl.glEnable(GL.GL_DEPTH_TEST);
		transform.drawMiddle(gl);
	}

}
