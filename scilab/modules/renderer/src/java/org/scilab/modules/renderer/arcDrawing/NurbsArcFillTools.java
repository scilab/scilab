/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre Koumar
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
import javax.media.opengl.glu.GLU;
import javax.media.opengl.glu.GLUnurbs;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;



/**
 * NurbsArcFillTools
 * @author Sylvestre Koumar
 * @author Jean-Baptiste Silvy
 */
public class NurbsArcFillTools extends ArcFillTools {

	/** Knots along s paramereter */
	private static final float[] KNOTS_S = {-1.0f, -1.0f, 1.0f, 1.0f};
	
	/** Knots along t parameter */
	private static final float[] KNOTS_T = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
	
	/** Nurbs order along s parameter */
	private static final int ORDER_S = 2;
	
	/** Nurbs order along t parameter */
	private static final int ORDER_T = 3;
	
	/** Number of control points along s coordinate */
	private static final int CPOINT_SIZE_T = 3;
	
	
	/** Index of the modified part of the control points, ie first coordinate of the 4th point */
	private static final int CPOINT_MODIF_INDEX = 12;
	
	/**
	 * Control points to draw an arc.
	 * The fist three points are the origin whereas the 3 last are computed
	 * depending on the arc angle.
	 */
	private float[] controlPoints = {0.0f, 0.0f, 0.0f, 1.0f,
									 0.0f, 0.0f, 0.0f, 1.0f,
									 0.0f, 0.0f, 0.0f, 1.0f,
									 0.0f, 0.0f, 0.0f, 1.0f,
									 0.0f, 0.0f, 0.0f, 1.0f,
									 0.0f, 0.0f, 0.0f, 1.0f};
	
	/**
	 * NurbsArcLineTools
	 * @param center Vector3D
	 * @param semiMinorAxis Vector3D
	 * @param semiMajorAxis Vector3D
	 * @param startAngle double
	 * @param endAngle double
	 */
	protected NurbsArcFillTools(Vector3D center, Vector3D semiMinorAxis,
			Vector3D semiMajorAxis, double startAngle, double endAngle) {
		super(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle);
	}
	
	/**
	 * drawCircle
	 * @param gl current GL pipeline
	 */
	protected void drawCircle(GL gl) {
		double displayedAngle = 0.0;
		int nbQuarter = 0;
		GLU glu = new GLU();
		double angle = getSweepAngle();
		
		gl.glEnable(GL.GL_MAP2_VERTEX_4);
		
		GLUnurbs nurbsObj = NurbsArcGL.createNurbsDrawer(glu);
		
		// draw as many quarter circle as needed
		while (nbQuarter < NurbsArcGL.NB_QUARTER_MAX && displayedAngle < angle - NurbsArcGL.QUARTER_ANGLE) {
			drawArcPart(glu, nurbsObj, displayedAngle, NurbsArcGL.QUARTER_ANGLE);
			displayedAngle += NurbsArcGL.QUARTER_ANGLE;
			nbQuarter++;
		}
		
		// finish the ramining arc if the circle is not already complete
		if (nbQuarter < NurbsArcGL.NB_QUARTER_MAX) {
			drawArcPart(glu, nurbsObj, displayedAngle, angle - displayedAngle);
		}
		
		NurbsArcGL.destroyNurbsObj(glu, nurbsObj);
		
		gl.glDisable(GL.GL_MAP2_VERTEX_4);
	}
	
	/**
	 * Draw part af circle starting from the point of angle start angle to angular region.
	 * The arc is centered on the origin.
	 * @param glu current GLU object/
	 * @param nurbsObj nurbsObj used to draw
	 * @param startAngle angle of the begining of the arc.
	 * @param sweepAngle size of the arc segment in radian. Should be lower than Pi and gt 0.
	 */
	public void drawArcPart(GLU glu, GLUnurbs nurbsObj, double startAngle, double sweepAngle) {
		// the control points to draw the arc part (not partial disc).
		float[] arcControlPoints = NurbsArcGL.computeArcControlPoints4D(startAngle, sweepAngle);
		
		// set the last 3 control points with the computed ones.
		for (int i = 0; i < arcControlPoints.length; i++) {
			controlPoints[i + CPOINT_MODIF_INDEX] = arcControlPoints[i];
		}
		
		glu.gluBeginSurface(nurbsObj);
		glu.gluNurbsSurface(nurbsObj, KNOTS_S.length, KNOTS_S, KNOTS_T.length, KNOTS_T,
						    CPOINT_SIZE_T * NurbsArcGL.SIZE_4D, NurbsArcGL.SIZE_4D,
						    controlPoints, ORDER_S, ORDER_T, GL.GL_MAP2_VERTEX_4);
		glu.gluEndSurface(nurbsObj);
		
	}

}
