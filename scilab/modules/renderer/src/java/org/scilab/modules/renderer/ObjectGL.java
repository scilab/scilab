/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Object which can be rendered in a GL pipeline 
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
import javax.media.opengl.Threading;
import javax.media.opengl.glu.GLU;

import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.TexturedColorMap;

/**
 * Object which can be rendered in a GL pipeline
 * @author Jean-Baptiste Silvy
 */
public class ObjectGL {

//	public static int nbObjectsCount = 0;
	
	/** Glu instance to use glu functionalities */
	private GLU curGluInstance;
	/** current context to draw in */
	private GL glPipeline;
	/** current colorMap to use */
	private TexturedColorMap curColorMap;
	
	private DrawableFigureGL parentFigureGL;

	/**
	 * Default constructor
	 */
	public ObjectGL() {
		curGluInstance = null;
	}
	
	/**
	 * Called when the object is destroyed from C code
	 * @param parentFigureIndex index of parent figure
	 */
	public void destroy(int parentFigureIndex) {
		// To fix bug 3785, only add the object if it as some OpenGL resources
		// to free. Java resources are freed thanks to the Garbage collector.
		// If for any reason, the figure couldn't show itself (if minimized or in drawlater mode
		// for example) objects won't use OGL resources since not displayed and won't
		// be stacked for ever.
		if (isUsingOGLResources()) {
			if (Threading.isOpenGLThread()) {
				// we can release openGL resources now
				clean(parentFigureIndex);
			} else {
				// schedule destroy on the OpenGL thread
				getParentFigureGL().getObjectCleaner().addObjectToDestroy(this);
			}
		}
	}
	
	/**
	 * Free all OpenGL resources used by this object.
	 * 
	 * @param parentFigureIndex index of parent figure
	 */
	public void clean(int parentFigureIndex) { }
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		updateParentFigure(parentFigureIndex);
		updateGLContext();
		updateColorMap();
	}
	
	/**
	 * Set the parent figure for this objects
	 * @param parentFigureIndex index of the parent figure.
	 */
	public void updateParentFigure(int parentFigureIndex) {
		parentFigureGL = FigureMapper.getCorrespondingFigure(parentFigureIndex);
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
	protected synchronized GLU getGlu() {
		if (curGluInstance == null) {
			curGluInstance = new GLU();
		}
		return curGluInstance;
	}
	
	/**
	 * Get the OpenGL context relative to the canvas figureIndex
	 * @return updated OpenGL pipeline
	 */
	protected GL updateGLContext() {
		//glPipeline = FigureMapper.getCorrespondingFigure(figureIndex).getGL();
		glPipeline = parentFigureGL.getGL();
		return glPipeline;
	}
	
	/**
	 * Get the current GL context which must be used to draw
	 * @return current GL pipeline
	 */
	protected GL getGL() {
		return glPipeline;
	}
	
	/**
	 * Get the colormap relative to figure with figureIndex
	 * @return updated colormap
	 */
	protected TexturedColorMap updateColorMap() {
		//curColorMap = FigureMapper.getCorrespondingFigure(figureIndex).getColorMap();
		curColorMap = parentFigureGL.getColorMap();
		return curColorMap;
	}
	
	/**
	 * Get the current colormap, ie the colormap of the parent canvas
	 * @return current colormap
	 */
	public TexturedColorMap getColorMap() {
		return curColorMap;
	}
	
	/**
	 * Set the colormap
	 * @param cmap new colormap
	 */
	protected void setColorMap(TexturedColorMap cmap) {
		curColorMap = cmap;
	}

	/**
	 * FigureGL getter
	 * @return figureGL
	 */
	public DrawableFigureGL getParentFigureGL() {
		return parentFigureGL;
	}

	/**
	 * figureGL setter
	 * @param figureGL DrawableFigureGL
	 */
	protected void setParentFigureGL(DrawableFigureGL figureGL) {
		this.parentFigureGL = figureGL;
	}
	
	/**
	 * @return current coordinate transformation
	 */
	public CoordinateTransformation getCoordinateTransformation() {
		return getParentFigureGL().getCoordinateTransformation();
	}
	
	/**
	 * This function is used to know if the object is using
	 * some OpenGL ressources that need to be released when the object is destroyed
	 * This function will be called from outside the OpenGL thread so should not contain any
	 * OpenGL call.
	 * @return true if the object contains such ressources, false otherwise
	 */
	public boolean isUsingOGLResources() {
		// by default return false
		return false;
	}
	
}
