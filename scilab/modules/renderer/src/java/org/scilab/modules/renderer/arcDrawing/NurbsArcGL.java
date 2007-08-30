/*------------------------------------------------------------------------*/
/* file: NurbsArcGL.java                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class with utility functions to draw arcs                       */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.arcDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.gluNurbsWrapping.GLUnurbsObj;
import org.scilab.modules.renderer.gluNurbsWrapping.GluNurbsConst;
import org.scilab.modules.renderer.utils.geom3D.Matrix4D;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class with utility functions to draw arcs
 * @author Jean-Baptiste Silvy
 */
public class NurbsArcGL {

	/** Number of controm points to draw an circle arc*/
	public static final int NB_CONTROL_POINTS = 3;
	/** Order of the nurbs, i.e. the nurbs will be polynoms of degree 3. */
	public static final int NURBS_ORDER = 3;
	/** Number of coordinates of a control point */
	public static final int SIZE_4D = 4;
	/** Number of coordinates of a control point */
	public static final int SIZE_3D = 3;
	/** Number of coordinates of a control point */
	public static final int SIZE_2D = 2;
	
	/** vector of knot. Size 6 = NB_CONTROL_POINTS + NURBS_ORDER */
	public static final float[] KNOTS = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
	
	
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
	 * Set the parameters to draw the arc
	 * @param center coordinates of the arc center
	 * @param semiMinorAxis coordinates of the ellipse minor axis
	 * @param semiMajorAxis coordinates of the ellipse major axis
	 * @param startAngle owest angle in radian, relative to the semi-major axis of the ellipse.
     *                   Start of the angular part to draw.
	 * @param endAngle highest angle in radian, relative to the semi-major axis of the ellipse.
     *                 End of the angular part to draw
	 */
	public NurbsArcGL(Vector3D center, Vector3D semiMinorAxis, Vector3D semiMajorAxis, double startAngle, double endAngle) {
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
	 * @return angular region of the arc.
	 */
	public double getSweepAngle() {
		return angularRegion;
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
	 * Set the default properties for the nurbs obj.
	 * @param nurbsObj current nurbs object
	 */
	public void setGluProperties(GLUnurbsObj nurbsObj) {
		nurbsObj.gluNurbsProperty(GluNurbsConst.GLU_CULLING, GL.GL_TRUE);
		nurbsObj.gluNurbsProperty(GluNurbsConst.GLU_SAMPLING_TOLERANCE, SAMPLING_TOLERANCE);
	}
	
	/**
	 * Compute the control points to draw an arc (of radius 1) with glu nurbs in 4D (x,y,z,w).
	 * @param startAngle beginning angle of the arc
	 * @param sweepAngle angular size of the arc
	 * @return control points to use with gluNurbsCurve
	 */
	public float[] computeArcControlPoints4D(double startAngle, double sweepAngle) {
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
	public float[] computeArcControlPoints3D(double startAngle, double sweepAngle) {
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
	
}
