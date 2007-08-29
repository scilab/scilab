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

	/** Number of controm points to draw an circle arc*/
	private static final int NB_CONTROL_POINTS = 3;
	/** Order of the nurbs, i.e. the nurbs will be polynoms of degree 3. */
	private static final int NURBS_ORDER = 3;
	/** Number of coordinates of a control point */
	private static final int CP_SIZE = 4;
	
	/** vector of knot. Size 6 = NB_CONTROL_POINTS + NURBS_ORDER */
	private static final float[] KNOT = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

	/** Angle for a quarter of a circle */
	private static final double QUARTER_ANGLE = Math.PI / 2.0;
	
	/** number of quarter in a circle */
	private static final int NB_QUARTER_MAX = 4;
	
	/** Maximal sampling tolerance */
	private static final float SAMPLING_TOLERANCE = 5.0f;
	

	/** scale to transform th ellipse into a circle */
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
	 * Default constructor
	 */
	public ArcLineDrawerGL() {
		super();
		rotationMatrix = new Matrix4D();
		scale = new Vector3D();
		center = new Vector3D();
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
		Vector3D thirdVector = semiMinorAxis.crossProduct(semiMajorAxis).getNormalized();
		System.err.println("semiMinorAxis = " + semiMinorAxis);
		System.err.println("semiMajorAxis = " + semiMajorAxis);
		System.err.println("thirdVector = " + thirdVector);
		
		rotationMatrix.setFromRotatedBasis(semiMinorAxis, semiMajorAxis, thirdVector);
		
		System.err.println("rotationMatrix = " + rotationMatrix);
		
		// the arc is flat, Z coordinate does not matter.
		scale.setValues(semiMinorAxis.getNorm(), semiMajorAxis.getNorm() , 1.0);
		
		this.startAngle = startAngle;
		this.angularRegion = endAngle - startAngle;
		
		
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
		// Put the minimum angle has greatest axis, so the draw sart at Y = 0.
		gl.glRotated(Math.toDegrees(startAngle), 0.0, 0.0, 1.0);
		
		// display circle has a nurbs
		GLUnurbsObj nurbsObj = GLUnurbsObj.gluNewNurbsRenderer();
		nurbsObj.gluNurbsProperty(GluNurbsConst.GLU_CULLING, GL.GL_TRUE);
		nurbsObj.gluNurbsProperty(GluNurbsConst.GLU_SAMPLING_TOLERANCE, SAMPLING_TOLERANCE);

        nurbsObj.gluBeginCurve();
        drawArc(gl, nurbsObj, angularRegion);
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
	 * @param gl current OpenGL pipeline
	 * @param nurbsObj nurbsObj used to draw
	 * @param angle size of the arc segment in radian. Should be positive.
	 */
	private void drawArc(GL gl, GLUnurbsObj nurbsObj, double angle) {
		// We draw has many quarter of circle has needed, but at most 4 (more is useless)
		// Then we draw the remaining part
		double remainingAngle = angle;
		int nbQuarter = 0;
		
		while (nbQuarter < NB_QUARTER_MAX && remainingAngle >= QUARTER_ANGLE) {
			drawArcSegment(nurbsObj, QUARTER_ANGLE);
			gl.glRotated(Math.toDegrees(QUARTER_ANGLE), 0.0, 0.0, 1.0);
			remainingAngle -= QUARTER_ANGLE;
			nbQuarter++;
		}
		
		// finish the ramining arc if the circle is not complete
		if (nbQuarter < NB_QUARTER_MAX) {
			drawArcSegment(nurbsObj, remainingAngle);
		}
	}
	
	/**
	 * Draw part af circle starting from the point (1,0) (ie angle = 0) to angular region.
	 * The arc is centered on the origin.
	 * @param nurbsObj nurbsObj used to draw
	 * @param angle size of the arc segment in radian. Should be lower than Pi and gt 0.
	 */
	private void drawArcSegment(GLUnurbsObj nurbsObj, double angle) {
		// The control polygon is made of three points.
		// The first and last are the starting and end point of the arc (ie [1,0] and [cos(angle),sin(angle)]).
		// The middle point is such has the three forms an isocele triangle. Its two angles on the first and
		// last control points are equal to angle/2.
		
		// middle of the isocele triangle base
		Vector3D baseMiddle = new Vector3D((Math.cos(angle) + 1.0) / 2.0, Math.sin(angle) / 2.0, 0.0);
		
		// height of the isocele triangle bewteen the first and last control points.
		double height = Math.sin(angle / 2.0) * Math.tan(angle / 2.0);
		
		// the three points origin, baseMult and the middle control points are aligned
		// and cp = O + baseMiddle + height * dir, where dir is the direction between the three
		Vector3D controlPoint = baseMiddle.add(baseMiddle.getNormalized().scalarMult(height));
		
		
		// the wheight of the middle point is cos(angle / 2.0), so
		controlPoint = controlPoint.scalarMult(Math.cos(angle / 2.0));
		
		float[] controlPoints = {1.0f, 0.0f, 0.0f, 1.0f,
								 (float) controlPoint.getX(), (float) controlPoint.getY(),
								 (float) controlPoint.getZ(), (float) Math.cos(angle / 2.0),
								 (float) Math.cos(angle), (float) Math.sin(angle), 0.0f, 1.0f };
		
		nurbsObj.gluBeginCurve();
	    nurbsObj.gluNurbsCurve(NB_CONTROL_POINTS + NURBS_ORDER, KNOT, CP_SIZE, controlPoints, NURBS_ORDER, GL.GL_MAP1_VERTEX_4);
	    nurbsObj.gluEndCurve();
		
	}
	
}
