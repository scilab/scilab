/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class to render objects 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;
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
	 * @param parentFigureIndex index of parent figure
	 */
	public void clean(int parentFigureIndex) {
		super.clean(parentFigureIndex);
		setFigureIndex(parentFigureIndex);
		updateGLContext();
		clearDisplayList();
	}
	
	/**
	 * Clear the display list of this object
	 */
	public void clearDisplayList() {
		// We need to be sure that the memory used by the display list is freed
        if (getGL().glIsList(dlIndex)) {
        	getGL().glDeleteLists(dlIndex, 1);
 		}
 		dlIndex = GLTools.UNINIT_DL_INDEX;
	}
	
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		super.endDrawing();
	}
	
	/**
	 * Translate the object
	 * @param tx x displacement
	 * @param ty y displacement
	 * @param tz z displacement
	 */
	public void translate(double tx, double ty, double tz) {
		getGL().glPushMatrix();
		getGL().glTranslated(tx, ty, tz);
		getCoordinateTransformation().setAdditionalTranslation(new Vector3D(tx, ty, tz));
	}
	
	/**
	 * Finish the translation of the object
	 */
	public void endTranslate() {
		getGL().glPopMatrix();
		getCoordinateTransformation().setAdditionalTranslation(null);
	}
	
	
	/**
	 * Create a new display list
	 * Each JoGL call will be saved in the DL
	 * until endRecordDL is called
	 */
	protected void startRecordDL() {
		if (getGL().glIsList(dlIndex)) {
			getGL().glDeleteLists(dlIndex, 1);
		}
		dlIndex = getGL().glGenLists(1);
		// as advised in OpenGL web site, don't use GL_COMPILE_AND_EXECUTE
		// but instead compile and then call the display list.
		getGL().glNewList(dlIndex, GL.GL_COMPILE/*_AND_EXECUTE*/);
	}
	
	/**
	 * End the recoding of a display list.
	 * Need to be called after a startRecordDL
	 */
	protected void endRecordDL() {
		// finish recording
		getGL().glEndList();

		// draw the recorded display list
		displayDL();
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
		return (getGL().glIsList(dlIndex));
	}
	
	/**
	 * @return index of the display lists for this object
	 */
	protected int getDlIndex() {
		return dlIndex;
	}
	
}
