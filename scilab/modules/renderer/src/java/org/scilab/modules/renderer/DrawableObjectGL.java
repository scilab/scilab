/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Parent class to render objects                                  */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Parent class for all graphic classes corresponding to a cpp one
 * @author Jean-Baptiste Silvy
 */
public abstract class DrawableObjectGL extends ObjectGL {

	/** Index of the display list */
	private int dlIndex;
	
	/**
	 * Default constructor
	 */
	public DrawableObjectGL() {
		super();
		dlIndex = GLTools.UNINIT_DL_INDEX; // can't create the index outside the jogl thread.
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public abstract void show(int parentFigureIndex);
	
	/**
	 * To be called when the cpp object is destroyed
	 */
	public void clean() {
		updateGLContext();
		clearDisplayList();
	}
	
	/**
	 * Clear the display list of this object
	 */
	public void clearDisplayList() {
		// We need to be sure that the memory used by the display list is freed
		if (dlIndex != GLTools.UNINIT_DL_INDEX) {
			getGL().glDeleteLists(dlIndex, 1);
		}
		dlIndex = GLTools.UNINIT_DL_INDEX;
	}
	
	
	
	/**
	 * Create a new display list
	 * Each JoGL call will be saved in the DL
	 * until endRecordDL is called
	 */
	protected void startRecordDL() {
		if (dlIndex != GLTools.UNINIT_DL_INDEX) {
			getGL().glDeleteLists(dlIndex, 1);
		}
		dlIndex = getGL().glGenLists(1);
		getGL().glNewList(dlIndex, GL.GL_COMPILE_AND_EXECUTE);
	}
	
	/**
	 * End the recoding of a display list.
	 * Need to be called after a startRecordDL
	 */
	protected void endRecordDL() {
		getGL().glEndList();
	}
	
	/**
	 * Call the display list
	 */
	protected void displayDL() {
		getGL().glCallList(dlIndex);
	}
	
	/**
	 * @return true if the display list has already been created, false otherwise.
	 */
	protected synchronized boolean isDLInit() {
		return (dlIndex != GLTools.UNINIT_DL_INDEX);
	}
	
	/**
	 * @return index of the display lists for this object
	 */
	protected int getDlIndex() {
		return dlIndex;
	}
	
}
