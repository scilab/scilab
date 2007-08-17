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
import org.scilab.modules.renderer.gluNurbsWrapping.GluNurbsConst;
import org.scilab.modules.renderer.utils.geom3D.Matrix4D;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class containing functions called by RectangleLineDrawerJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class ArcLineDrawerGL extends LineDrawerGL implements ArcDrawerStrategy {

	/** Number of controm points to desgn the curve */
	private static final int NB_CONTROL_POINTS = 9;
	/** Order of the nurbs, i.e. the nurbs will be polynoms of degree 3. */
	private static final int NURBS_ORDER = 3;
	/** Number of coordinates of a control point */
	private static final int CP_SIZE = 4;
	
	/** vector of knot. Size 12 = NB_CONTROL_POINTS + NURBS_ORDER */
	private static final float[] KNOT = {0.0f, 0.0f, 0.0f, 0.25f, 0.25f, 0.5f, 0.5f, 0.75f, 0.75f, 1.0f, 1.0f, 1.0f};
	
	/** Specify the weight to use control points */
	private static final float FACTOR = (float) Math.sqrt(2.0) / 2.0f;
	
	/** Maximal sampling tolerance */
	private static final float SAMPLING_TOLERANCE = 5.0f;
	
	/**
	 * Control points of the curve (x,y,z,w) where w is the weigth of the point
	 * This define an square around the circle.
	 */
	private static final float[] CONTROL_POINTS = {1.0f, 0.0f, 0.0f, 1.0f,
												   FACTOR, FACTOR, 0.0f, FACTOR,
												   0.0f, 1.0f, 0.0f, 1.0f,
												   -FACTOR, FACTOR, 0.0f, FACTOR,
												   -1.0f, 0.0f, 0.0f, 1.0f,
												   -FACTOR, -FACTOR, 0.0f, FACTOR,
												   0.0f, -1.0f, 0.0f, 1.0f,
												   FACTOR, -FACTOR, 0.0f, FACTOR,
												   1.0f, 0.0f, 0.0f, 1.0f};
	/** scale to transform th ellipse into a circle */
	private Vector3D scale;
	/** To put the ellipse in the right position */
	private Matrix4D rotationMatrix;
	/** center of the ellipse */
	private Vector3D center;
	
	/**
	 * Default constructor
	 */
	public ArcLineDrawerGL() {
		super();
		rotationMatrix = new Matrix4D();
		scale = new Vector3D();
		center = new Vector3D();
		//nurbsObj = null;
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
		
		center = new Vector3D(centerX, centerY, centerZ);
		Vector3D semiMinorAxis = new Vector3D(semiMinorAxisX, semiMinorAxisY, semiMinorAxisZ);
		Vector3D semiMajorAxis = new Vector3D(semiMajorAxisX, semiMajorAxisY, semiMajorAxisZ);
		Vector3D thirdVector = semiMajorAxis.crossProduct(semiMinorAxis);
		
		rotationMatrix.setFromRotatedBasis(semiMajorAxis, semiMinorAxis, thirdVector);
		
		// the arc is flat, Z coordinate does not matter.
		scale.setValues(semiMajorAxis.getNorm(), semiMinorAxis.getNorm() , 1.0);
		
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
		gl.glTranslated(center.getX(), center.getY(), center.getZ());
		gl.glMultMatrixd(rotationMatrix.getOpenGLRepresentation(), 0);
		gl.glScaled(scale.getX(), scale.getY(), scale.getZ());
		
		// display circle has a nurbs
		GLUnurbsObj nurbsObj = GLUnurbsObj.gluNewNurbsRenderer();
		nurbsObj.gluNurbsProperty(GluNurbsConst.GLU_CULLING, GL.GL_TRUE);
		nurbsObj.gluNurbsProperty(GluNurbsConst.GLU_SAMPLING_TOLERANCE, SAMPLING_TOLERANCE);

        nurbsObj.gluBeginCurve();
        nurbsObj.gluNurbsCurve(NB_CONTROL_POINTS + NURBS_ORDER, KNOT, CP_SIZE, CONTROL_POINTS, NURBS_ORDER, GL.GL_MAP1_VERTEX_4);
        nurbsObj.gluEndCurve();
        
		GLUnurbsObj.gluDeleteNurbsRenderer(nurbsObj);
		nurbsObj = null;
        
        gl.glPopMatrix();
        
        GLTools.endDashMode(gl);
	}
	
}
