/*------------------------------------------------------------------------*/
/* file: PolylineTesselatorCallback.java                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains glu tesselator callbacks to draw a filled polygon      */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.glu.GLU;
import javax.media.opengl.glu.GLUtessellatorCallbackAdapter;
import javax.media.opengl.GL;

/**
 * Contains glu tesselator callbacks to draw a filled polygon.
 * In the callback functions, the Object class stands for Vector3D
 * @author Jean-Baptiste Silvy
 */
public class PolylineTesselatorCallback extends GLUtessellatorCallbackAdapter {

	private GL currentGL;
	private GLU currentGLU;
	
	/**
	 * Default constructor
	 */
	public PolylineTesselatorCallback() {
		super();
	}
	
	/**
	 * Set the OpenGL to use for rendering.
	 * @param gl currentOpenGL pipeline
	 * @param glu current glu object
	 */
	public void setCurrentGL(GL gl, GLU glu) {
		currentGL = gl;
		currentGLU = glu;
	}
	
	/**
	 * @return OpenGL pipeline to use for this callback
	 */
	public GL getGL() {
		return currentGL;
	}
	
	/**
	 * The begin callback method is invoked like glBegin to indicate the start of a (triangle) primitive.
	 * @param type type of the primitives to use
	 */
	@Override
	public void begin(int type) {
		getGL().glBegin(type);
	}
	
	/**
	 * The combine callback method is called to create a new vertex when the tessellation detects an intersection,
	 * or wishes to merge features.
	 * @param coords Specifics the location of the new vertex (a vector3D)
     * @param data Specifics the vertices used to create the new vertex.
     * @param weight Specifics the weights used to create the new vertex.
     * @param outData Reference user the put the coodinates of the new vertex.
	 */
	@Override
	public void combine(double[] coords, Object[] data, float[] weight, Object[] outData) {
		outData[0] = coords;
	}
	
	/**
	 * The end callback serves the same purpose as glEnd.
	 */
	@Override
	public void end() {
		getGL().glEnd();
	}
	
	/**
	 * The vertex callback method is invoked between the begin and end callback methods.
	 * @param vertexData Specifics a reference to the vertices of the triangles created by the tessellatin process.
	 */
	@Override
	public void vertex(Object vertexData) {
		double[] vect = (double[]) vertexData;
		getGL().glVertex3dv(vect, 0);
	}
	
	/**
	 * The error callback method is called when an error is encountered.
	 * @param errnum Specifics the error number code.
	 */
	@Override
	public void error(int errnum) {
		try {
			System.err.println("Glu Error: " + currentGLU.gluErrorString(errnum));
		} catch (ArrayIndexOutOfBoundsException e) {
			System.err.println("Glu Error: Unknown glu Error number " + errnum);
		}
	}
}
