/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.figureDrawing;


import javax.media.opengl.GL;


import org.scilab.modules.renderer.ObjectGL;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.ObjectGLCleaner;



/**
 * Class containing functions called by DrawableFigureJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableFigureGL extends ObjectGL {
	
	/** Canvas to draw the figure */
	private RendererProperties guiProperties;
	/** store the figureIndex */
	private int figureId;
	/** To get all the objects which needs to be destroyed */
	private ObjectGLCleaner destroyedObjects;
	
	/** To know if the figure can be displayed */
	private boolean isReadyForRendering;
	
	/**
	 * Default Constructor
	 */
	public DrawableFigureGL() {
		super();
      	guiProperties = null;
      	setColorMap(TexturedColorMap.create());
      	figureId = -1; // figure ids should be greater than 0.
      	destroyedObjects = new ObjectGLCleaner();
      	isReadyForRendering = false;
    }
	
	/**
	 * Set .OR. get if the rendering is enable for this figure.
	 * @param setMode if true act as a set function, if false act a get function
	 * @param isEnable when in set mode, specify the new value for rendering
	 * @return if in get mode return the current rendering value
	 */
	private synchronized boolean isRenderingEnable(boolean setMode, boolean isEnable) {
		if (setMode) {
			isReadyForRendering = isEnable;
		}
		return isReadyForRendering;
	}
	
	/**
	 * Specify a new state for the rendering enable mode
	 * @param isEnalbe if true figure can no be rendered
	 */
	public void setIsRenderingEnable(boolean isEnalbe) {
		isRenderingEnable(true, isEnalbe);
	}
	
	/**
	 * @return true if the figure can be rendered, false otherwise
	 */
	public boolean getIsRenderingEnable() {
		return isRenderingEnable(false, false);
	}
	
	/**
	 * Set figureId property
	 * @param figureId new figure Id
	 */
	public void setFigureIndex(int figureId) {
		if (FigureMapper.containsFigure(this)) {
			FigureMapper.removeMapping(this.figureId);
		}
		this.figureId = figureId;
		FigureMapper.addMapping(figureId, this);
		super.setFigureIndex(figureId);
	}
	
	/**
	 * Specify a new object to access canvas and other gui related properties.
	 * @param prop new object to access gui properties
	 */
	public void setRendererProperties(RendererProperties prop) {
		guiProperties = prop;
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		// destroy all the objects stored in the destroy list
		destroyedObjects.destroyAll(parentFigureIndex);
		
		super.initializeDrawing(parentFigureIndex);
		GL gl = getGL();
		gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
		gl.glClearDepth(1.0f);
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		super.endDrawing();
	}
  
	/**
   	 * Get the info message used by the figure
   	 * @return the info message
   	 */
	public String getInfoMessage() {
		return guiProperties.getInfoMessage();
	}
	
	/**
	 * Display the info message of the figure
	 * @param infoMessage string to display describing the figure
	 */
	public void setInfoMessage(String infoMessage) {
		guiProperties.setInfoMessage(infoMessage);
	}

	/**
	 * Force the display of the canvas
	 * Called from C to be sure to be in the right context
	 */
	public void drawCanvas() {
		guiProperties.forceDisplay();
	}

  	/**
  	 * Destroyr the rendering context
  	 */
  	public void closeRenderingCanvas() {
  		guiProperties = null;
   	}
  
  /**
   * Set a new colormap to the figure.
   * @param rgbMat matrix containing the new data. This is a nbColor x 3 matrix
   *               where 3 is the number of color in the colormap
   */
  	public void setColorMapData(double[] rgbMat) {
  		getColorMap().setData(rgbMat);
  	}
  	
  	/**
  	 * Get the raw data of the figure colormap
  	 * Should only be used by C code
  	 * @return colormap data
  	 */
  	public double[] getColorMapData() {
  		return getColorMap().getData();
  	}
  
  /**
   * Get the size of te colormap
   * @return number of color in the colorMap
   */
  public int getColorMapSize() {
    return getColorMap().getSize();
  }
  	
  	/**
  	 * Get the raw data of the figure colormap
  	 * Should only be used by C code
  	 * @param data matrix filled with the colormap data
  	 */
  	public void getColorMapData(double[] data) {
  		getColorMap().getData(data);
  	}
  	
  	/**
  	 * @return object used to destroy JoGL objects
  	 */
  	public ObjectGLCleaner getObjectCleaner() {
  		return destroyedObjects;
  	}
  	
  	/**
  	 * Get the canvas in which the figure is drawn
  	 * @return canvas in x=which the figur eis drawn or null if none exists
  	 */
  	public RendererProperties getRendererProperties() { return guiProperties; }
  	
  	/**
  	 * Set the background color of the figure
  	 * @param colorIndex index of the colro to use
  	 */
  	public void setBackgroundColor(int colorIndex) {
  		double[] color = getColorMap().getColor(colorIndex);
  		getGL().glClearColor((float) color[0], (float) color[1], (float) color[2], 1.0f); // alpha is set to 1
  		getGL().glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
  	}
  	
  	/**
	 * Called when the object is destroyed from C code
	 * @param parentFigureIndex index of parent figure
	 */
  	@Override
	public void destroy(int parentFigureIndex) {
		// figure should not be add to the object cleaner or will destroy themselves.
		// no operation for now
  		FigureMapper.removeMapping(figureId);
	}
	
	
	/**
	 * @return width of the rendering canvas
	 */
	public int getCanvasWidth() {
		return guiProperties.getCanvasWidth();
	}
	
	/**
	 * @return height of the rendering canvas
	 */
	public int getCanvasHeight() {
		return guiProperties.getCanvasHeight();
	}
	
	/**
	 * Set the rendering canvas size.
	 * @param width new width in pixels
	 * @param height new height in pixels
	 */
	public void setCanvasSize(int width, int height) {
		guiProperties.setCanvasSize(width, height);
	}
	
	/**
	 * @return width of the rendering window
	 */
	public int getWindowWidth() {
		return guiProperties.getWindowWidth();
	}
	
	/**
	 * @return height of the rendering window
	 */
	public int getWindowHeight() {
		return guiProperties.getWindowHeight();
	}
	
	/**
	 * Set the rendering window size.
	 * @param width new width in pixels
	 * @param height new height in pixels
	 */
	public void setWindowSize(int width, int height) {
		guiProperties.setWindowSize(width, height);
	}
	
	/**
	 * Get the position of the window enclosing the figure
	 * @return X coordinate in pixels of the window
	 */
	public int getWindowPosX() {
		return guiProperties.getWindowPosX();
	}
	
	/**
	 * Get the position of the window enclosing the figure
	 * @return Y coordinate in pixels of the window
	 */
	public int getWindowPosY() {
		return guiProperties.getWindowPosY();
	}
	
	/**
	 * Get the position of the window enclosing the figure
	 * @param posX X coordinate in pixels of the window
	 * @param posY Y coordinate in pixels of the window
	 */
	public void setWindowPosition(int posX, int posY) {
		guiProperties.setWindowPosition(posX, posY);
	}
	
	/**
	 * Get the current GL pipeline associated with the rendering context
	 * @return current GL pipeline
	 */
	@Override
	public GL getGL() {
		return guiProperties.getGLPipeline();
	}
	
	
	/**
	 * @param onOrOff specify if we set the pixmap on or off
	 */
	public void setPixmapMode(boolean onOrOff) {
		
	}
	
	/**
	 * @return wether the pixmap mode is on ar off.
	 */
	public boolean getPixmapModeB() {
		return false;
	}
	
	/**
	 * @param onOrOff if 0 set pixmap mode to false, otherwise set it to true.
	 */
	public void setPixmapMode(int onOrOff) {
		if (onOrOff == 0) {
			setPixmapMode(false);
		} else {
			setPixmapMode(true);
		}
	}
	
	/**
	 * @return 0 if pixmap mode is false, 1 otherwise.
	 */
	public int getPixmapMode() {
		if (getPixmapModeB()) {
			return 1;
		} else {
			return 0;
		}
	}
	
  	
}
