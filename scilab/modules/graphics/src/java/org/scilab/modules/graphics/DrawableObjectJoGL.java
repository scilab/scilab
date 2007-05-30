/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Parent class to render objects                                  */
/*------------------------------------------------------------------------*/

package org.scilab.modules.graphics;

import javax.media.opengl.GL;

/**
 * Parent class for all graphic classes corresponding to a cpp one
 * @author silvy
 */
public abstract class DrawableObjectJoGL extends ObjectJoGL {

	/** Index of the display list */
	private int dlIndex;
	/** display lists index is always different than 0 */
	private final int uninitDlIndex = 0;
	
	/**
	 * Default constructor
	 */
	public DrawableObjectJoGL() {
		dlIndex        = uninitDlIndex; // can't create the index outside the jogl thread.
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		// the display list should already have been created.
		updateGLContext(parentFigureIndex);
		getGL().glCallList(dlIndex);
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		// get the context from the drawing canvas
		super.initializeDrawing(parentFigureIndex);
		//startRecordDL();
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		super.endDrawing();
		//endRecordDL();
	}
	
	/**
	 * To be called when the cpp object is destroyed
	 */
	public void destroy() {
		// We need to be sure that the memory used by the display list is freed
		if (dlIndex != uninitDlIndex) {
			getGL().glDeleteLists(dlIndex, 1);
		}
		dlIndex = uninitDlIndex;
	}
	
	
	
	/**
	 * Create a new display list
	 * Each JoGL call will be saved in the DL
	 * until endRecordDL is called
	 */
	protected void startRecordDL() {
		if (dlIndex == uninitDlIndex) {
			// create a new number
			dlIndex = getGL().glGenLists(1);
		} else {
			// the number can be reused.
			getGL().glDeleteLists(dlIndex, 1);
		}
		getGL().glNewList(dlIndex, GL.GL_COMPILE);
	}
	
	/**
	 * End the recoding of a display list.
	 * Need to be called after a startRecordDL
	 */
	protected void endRecordDL() {
		getGL().glEndList();
	}
	
}
