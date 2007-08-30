/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerGL.java                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw the      */
/*        outline of an arc object                                        */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.arcDrawing;



import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.drawers.LineDrawerGL;
import org.scilab.modules.renderer.gluNurbsWrapping.GLUnurbsObj;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class containing functions called by RectangleLineDrawerJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class ArcLineDrawerGL extends LineDrawerGL implements ArcDrawerStrategy {

	/** Angle for a quarter of a circle */
	private static final double QUARTER_ANGLE = Math.PI / 2.0;
	
	/** number of quarter in a circle */
	private static final int NB_QUARTER_MAX = 4;
	
	private NurbsArcGL nurbsTools;
	
	/**
	 * Default constructor
	 */
	public ArcLineDrawerGL() {
		super();
	}

	
	/**
	 * Create the display list for the arc
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
	protected void startRecordDL() {
		// No DL usable
	}
	
	/**
	 * End the recoding of a display list.
	 * Need to be called after a startRecordDL	
	 */
	protected void endRecordDL() {
		// No DL usable
	}
	
	/**
	 * Call gl routine to draw to outline of the arc.
	 */
	public void drawArc() {
		GL gl = getGL();
		gl.glEnable(GL.GL_MAP1_VERTEX_4);
		// set dash mode
		gl.glLineWidth(getThickness());
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
		
		// set color
		double[] color = getLineColor();
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
		nurbsObj = null;
        
        gl.glPopMatrix();
        
        GLTools.endDashMode(gl);
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
		double displayedAngle = 0.0;
		int nbQuarter = 0;
		
		while (nbQuarter < NB_QUARTER_MAX && displayedAngle < angle - QUARTER_ANGLE) {
			drawArcSegment(nurbsObj, displayedAngle, QUARTER_ANGLE);
			displayedAngle += QUARTER_ANGLE;
			nbQuarter++;
		}
		
		// finish the ramining arc if the circle is not already complete
		if (nbQuarter < NB_QUARTER_MAX) {
			drawArcSegment(nurbsObj, displayedAngle, angle - displayedAngle);
		}
	}
	
	/**
	 * Draw part af circle starting from the point of angle start angle to angular region.
	 * The arc is centered on the origin.
	 * @param nurbsObj nurbsObj used to draw
	 * @param startAngle angle of the begining of the arc.
	 * @param sweepAngle size of the arc segment in radian. Should be lower than Pi and gt 0.
	 */
	private void drawArcSegment(GLUnurbsObj nurbsObj, double startAngle, double sweepAngle) {
		float[] controlPoints = nurbsTools.computeArcControlPoints4D(startAngle, sweepAngle);
		nurbsObj.gluBeginCurve();
	    nurbsObj.gluNurbsCurve(NurbsArcGL.KNOTS.length, NurbsArcGL.KNOTS, NurbsArcGL.SIZE_4D,
	    					   controlPoints, NurbsArcGL.NURBS_ORDER, GL.GL_MAP1_VERTEX_4);
	    nurbsObj.gluEndCurve();
		
	}
	
}
