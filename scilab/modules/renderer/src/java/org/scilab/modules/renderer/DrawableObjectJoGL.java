/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Parent class to render objects                                  */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer;

import javax.media.opengl.GL;

/**
 * Parent class for all graphic classes corresponding to a cpp one
 * @author Jean-Baptiste Silvy
 */
public abstract class DrawableObjectJoGL extends ObjectJoGL {

	/** display lists index is always different than 0 */
	private static final int UNINIT_DL_INDEX = 0;
	/** Index of the display list */
	private int dlIndex;
	
	/**
	 * Default constructor
	 */
	public DrawableObjectJoGL() {
		super();
		dlIndex = UNINIT_DL_INDEX; // can't create the index outside the jogl thread.
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		// the display list should already have been created.
		if (dlIndex != UNINIT_DL_INDEX) {
			super.initializeDrawing(parentFigureIndex);
			getGL().glCallList(dlIndex);
			super.endDrawing();
		}
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		// get the context from the drawing canvas
		super.initializeDrawing(parentFigureIndex);
		startRecordDL();
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		endRecordDL();
		super.endDrawing();
	}
	
	/**
	 * To be called when the cpp object is destroyed
	 */
	public void destroy() {
		clearDisplayList();
	}
	
	/**
	 * Clear the display list of this object
	 */
	public void clearDisplayList() {
		// We need to be sure that the memory used by the display list is freed
		if (dlIndex != UNINIT_DL_INDEX) {
			getGL().glDeleteLists(dlIndex, 1);
		}
		dlIndex = UNINIT_DL_INDEX;
	}
	
	
	
	/**
	 * Create a new display list
	 * Each JoGL call will be saved in the DL
	 * until endRecordDL is called
	 */
	protected void startRecordDL() {
		if (dlIndex != UNINIT_DL_INDEX) {
			getGL().glDeleteLists(dlIndex, 1);
		}
		dlIndex = getGL().glGenLists(1);
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
