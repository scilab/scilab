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
	
	/** Mapping between scilab pixel drawing mode and OpenGL ones */
	private static final int[] LOGICAL_OPS = {GL.GL_CLEAR,
											  GL.GL_AND,
											  GL.GL_AND_REVERSE,
											  GL.GL_COPY,
											  GL.GL_AND_INVERTED,
											  GL.GL_NOOP,
											  GL.GL_XOR,
											  GL.GL_OR,
											  GL.GL_NOR,
											  GL.GL_EQUIV,
											  GL.GL_INVERT,
											  GL.GL_OR_REVERSE,
											  GL.GL_COPY_INVERTED,
											  GL.GL_OR_INVERTED,
											  GL.GL_NAND,
											  GL.GL_SET};
	
	/** Canvas to draw the figure */
	private RendererProperties guiProperties;
	/** store the figureIndex */
	private int figureId;
	/** To get all the objects which needs to be destroyed */
	private ObjectGLCleaner destroyedObjects;
	
	/** To know if the figure can be displayed */
	private boolean isReadyForRendering;
	
	private boolean pixmapModeOn;
	
	private String exportFileName;
	private int exportFileType;
	
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
      	pixmapModeOn = true;
      	exportFileName = null;
      	exportFileType = 0;
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
  	 * @param colorIndex index of the color to use
  	 */
  	public void setBackgroundColor(int colorIndex) {
  		double[] color = getColorMap().getColor(colorIndex);
		getGL().glClearColor((float) color[0], (float) color[1], (float) color[2], 1.0f); // alpha is set to 1
  	}
  	
  	
  	/**
     * Set the pixel drawing mode of the figure.
  	 * @param logicOpIndex Scilab value of pixel drawing mode
  	 */
  	public void setLogicalOp(int logicOpIndex) {
  		// convert to OpenGL value
  		getGL().glLogicOp(LOGICAL_OPS[logicOpIndex]);
  	}
  	
  	/**
  	 * Set all figure parameters in one time
  	 * @param backgroundColor index of the background color
  	 * @param logicOpIndex Scilab value of pixel drawing mode
  	 */
  	public void setFigureParameters(int backgroundColor, int logicOpIndex) {
  		setLogicalOp(logicOpIndex);
  		setBackgroundColor(backgroundColor);
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
		pixmapModeOn = onOrOff;
	}
	
	/**
	 * @return wether the pixmap mode is on ar off.
	 */
	public boolean getPixmapMode() {
		return pixmapModeOn;
	}
	
	/**
	 * Export function
	 * @param fileName name of the file in which the window will be rendered
	 * @param fileType type of the file (ie jpg, bmp, ...).
	 */
	public void exportToFile(String fileName, int fileType) {
		exportFileName = fileName;
		exportFileType = fileType; 
  		
	    //GLPbuffer pbuffer = GLDrawableFactory.getFactory().createGLPbuffer(new GLCapabilities(), null, 2000 , 2000, null);

		
		drawCanvas();		

		exportFileName = null;
		exportFileType = 0;
	}
	
	/**
	 * @return true if we need to export a graphic file, false otherwise
	 */
	public boolean isExportEnable() {
		return exportFileType != 0;
	}
	
	/**
	 * Export function
	 * @param fileName name of the file in which the window will be rendered
	 * @param fileType type of the file (ie jpg, bmp, ...).
	 */
  	public void exportToBitmapFile(String fileName, int fileType) {
  		GL gl = getGL();
  		// use the lastly modified buffer
  		gl.glReadBuffer(GL.GL_FRONT);
  		ExportToFile export = ExportToFile.createExporter(fileName, fileType);
  		export.setFileSize(getCanvasWidth(), getCanvasHeight());
  		export.exportToBitmap();
  		// back to defautl value
  		gl.glReadBuffer(GL.GL_BACK);
  	}

  	/**
  	 * @return name of the file to export
  	 */
	public String getExportFileName() {
		return exportFileName;
	}

	/**
	 * @return type of the file to export
	 */
	public int getExportFileType() {
		return exportFileType;
	}

}
