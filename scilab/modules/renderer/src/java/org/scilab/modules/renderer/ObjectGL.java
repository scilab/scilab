/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Object which can be rendered in a GL pipeline                   */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.utils.TexturedColorMap;

/**
 * Object which can be rendered in a GL pipeline
 * @author Jean-Baptiste Silvy
 */
public abstract class ObjectGL {

	/** Glu instance to use glu functionalities */
	private GLU curGluInstance = new GLU();
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
	 * Specify index of parent figure to get all interesting data.
	 * @param parentFigureIndex index of parentFigure
	 */
	public void setFigureIndex(int parentFigureIndex) {
		// get the context from the drawing canvas
		parentFigureGL = FigureMapper.getCorrespondingFigure(parentFigureIndex);
		updateColorMap();
	}
	
	/**
	 * Called when the object is destroyed from C code
	 * @param parentFigureIndex index of parent figure
	 */
	public void destroy(int parentFigureIndex) {
		FigureMapper.getCorrespondingFigure(parentFigureIndex).getObjectCleaner().addObjectToDestroy(this);
	}
	
	/**
	 * Free all things used by this object
	 * @param parentFigureIndex index of parent figure
	 */
	public void clean(int parentFigureIndex) { }
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		parentFigureGL = FigureMapper.getCorrespondingFigure(parentFigureIndex);
		updateGLContext();
		updateColorMap();
		/*updateGLContext(parentFigureIndex);
		updateColorMap(parentFigureIndex);*/
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
	
}
