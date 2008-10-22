/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines position the
 * camera with standard pojection    
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

/**
 * Class containing the driver dependant routines position the
 *        camera with standard pojection
 * @author Jean-Baptiste Silvy
 */
public class IsometricCameraGL extends CameraGL {
	
	/**
	 * Default constructor.
	 */
	public IsometricCameraGL() {
		super();
	}
	
	/**
	 * Compute the scale wich will best fit the window in accordance with viewing angles.
	 */
	@Override
	protected void computeFittingScale() {
		GL gl = getGL();
		
		double[] screenExtent = getBoxScreenExtent(gl);		
		double[] marginSize = getMarginSize();

		gl.glScaled(marginSize[0] / screenExtent[0], marginSize[1] / screenExtent[1], 1.0);
	}
	
	/**
	 * Set the viewPort of the object.
	 */
	@Override
	protected void setViewPort() {
		//get width ad height of the viewPort
		setViewPortSize(1.0, 1.0);
	}

}
