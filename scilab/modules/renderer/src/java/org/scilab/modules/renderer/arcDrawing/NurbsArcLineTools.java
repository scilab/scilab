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
import javax.media.opengl.glu.GLU;
import javax.media.opengl.glu.GLUnurbs;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * NurbsArcLineTools
 * @author Sylvestre Koumar
 *
 */
public class NurbsArcLineTools extends ArcLineTools {

	
	/** vector of knot. Size 6 = NB_CONTROL_POINTS + NURBS_ORDER */
	protected static final float[] KNOTS = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
	
	/** Order of the nurbs, i.e. the nurbs will be polynoms of degree 3. */
	protected static final int NURBS_ORDER = 3;
	
	/**
	 * NurbsArcLineTools
	 * @param center Vector3D
	 * @param semiMinorAxis Vector3D
	 * @param semiMajorAxis Vector3D
	 * @param startAngle double
	 * @param endAngle double
	 */
	protected NurbsArcLineTools(Vector3D center, Vector3D semiMinorAxis,
			Vector3D semiMajorAxis, double startAngle, double endAngle) {
		super(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle);
	}
	
	/**
	 * drawCircle
	 * @param gl current GL pipeline
	 */
	public void drawCircle(GL gl) {		
		double displayedAngle = 0.0;
		int nbQuarter = 0;
		GLU glu = new GLU();
		double angle = getSweepAngle();
		
		gl.glEnable(GL.GL_MAP1_VERTEX_4);
		
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
		
		gl.glDisable(GL.GL_MAP1_VERTEX_4);
		
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
		float[] controlPoints = NurbsArcGL.computeArcControlPoints4D(startAngle, sweepAngle);
		glu.gluBeginCurve(nurbsObj);
		glu.gluNurbsCurve(nurbsObj, KNOTS.length, KNOTS, NurbsArcGL.SIZE_4D,
				          controlPoints, NURBS_ORDER, GL.GL_MAP1_VERTEX_4);
	    glu.gluEndCurve(nurbsObj);
		
	}
	
	

}
