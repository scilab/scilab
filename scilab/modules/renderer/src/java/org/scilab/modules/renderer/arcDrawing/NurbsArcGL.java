/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class with utility functions to draw arcs  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.arcDrawing;

import javax.media.opengl.glu.GLU;
import javax.media.opengl.glu.GLUnurbs;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class with utility functions to draw arcs
 * @author Jean-Baptiste Silvy
 */
public final class NurbsArcGL {

	/** Number of control points to draw an circle arc*/
	public static final int NB_CONTROL_POINTS = 3;
	/** Number of coordinates of a control point */
	public static final int SIZE_4D = 4;
	/** Number of coordinates of a control point */
	public static final int SIZE_3D = 3;
	/** Number of coordinates of a control point */
	public static final int SIZE_2D = 2;	

	
	/** Angle for a quarter of a circle */
	protected static final double QUARTER_ANGLE = Math.PI / 2.0;
	
	/** number of quarter in a circle */
	protected static final int NB_QUARTER_MAX = 4;
	
	
	/* Maximal sampling tolerance */
	// can not currently be used.
	//private static final float SAMPLING_TOLERANCE = 5.0f;
	
	
	/**
	 * Default contstructor.
	 * Should not be called
	 */
	protected NurbsArcGL() {
		
	}
	
	/**
	 * Set the default properties for the nurbs obj.
	 * @param glu current glu instance.
	 * @param nurbsObj current nurbs object
	 */
	public static void setGluProperties(GLU glu, GLUnurbs nurbsObj) {
		// Nurbs property can not be modified in current JOGL version.
		// TODO check when newer version of JOGL become available.
		//glu.gluNurbsProperty(GLU.GLU_CULLING, GL.GL_TRUE);
		//nurbsObj.gluNurbsProperty(GluNurbsConst.GLU_SAMPLING_TOLERANCE, SAMPLING_TOLERANCE);
	}
	
	/**
	 * Compute the control points to draw an arc (of radius 1) with glu nurbs in 4D (x,y,z,w).
	 * @param startAngle beginning angle of the arc
	 * @param sweepAngle angular size of the arc
	 * @return control points to use with gluNurbsCurve
	 */
	public static float[] computeArcControlPoints4D(double startAngle, double sweepAngle) {
		// The control polygon is made of three points.
		// The first and last are the starting and end point of the arc (ie [cos(startAngle),sin(startAngle)]
		// and [cos(endAngle),sin(endAngle)]).
		// The middle point is such has the three forms an isocele triangle. Its two angles on the first and
		// last control points are equal to sweepAngle/2.
		
		double endAngle = startAngle + sweepAngle;
		
		// middle of the isocele triangle base
		Vector3D baseMiddle = new Vector3D((Math.cos(startAngle) + Math.cos(endAngle)) / 2.0,
				                           (Math.sin(startAngle) + Math.sin(endAngle)) / 2.0, 0.0);
		
		// height of the isocele triangle bewteen the first and last control points.
		double height = Math.sin(sweepAngle / 2.0) * Math.tan(sweepAngle / 2.0);
		
		// the three points origin, baseMult and the middle control points are aligned
		// and cp = O + baseMiddle + height * dir, where dir is the direction between the three
		Vector3D controlPoint = baseMiddle.add(baseMiddle.getNormalized().scalarMult(height));
		
		
		// the wheight of the middle point is cos(angle / 2.0), so
		controlPoint = controlPoint.scalarMult(Math.cos(sweepAngle / 2.0));
		
		float[] controlPoints = {(float) Math.cos(startAngle), (float) Math.sin(startAngle), 0.0f, 1.0f,
								 (float) controlPoint.getX(), (float) controlPoint.getY(),
								 (float) controlPoint.getZ(), (float) Math.cos(sweepAngle / 2.0),
								 (float) Math.cos(endAngle), (float) Math.sin(endAngle), 0.0f, 1.0f };
		
		return controlPoints;
	}
	
	/**
	 * Compute the control points to draw an arc (of radius 1) with glu nurbs in 3D (x,y,w).
	 * @param startAngle beginning angle of the arc
	 * @param sweepAngle angular size of the arc
	 * @return control points to use with gluNurbsCurve
	 */
	public static float[] computeArcControlPoints3D(double startAngle, double sweepAngle) {
		float[] controlPoints4D = computeArcControlPoints4D(startAngle, sweepAngle);
		float[] controlPoints3D = new float[NB_CONTROL_POINTS * SIZE_3D];
		
		
		// copy the 4D array in the 3D array and remove z coordinate.
		int ind4D = 0;
		int ind3D = 0;
		while (ind4D < controlPoints4D.length) {
			if (ind4D % SIZE_4D == 2) {
				// z coordinate
				ind4D++;
			} else {
				controlPoints3D[ind3D] = controlPoints4D[ind4D];
				ind4D++;
				ind3D++;
			}
		}		
		return controlPoints3D;		
	}
	
	
	/**
	 * createNurbsDrawer 
	 * @param glu GLU 
	 * @return nurbsObj GLUnurbs
	 */
	public static GLUnurbs createNurbsDrawer(GLU glu) {
		GLUnurbs nurbsObj = glu.gluNewNurbsRenderer();
		setGluProperties(glu, nurbsObj);
		return nurbsObj;
	}
	
	/**
	 * destroyNurbsObj
	 * @param glu GLU
	 * @param nurbsObj GLUnurbs
	 */
	public static void destroyNurbsObj(GLU glu, GLUnurbs nurbsObj) {
		// not yet implemented in JOGL
		//glu.gluDeleteNurbsRenderer(nurbsObj);
	}
	
	
}
