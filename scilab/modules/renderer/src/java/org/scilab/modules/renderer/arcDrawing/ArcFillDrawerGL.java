/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerGL.java                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw inside   */
/*        of an arc object                                                */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.arcDrawing;


import javax.media.opengl.GL;

import org.scilab.modules.renderer.drawers.FillDrawerGL;
import org.scilab.modules.renderer.gluNurbsWrapping.GLUnurbsObj;
import org.scilab.modules.renderer.gluNurbsWrapping.GluNurbsConst;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing the driver dependant routines to draw inside of an arc object.
 * @author Jean-Baptiste Silvy
 */
public class ArcFillDrawerGL extends FillDrawerGL implements ArcDrawerStrategy {

	/** Knots used to draw a square with nurbs of degree 2. */
	private static final float[] KNOTS_SQUARE = {-1.0f, -1.0f, 1.0f, 1.0f};
	
	/** Control points used to draw a square with nurbs of degree 2. */
	private static final float[] CP_POINTS_SQUARE = {-1.0f, -1.0f, 0.0f,
											         -1.0f, 1.0f, 0.0f,
											         1.0f, -1.0f, 0.0f,
											         1.0f, 1.0f, 0.0f };
	
	/** Angle for a quarter of a circle */
	private static final double QUARTER_ANGLE = Math.PI / 2.0;
	
	/** number of quarter in a circle */
	private static final int NB_QUARTER_MAX = 4;
	
	/** Size of closing picewise linear curve */
	private static final int PWL_LENGTH = 3;
	
	/** Algorithm to draw nurbs curves */
	private NurbsArcGL nurbsTools;
	
	/**
	 * Default constructor
	 */
	public ArcFillDrawerGL() {
		super();
	}
	
	/**
	 * quicly call the parent figure
	 * @param parentFigureIndex index of parent figure
	 */
	@Override
	public void show(int parentFigureIndex) {
		drawArc();
	}
	
	
	/**
	 * Create a new display list
	 * Each JoGL call will be saved in the DL
	 * until endRecordDL is called
	 */
	@Override
	protected void startRecordDL() {
		// No DL usable
	}
	
	/**
	 * End the recoding of a display list.
	 * Need to be called after a startRecordDL	
	 */
	@Override
	protected void endRecordDL() {
		// No DL usable
	}
	
	/**
	 * create the display list for the arc
	 * @param centerX X coordinate of the arc center
	 * @param centerY Y coordinate of the arc center
	 * @param centerZ Z coordinate of the arc center
	 * @param semiMinorAxisX X coordinate of the ellipse minor axis
	 * @param semiMinorAxisY Y coordinate of the ellipse minor axis
	 * @param semiMinorAxisZ Z coordinate of the ellipse minor axis
	 * @param semiMajorAxisX X coordinate of the ellipse major axis
	 * @param semiMajorAxisY Y coordinate of the ellipse minor axis
	 * @param semiMajorAxisZ Z coordinate of the ellipse minor axis
	 * @param startAngle lowest angle in radian, relative to the semi-major axis of the ellipse.
     *                   Start of the angular part to draw.
	 * @param endAngle highest angle in radian, relative to the semi-major axis of the ellipse.
     *                 End of the angular part to draw
	 */
	public void drawArc(double centerX, double centerY, double centerZ,
						double semiMinorAxisX, double semiMinorAxisY, double semiMinorAxisZ,
						double semiMajorAxisX, double semiMajorAxisY, double semiMajorAxisZ,
						double startAngle, double endAngle) {
		
		Vector3D center = new Vector3D(centerX, centerY, centerZ);
		Vector3D semiMinorAxis = new Vector3D(semiMinorAxisX, semiMinorAxisY, semiMinorAxisZ);
		Vector3D semiMajorAxis = new Vector3D(semiMajorAxisX, semiMajorAxisY, semiMajorAxisZ);
		
		nurbsTools = new NurbsArcGL(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle);
		
		drawArc();
	}
	
	/**
	 * Call gl routine to draw to the inside of the arc.
	 */
	public void drawArc() {
		
		GL gl = getGL();
		gl.glEnable(GL.GL_MAP1_VERTEX_4);
		
		// set color
		double[] color = getBackColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		// transform the ellipse so we can draw a circle
		gl.glPushMatrix();
		nurbsTools.setCoordinatesToCircleGL(gl);
		
		// display circle has a nurbs
		GLUnurbsObj nurbsObj = GLUnurbsObj.gluNewNurbsRenderer();
		nurbsTools.setGluProperties(nurbsObj);

        nurbsObj.gluBeginCurve();
        drawArc(nurbsObj, nurbsTools.getSweepAngle());
        nurbsObj.gluEndCurve();
        
		GLUnurbsObj.gluDeleteNurbsRenderer(nurbsObj);
        
        gl.glPopMatrix();
	}
	
	/**
	 * Draw an arc starting from the point (1,0) (ie angle = 0) to the angular region.
	 * The arc is centered on the origin. Unlike draw arc segment, this function can handle
	 * angles higher than Pi.
	 * @param nurbsObj nurbsObj used to draw
	 * @param angle size of the arc segment in radian. Should be positive.
	 */
	private void drawArc(GLUnurbsObj nurbsObj, double angle) {
		// We draw has many quarter of circle has needed, but at most 4 (more is useless)
		// Then we draw the remaining part
		
		nurbsObj.gluBeginSurface();
		
		// draw the square
		nurbsObj.gluNurbsSurface(KNOTS_SQUARE.length, KNOTS_SQUARE, KNOTS_SQUARE.length, KNOTS_SQUARE,
				                 NurbsArcGL.SIZE_2D * NurbsArcGL.SIZE_3D, NurbsArcGL.SIZE_3D,
				                 CP_POINTS_SQUARE, NurbsArcGL.SIZE_2D, NurbsArcGL.SIZE_2D, GL.GL_MAP2_VERTEX_3);
		// then cut an arc inside
		trimArc(nurbsObj, angle);
		
		nurbsObj.gluEndSurface();
	}
	
	/**
	 * Trim an arc inside a glu nurbs surface
	 * @param nurbsObj nurbsObj used to draw
	 * @param angle size of the arc segment in radian. Should be positive.
	 */
	private void trimArc(GLUnurbsObj nurbsObj, double angle) {
		double displayedAngle = 0.0;
		int nbQuarter = 0;
		// Cut part of a circle inside the square
		nurbsObj.gluBeginTrim();
		while (nbQuarter < NB_QUARTER_MAX && displayedAngle < angle - QUARTER_ANGLE) {
			//drawArcSegment(nurbsObj, displayedAngle, QUARTER_ANGLE);
			trimArcSegment(nurbsObj, displayedAngle, QUARTER_ANGLE);
			displayedAngle += QUARTER_ANGLE;
			nbQuarter++;
		}
		
		// finish the ramining arc if the circle is not already complete
		if (nbQuarter < NB_QUARTER_MAX) {
			// the arc is not a full circle
			trimArcSegment(nurbsObj, displayedAngle, angle - displayedAngle);
			
			// close the trimming region with a pwl curve
			float[] pwlPoints = {(float) Math.cos(angle), (float) Math.sin(angle), 0.0f, 0.0f, 1.0f, 0.0f };
			nurbsObj.gluPwlCurve(PWL_LENGTH, pwlPoints, NurbsArcGL.SIZE_2D, GluNurbsConst.GLU_MAP1_TRIM_2);
		}
		
		
		nurbsObj.gluEndTrim();
	}
	
	/**
	 * Draw part af circle starting from the point of angle start angle to angular region.
	 * The arc is centered on the origin. This is used to trim glu Nurbs surfaces
	 * @param nurbsObj nurbsObj used to draw
	 * @param startAngle angle of the begining of the arc.
	 * @param sweepAngle size of the arc segment in radian. Should be lower than Pi and gt 0.
	 */
	private void trimArcSegment(GLUnurbsObj nurbsObj, double startAngle, double sweepAngle) {
		float[] controlPoints = nurbsTools.computeArcControlPoints3D(startAngle, sweepAngle);
		nurbsObj.gluBeginCurve();
	    nurbsObj.gluNurbsCurve(NurbsArcGL.KNOTS.length, NurbsArcGL.KNOTS, NurbsArcGL.SIZE_3D,
	    					   controlPoints, NurbsArcGL.NURBS_ORDER, GluNurbsConst.GLU_MAP1_TRIM_3);
	    nurbsObj.gluEndCurve();
	}
		
	
}
