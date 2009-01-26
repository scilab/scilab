/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class to render clipped objects 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer;

import org.scilab.modules.renderer.utils.glTools.MovableClipPlane3D;

/**
 * Parent class to render clipped objects
 * @author Jean-Baptiste Silvy
 */
public abstract class DrawableClippedObjectGL extends DrawableObjectGL {

	/** Experimental value to avoid to strict clipping */
	private static final double EPSILON = 1.0e-5;
	
	private MovableClipPlane3D planeXmin;
	private MovableClipPlane3D planeXmax;
	private MovableClipPlane3D planeYmin;
	private MovableClipPlane3D planeYmax;
	private MovableClipPlane3D planeZmin;
	private MovableClipPlane3D planeZmax;

	/**
	 * Clip an object with 2 planes, along X axis
	 * @param xMin minimum shown value on X axis
	 * @param xMax maximum shown value on X axis
	 */
	public void clipX(double xMin, double xMax) {
		// add a little offset to avoid being to strict on clipping
		double offset = (xMax - xMin) * EPSILON;
		
		// drawn points (x,y,z) are the one verifying a.x + b.y + c.z + d >= 0.
		double[] equationXmin = {1.0, 0.0, 0.0, -xMin + offset}; // x >= xMin - offset
		planeXmin = new MovableClipPlane3D(equationXmin);
		clipPlane(planeXmin);

		double[] equationXmax = {-1.0, 0.0, 0.0, xMax + offset}; // x <= xMax + offset
		planeXmax = new MovableClipPlane3D(equationXmax);
		clipPlane(planeXmax);
	}
	
	/**
	 * Clip an object with 2 planes, along Y axis
	 * @param yMin minimum shown value on Y axis
	 * @param yMax maximum shown value on Y axis
	 */
	public void clipY(double yMin, double yMax) {
		double offset = (yMax - yMin) * EPSILON;
		
		double[] equationYmin = {0.0, 1.0, 0.0, -yMin + offset};
		planeYmin = new MovableClipPlane3D(equationYmin);
		clipPlane(planeYmin);

		double[] equationYmax = {0.0, -1.0, 0.0, yMax + offset};
		planeYmax = new MovableClipPlane3D(equationYmax);
		clipPlane(planeYmax);
	}
	
	/**
	 * Clip an object with 2 planes, along Z axis
	 * @param zMin minimum shown value on Z axis
	 * @param zMax maximum shown value on Z axis
	 */
	public void clipZ(double zMin, double zMax) {
		double offset = (zMax - zMin) * EPSILON;
		
		double[] equationZmin = {0.0, 0.0, 1.0, -zMin + offset};
		planeZmin = new MovableClipPlane3D(equationZmin);
		clipPlane(planeZmin);

		double[] equationZmax = {0.0, 0.0, -1.0, zMax + offset};
		planeZmax = new MovableClipPlane3D(equationZmax);
		clipPlane(planeZmax);
	}
	
	
	/**
	 * Disable clipping after a call to clip
	 */
	public void unClip() {
		if (planeXmin != null) { unClipPlane(planeXmin); planeXmin = null; }
		if (planeXmax != null) { unClipPlane(planeXmax); planeXmax = null; }
		if (planeYmin != null) { unClipPlane(planeYmin); planeYmin = null; }
		if (planeYmax != null) { unClipPlane(planeYmax); planeYmax = null; }
		if (planeZmin != null) { unClipPlane(planeZmin); planeZmin = null; }
		if (planeZmax != null) { unClipPlane(planeZmax); planeZmax = null; }

	}
	
	/**
	 * Unclip a plane and remove it from the clipping manager
	 * @param plane one of the 6 planes
	 */
	private void unClipPlane(MovableClipPlane3D plane) {
		plane.unClip(getGL());
		getParentFigureGL().getClipPlaneManager().removeClippedPlane(plane);
	}
	
	/**
	 * Clip a plane an insert it into the clipping manager
	 * @param plane one of the 6 planes
	 */
	private void clipPlane(MovableClipPlane3D plane) {
		plane.clip(getGL());
		getParentFigureGL().getClipPlaneManager().addClippedPlane(plane);
	}

}
