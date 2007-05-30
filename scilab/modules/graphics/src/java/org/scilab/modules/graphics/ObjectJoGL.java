/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Object which can be rendered in a GL pipeline                   */
/*------------------------------------------------------------------------*/

package org.scilab.modules.graphics;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

/**
 * Object which can be rendered in a GL pipeline
 * @author Jean-Baptiste Silvy
 */
public class ObjectJoGL {

	/** current context to draw in */
	private GL glPipeline;
	/** Glu instance to use glu functionalities */
	private GLU curGluInstance;

	/**
	 * Default constructor
	 */
	public ObjectJoGL() {
		glPipeline = null;
		curGluInstance = null;
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		// get the context from the drawing canvas
		updateGLContext(parentFigureIndex);
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() { }
	
	/**
	 * To avoid always creating new glu instance for the object,
	 * only one is created
	 * @return The current glu instance
	 */
	protected GLU getGlu() {
		if (curGluInstance == null) {
			curGluInstance = new GLU();
		}
		return curGluInstance;
	}
	
	/**
	 * Get the OpenGL context relative to the canvas figureIndex
	 * @param figureIndex index of the figure correspondingto the canvas
	 */
	protected void updateGLContext(int figureIndex) {
		glPipeline = FigureCanvasMapper.getCorrespondingCanvas(figureIndex).getGL();
	}
	
	/**
	 * Get the current GL context which must be used to draw
	 * @return current GL current pipeline
	 */
	protected GL getGL() {
		return glPipeline;
	}
	
}
