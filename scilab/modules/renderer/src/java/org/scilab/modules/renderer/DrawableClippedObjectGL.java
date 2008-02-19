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
import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.glTools.MovableClipPlane3D;

/**
 * Parent class to render clipped objects
 * @author Jean-Baptiste Silvy
 */
public abstract class DrawableClippedObjectGL extends DrawableObjectGL {

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
		GL gl = getGL();
		// drawn points (x,y,z) are the one verifying a.x + b.y + c.z + d >= 0.
		double[] equationXmin = {1.0, 0.0, 0.0, -xMin}; // x >= xMin
		planeXmin = new MovableClipPlane3D(equationXmin);
		planeXmin.clip(gl);

		double[] equationXmax = {-1.0, 0.0, 0.0, xMax}; // x <= xMax
		planeXmax = new MovableClipPlane3D(equationXmax);
		planeXmax.clip(gl);
		
	}
	
	/**
	 * Clip an object with 2 planes, along Y axis
	 * @param yMin minimum shown value on Y axis
	 * @param yMax maximum shown value on Y axis
	 */
	public void clipY(double yMin, double yMax) {
		GL gl = getGL();
		double[] equationYmin = {0.0, 1.0, 0.0, -yMin};
		planeYmin = new MovableClipPlane3D(equationYmin);
		planeYmin.clip(gl);

		double[] equationYmax = {0.0, -1.0, 0.0, yMax};
		planeYmax = new MovableClipPlane3D(equationYmax);
		planeYmax.clip(gl);
	}
	
	/**
	 * Clip an object with 2 planes, along Z axis
	 * @param zMin minimum shown value on Z axis
	 * @param zMax maximum shown value on Z axis
	 */
	public void clipZ(double zMin, double zMax) {
		GL gl = getGL();
		double[] equationZmin = {0.0, 0.0, 1.0, -zMin};
		planeZmin = new MovableClipPlane3D(equationZmin);
		planeZmin.clip(gl);

		double[] equationZmax = {0.0, 0.0, -1.0, zMax};
		planeZmax = new MovableClipPlane3D(equationZmax);
		planeZmax.clip(gl);
	}
	
	
	/**
	 * Disable clipping after a call to clip
	 */
	public void unClip() {
		GL gl = getGL();
		if (planeXmin != null) { planeXmin.unClip(gl); planeXmin = null; }
		if (planeXmax != null) { planeXmax.unClip(gl); planeXmax = null; }
		if (planeYmin != null) { planeYmin.unClip(gl); planeYmin = null; }
		if (planeYmax != null) { planeYmax.unClip(gl); planeYmax = null; }
		if (planeZmin != null) { planeZmin.unClip(gl); planeZmin = null; }
		if (planeZmax != null) { planeZmax.unClip(gl); planeZmax = null; }

	}

}
