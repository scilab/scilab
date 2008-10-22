/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines position the
 * camera with an isoview scale
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
 * camera with an isoview scale
 * @author Jean-Baptiste Silvy
 */
public class IsoViewCameraGL extends CameraGL {

	/**
	 * Default constructor.
	 */
	public IsoViewCameraGL() {
		super();
	}
	
	/**
	 * Compute the scale wich will best fit the window in accordance with viewing angles.
	 */
	@Override
	protected void computeFittingScale() {
		GL gl = getGL();
		double[] screenExtent = getBoxScreenExtent(gl);
		// apply same factor to preserve isoview.
		double[] marginSize = getMarginSize();
		
		if (is2DCamera()) {
			// don't take Z coordinate into account
			// otherwise it create issues when zooming, ie the X and Y axis become smaller
			// and smaller in order to conserve isoview between the 3 axes (Z axis does not change)
			double minScale = Math.min(marginSize[0] / screenExtent[0], (marginSize[1] / screenExtent[1]));
			gl.glScaled(minScale, minScale, 1.0);
		} else {
			double minScale = Math.min(marginSize[0] / screenExtent[0], Math.min(marginSize[1] / screenExtent[1], 1.0));
			gl.glScaled(minScale, minScale, minScale);
		}
		
		
	}

	/**
	 * Set the viewPort of the object.
	 */
	@Override
	protected void setViewPort() {
		double[] viewPortSize = getViewPortSize();
		
		
		setViewPortSize(viewPortSize[0], viewPortSize[1]);
	}

}
