/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.arcDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.geom3D.Matrix4D;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * ArcTools
 * @author Sylvestre Koumar
 *
 */
public abstract class ArcTools {
	
	/** number of slice for the circle */
	protected static final int NB_SLICES = 64;
	
	/** scale to transform the ellipse into a circle */
	private Vector3D scale;
	/** To put the ellipse in the right position */
	private Matrix4D rotationMatrix;
	/** center of the ellipse */
	private Vector3D center;
	/** starting angle */
	private double startAngle;
	
	/**size of the angular region of the arc */
	private double angularRegion;	

	
	/**
	 * Constructor
	 * @param center Vector3D
	 * @param semiMinorAxis Vector3D
	 * @param semiMajorAxis Vector3D 
	 * @param startAngle double
	 * @param endAngle double
	 */
	protected ArcTools(Vector3D center, Vector3D semiMinorAxis, Vector3D semiMajorAxis, double startAngle, double endAngle) {
		this.center = center;
		Vector3D thirdVector = semiMinorAxis.crossProduct(semiMajorAxis).getNormalized();
		
		rotationMatrix = new Matrix4D();
		rotationMatrix.setFromRotatedBasis(semiMinorAxis, semiMajorAxis, thirdVector);
		
		// the arc is flat, Z coordinate does not matter.
		scale = new Vector3D(semiMinorAxis.getNorm(), semiMajorAxis.getNorm() , 1.0);
		
		this.startAngle = startAngle;
		this.angularRegion = endAngle - startAngle;
		
	}
	
	/**
	 * Modify OpenGL frame so the ellipse which is the support of the arc become a circle
	 * @param gl current OpenGL pipeline
	 */
	public void setCoordinatesToCircleGL(GL gl) {
		gl.glTranslated(center.getX(), center.getY(), center.getZ());
		gl.glMultMatrixd(rotationMatrix.getOpenGLRepresentation(), 0);
		gl.glScaled(scale.getX(), scale.getY(), scale.getZ());
		// Put the minimum angle has greatest axis, so the draw sart at Y = 0.
		gl.glRotated(Math.toDegrees(startAngle), 0.0, 0.0, 1.0);
	}

	/**
	 * drawArc
	 * @param gl current GL pipeline
	 */
	protected abstract void drawCircle(GL gl);

	/**
	 * @return angular region of the arc.
	 */
	public double getSweepAngle() {
		return angularRegion;
	}

}
