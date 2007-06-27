/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Object which can be rendered in a GL pipeline                   */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

import org.scilab.modules.renderer.utils.ColorMap;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureJoGL;
import org.scilab.modules.renderer.figureDrawing.FigureCanvas;

/**
 * Object which can be rendered in a GL pipeline
 * @author Jean-Baptiste Silvy
 */
public abstract class ObjectJoGL {

	/** Glu instance to use glu functionalities */
	private GLU curGluInstance = new GLU();
	/** current context to draw in */
	private GL glPipeline;
	/** current colorMap to use */
	private ColorMap curColorMap;

	/**
	 * Default constructor
	 */
	public ObjectJoGL() {
		curGluInstance = null;
	}
	
	/**
	 * Specify index of parent figure to get all interesting data.
	 * @param parentFigureIndex index of parentFigure
	 */
	public void setFigureIndex(int parentFigureIndex) {
		updateColorMap(parentFigureIndex);
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
		// get the context from the drawing canvas
		updateGLContext(parentFigureIndex);
		updateColorMap(parentFigureIndex);
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
	 * @param figureIndex index of the figure correspondingto the canvas
	 * @return updated OpenGL pipeline
	 */
	protected GL updateGLContext(int figureIndex) {
		DrawableFigureJoGL figure = FigureMapper.getCorrespondingFigure(figureIndex);
		FigureCanvas canvas = figure.getCanvas();
		glPipeline = canvas.getGL();
		return glPipeline;
	}
	
	/**
	 * Get the current GL context which must be used to draw
	 * @return current GL current pipeline
	 */
	protected GL getGL() {
		return glPipeline;
	}
	
	/**
	 * Get the colormap relative to figure with figureIndex
	 * @param figureIndex index of the figure correspondingto the canvas
	 * @return updated colormap
	 */
	protected ColorMap updateColorMap(int figureIndex) {
		curColorMap = FigureMapper.getCorrespondingFigure(figureIndex).getColorMap();
		return curColorMap;
	}
	
	/**
	 * Get the current colormap, ie the colormap of the parent canvas
	 * @return current colormap
	 */
	protected ColorMap getColorMap() {
		return curColorMap;
	}
	
	/**
	 * Set the colormap
	 * @param cmap new colormap
	 */
	protected void setColorMap(ColorMap cmap) {
		curColorMap = cmap;
	}
	
}
