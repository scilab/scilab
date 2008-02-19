/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class positioning the Title label 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.labelDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class positioning the Title label
 * @author Jean-Baptiste Silvy
 */
public class TitlePositionerGL extends LabelPositionerGL {

	private Vector3D viewingAreaUpperPos;
	
	/**
	 * Default constructor.
	 */
	public TitlePositionerGL() {
		super();
		viewingAreaUpperPos = null;
	}
	
	/**
	 * Set the position of the upper position of the viewing area (area
	 * in pixels in whixh the axes must fit).
	 * The upper point should be the middle of the upper edge
	 * @param xPos X coordinate in pixels
	 * @param yPos Y coordinates in pixels
	 */
	public void setViewingAreaUpperPos(int xPos, int yPos) {
		viewingAreaUpperPos = new Vector3D(xPos, yPos, 0.0);
	}
	
	
	/**
	 * Compute the position of the label for best display.
	 * @return vector containg the position 
	 */
	public Vector3D getLabelPosition() {
		GL gl = getGL();
		
		// put the title just upper the box
		Vector3D labelCenterPix = viewingAreaUpperPos.add(new Vector3D(0.0, getBoundingBoxHeight(), 0.0));
		
		return CoordinateTransformation.getTransformation(gl).retrieveSceneCoordinates(gl, labelCenterPix);
	}

}
