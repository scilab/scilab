/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerGL.java                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw inside   */
/*        of an arc object                                                */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.arcDrawing;


import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;
import javax.media.opengl.glu.GLUnurbs;

import org.scilab.modules.renderer.drawers.FillDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Matrix4D;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing the driver dependant routines to draw inside of an arc object.
 * @author Jean-Baptiste Silvy
 */
public class ArcFillDrawerGL extends FillDrawerGL implements ArcDrawerStrategy {

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
		GLU glu = new GLU();
		gl.glEnable(GL.GL_MAP2_VERTEX_4);
		
		// set color
		double[] color = getBackColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		Matrix4D matbef = new Matrix4D();
		matbef.setToCurrentOpenGLMatrix(gl);
		
		// transform the ellipse so we can draw a circle
		gl.glPushMatrix();
		nurbsTools.setCoordinatesToCircleGL(gl);
		
		// display circle has a nurbs
		GLUnurbs nurbsObj = glu.gluNewNurbsRenderer();
		nurbsTools.setGluProperties(glu, nurbsObj);
		
        drawArc(glu, nurbsObj, nurbsTools.getSweepAngle());
        
        //glu.gluDeleteNurbsRenderer(nurbsObj);
		nurbsObj = null;
        
		gl.glPopMatrix();
		
		gl.glDisable(GL.GL_MAP2_VERTEX_4);
		
		Matrix4D matend = new Matrix4D();
		matend.setToCurrentOpenGLMatrix(gl);
		//matend = matbef.substract(matend);
		//System.err.println("matbeg = " + matbef);
		//System.err.println("matend = " + matend);
	}
	
	/**
	 * Draw an arc starting from the point (1,0) (ie angle = 0) to the angular region.
	 * The arc is centered on the origin. Unlike draw arc segment, this function can handle
	 * angles higher than Pi.
	 * @param glu current glu object
	 * @param nurbsObj nurbsObj used to draw
	 * @param angle size of the arc segment in radian. Should be positive.
	 */
	private void drawArc(GLU glu, GLUnurbs nurbsObj, double angle) {
		nurbsTools.drawArc(this, glu, nurbsObj, angle);
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
		float[] arcControlPoints = nurbsTools.computeArcControlPoints4D(startAngle, sweepAngle);
		
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
