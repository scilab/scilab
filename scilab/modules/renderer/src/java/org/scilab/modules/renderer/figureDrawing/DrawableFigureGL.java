/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Class containing the driver dependant routines to draw a 
 * figure object with JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.figureDrawing;

import java.lang.reflect.InvocationTargetException;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.swing.SwingUtilities;

import org.scilab.modules.renderer.ObjectGL;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.arcDrawing.ArcRendererFactory;
import org.scilab.modules.renderer.arcDrawing.NurbsArcRendererFactory;
import org.scilab.modules.renderer.polylineDrawing.JOGLShadeFacetDrawer;
import org.scilab.modules.renderer.polylineDrawing.ShadeFacetDrawer;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.glTools.ClipPlane3DManager;
import org.scilab.modules.renderer.utils.selection.RubberBox;
import org.scilab.modules.renderer.utils.textRendering.JOGLTextRendererFactory;
import org.scilab.modules.renderer.utils.textRendering.TextRendererFactory;
import org.scilab.modules.renderer.utils.textRendering.TextRendererManager;
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
	
	/** Keep a pointer on the OpenGL rendering target (Canvas, pBuffer, ...). */
	private GLAutoDrawable renderingTarget;

	/** Default ArcRenderer */
	private ArcRendererFactory arcRendererFactory;
	
	/** index of the background color */
	private int backGroundColorIndex;
	
	/** Rubber box used when in rubber box mode */
	private RubberBox rubberBox;
	
	/** Choose the type of polygon's paint */
	private ShadeFacetDrawer shadeFacetDrawer;
	
	private TextRendererManager textRendererCreator;
	
	private ClipPlane3DManager clipPlaneManager;
	
	/** To know if the rendering is requested by Scilab or not */
	private boolean renderRequested;
	
	/** Coordinate transfomrations used to draw this figure */
	private CoordinateTransformation transform;
	
	/** Number of subwins */
	private int nbSubwins;
	
	/**
	 * Default Constructor
	 */
	public DrawableFigureGL() {
		super();
      	guiProperties = null;
      	setColorMap(TexturedColorMap.create());
      	figureId = -1; // figure ids should be greater than 0.
      	destroyedObjects = new ObjectGLCleaner();
      	renderingTarget = null;
      	textRendererCreator = new TextRendererManager();
      	setDefaultTextRenderer();
      	setDefaultArcRendererFactory();
      	setDefaultShadeFacetDrawer();
      	backGroundColorIndex = 0;
      	rubberBox = null;
      	renderRequested = false;
      	transform = new CoordinateTransformation();
      	clipPlaneManager = new ClipPlane3DManager();
      	nbSubwins = 0;
    }
	
	/**
	 * Set the default TextRenderer
	 */
	public void setDefaultTextRenderer() {
		this.setTextRendererFactory(new JOGLTextRendererFactory());
	}
	
	/**
	 * Set the default ArcRenderer Factory
	 */
	public void setDefaultArcRendererFactory() {
		arcRendererFactory = new NurbsArcRendererFactory();
	}
	
	/**
	 * Set the default ShadeFacetDrawer
	 */
	public void setDefaultShadeFacetDrawer() {
		shadeFacetDrawer = new JOGLShadeFacetDrawer();
	}
	
	/**
	 * @return current coordinate transformation
	 */
	public CoordinateTransformation getCoordinateTransformation() {
		return transform;
	}
	
	/**
	 * @return Object use to manage clip planes
	 */
	public ClipPlane3DManager getClipPlaneManager() {
		return clipPlaneManager;
	}
	
	/**
	 * Set figureId property
	 * @param figureId new figure Id
	 */
	public void setFigureIndex(int figureId) {
		// a chack will be performed to see if the figure is alraedy here
		FigureMapper.addMapping(figureId, this);
		this.figureId = figureId;
	}
	
	/**
	 * @return Id of the fugure
	 */
	public int getFigureId() {
		return figureId;
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
		// don't clear the color buffer as usual, we may need to keep it
		// depending on the pixel mode
		gl.glClear(GL.GL_DEPTH_BUFFER_BIT);
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
   	 * Get the title of the figure
   	 * @return the title
   	 */
	public String getTitle() {
		return guiProperties.getTitle();
	}
	
	/**
	 * Set the title of the figure
	 * @param title the title of the figure
	 */
	public void setTitle(String title) {
		guiProperties.setTitle(title);
	}

	/**
	 * Force the display of the canvas
	 * Called from C to be sure to be in the right context
	 */
	public void drawCanvas() {
		setRenderingRequested(true);
		guiProperties.forceDisplay();
	}
	
	/**
	 * @param nbSubwins number of subwins among figure children
	 */
	public void setNbSubwins(int nbSubwins) {
		this.nbSubwins = nbSubwins;
	}
	
	/**
	 * @return number of subwinws under the figure.
	 */
	public int getNbSubwins() {
		return nbSubwins;
	}
	
	/**
	 * @return true if Scilab requested a redraw, false otherwise
	 */
	public synchronized boolean getRenderingRequested() {
		return renderRequested;
	}
	
	/**
	 * Specify that scilab has performed some modifications and
	 * A redraw of the graphic window must be performed
	 * @param requested if true, next call to the display function will redraw the scene
	 */
	public synchronized void setRenderingRequested(boolean requested) {
		this.renderRequested = requested;
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
  	 * Set the background of the canvas
  	 * @param colorIndex color index in the colormap
  	 */
  	public void setBackgroundColor(int colorIndex) {
  		backGroundColorIndex = colorIndex;
  		double[] color = getBackgroundColor();
  		getRendererProperties().setBackgroundColor(color[0], color[1], color[2]);
  	}
  	
  	/**
  	 * @return 3 channels of the background color
  	 */
  	public double[] getBackgroundColor() {
  		return getColorMap().getColor(backGroundColorIndex);
  	}
  	
  	/**
  	 * Set the background color of the figure
  	 */
  	public void drawBackground() {
  		GL gl = getGL();
  		double[] color = getBackgroundColor();
  		// not really needed actually
  		gl.glClearColor((float) color[0], (float) color[1], (float) color[2], 1.0f); // alpha is set to 1
  		gl.glClear(GL.GL_COLOR_BUFFER_BIT);
  	}
  	
  	
  	/**
     * Set the pixel drawing mode of the figure.
  	 * @param logicOpIndex Scilab value of pixel drawing mode
  	 */
  	public void setLogicalOp(int logicOpIndex) {
  		// convert to OpenGL value
  		GL gl = getGL();
		gl.glEnable(GL.GL_COLOR_LOGIC_OP); // to use pixel drawing mode
		gl.glLogicOp(LOGICAL_OPS[logicOpIndex]);
  	}
  	
  	/**
	 * Called when the object is destroyed from C code.
	 * @param parentFigureIndex index of parent figure
	 */
  	@Override
	public void destroy(int parentFigureIndex) {
  		getRendererProperties().disableCanvas();
  		setRenderingRequested(false);
  		// destroy all the objects stored in the destroy list
		destroyedObjects = null;
  		FigureMapper.removeMapping(figureId);
  	
  		// blocking call. So graphic synchrnonization must be desactivated here.
  		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				public void run() {
					getRendererProperties().closeCanvas();
				}
			});
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.printStackTrace();
		}
  		
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
	 * @return indicates if the size could be successfully modified
	 */
	public int setCanvasSize(int width, int height) {
		return guiProperties.setCanvasSize(width, height);
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
     * Specify wether the canvas should fit the parent tab size
     * (and consequently the scrollpane size) or not
     * @param onOrOff true to enable autoresize mode
     */
    public void setAutoResizeMode(boolean onOrOff) {
    	getRendererProperties().setAutoResizeMode(onOrOff);
    }
    
    /**
     * @return wether the resize mode is on or off
     */
    public boolean getAutoResizeMode() {
    	return getRendererProperties().getAutoResizeMode();
    }
    
    /**
	 * Get the part of the canvas which is currently viewed
	 * @return [x,y,w,h] array
	 */
   public int[] getViewport() {
	   return getRendererProperties().getViewport();
   }
   
   /**
	 * Specify a new viewport for the canvas
	 * For SwingScilabCanvas viewport can not be modified
	 * since it match the parent tab size
	 * @param posX X coordinate of upper left point of the viewport within the canvas
	 * @param posY Y coordinate of upper left point of the viewport within the canvas
	 * @param width width of the viewport
	 * @param height height of the viewport
	 */
	public void setViewport(int posX, int posY, int width, int height) {
		getRendererProperties().setViewport(posX, posY, width, height);
	}
	
	/**
	 * Specify the default target on which the figure is rendered.
	 * @param target pointer on the target
	 */
	public void setRenderingTarget(GLAutoDrawable target) {
		this.renderingTarget = target;
	}
	
	/**
	 * @return Default target on which the figure is rendered.
	 */
	public GLAutoDrawable getRenderingTarget() {
		return renderingTarget;
	}
	
	/**
	 * setTextRendererFactory
	 * @param textRendererFactory TextRendererFactory
	 */
	public void setTextRendererFactory(TextRendererFactory textRendererFactory) {
		textRendererCreator.setTextRendererFactory(textRendererFactory);
	}
	
	/**
	 * setArcRendererFactory
	 * @param arcRendererFactory ArcRendererFactory
	 */
	public void setArcRendererFactory(ArcRendererFactory arcRendererFactory) {
		this.arcRendererFactory = arcRendererFactory;
	}
	
	/**
	 * getArcRendererFactory
	 * @return ArcRendererFactory
	 */
	public ArcRendererFactory getArcRendererFactory() {
		return arcRendererFactory;
		
	}
	
	/**
	 * Get the type of the facet we have decomposed on a triangle
	 * @return shadeFacetDrawer type of facet (JOGL or GL2PS)
	 */
	public ShadeFacetDrawer getShadeFacetDrawer() {
		return shadeFacetDrawer;
	}

	/**
	 * Display facets that we decompose on a triangle
	 * there is 2 type of facets(JOGL & GL2PS)
	 * @param shadeFacetDrawer type of facet (JOGL or GL2PS)
	 */
	public void setShadeFacetDrawer(ShadeFacetDrawer shadeFacetDrawer) {
		this.shadeFacetDrawer = shadeFacetDrawer;
	}
	
	/**
	 * Switch to rubber box mode by specifying a rubber box to use
	 * @param rb rubber box to use
	 */
	public synchronized void setRubberBox(RubberBox rb) {
		this.rubberBox = rb;
	}
	
	/**
	 * Desactivate rubber box mode
	 */
	public synchronized void removeRubberBox() {
		this.rubberBox = null;
	}
	
	/**
	 * Get the rubber box currently used in the figure if one exists
	 * @return the rubber box used by the figure or null if none exists
	 */
	public synchronized RubberBox getRubberBox() {
		return this.rubberBox;
	}
	
	/**
	 * @return Wether the rubber box mod eis on
	 */
	public synchronized boolean isRubberBoxModeOn() {
		return this.rubberBox != null;
	}
	
	/**
	 * Create an interactive selection rectangle and return its pixel coordinates
	 * @param isClick specify wether the rubber box is selected by one click for each one of the two edge
	 *                or a sequence of press-release
	 * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
	 * @param initialRect if not null specify the initial rectangle to draw
	 * @return array of size 5 [x1, y1, x2, y2, button] where (x1,y1) is one of the rectangle corners
	 *         (x2,y2) the coordinates of the opposite rectangle in pixel anf button the Scilab code
	 *         of the mouse button.
	 */
	public int[] rubberBox(boolean isClick, boolean isZoom, int[] initialRect) {
		
		// giws is unable to pass null pointers
		// we found a 0 sized array instead
		int[] realIntialRect;
		if (initialRect == null || initialRect.length == 0) {
			realIntialRect = null;
		} else {
			realIntialRect = initialRect;
		}
		
		int[] res = {0, 0, 0, 0, 0};
		
		// get [x1,y1,x2,y2] in res and button
		int button = getRendererProperties().rubberBox(isClick, isZoom, realIntialRect, res);
		res[res.length - 1] = button;
		return res;
	}
	
	/**
	 * Get current displacement in the graphic window, to be used for axes rotation.
	 * @return array [dx, dy, stop] where [dx, dy] is the mouse displacement in pixels
	 *         or the position of the mouse with the first call. And stop is 0 if the
	 *         tracking is finished, 1 otherwise.
	 */
	public int[] getRotationDisplacement() {
		int[] res = new int[2 + 1];
		
		// get [dx, dy] displacement
		boolean stop = getRendererProperties().getRotationDisplacement(res);
		
		if (stop) {
			res[2] = 1;
		} else {
			res[2] = 0;
		}
		
		return res;
	}
	
	/**
	 * If a rotation displacement is recording, cancel it.
	 */
	public void stopRotationRecording() {
		getRendererProperties().stopRotationRecording();
	}
	
	/**
	 * @return Instance of the object which create textRenderer.
	 */
	public TextRendererManager getTextRendererCreator() {
		return textRendererCreator;
	}
	
	/**
	 * Put the figure in top of other windows
	 */
	public void showWindow() {
		getRendererProperties().showWindow();
	}
	
}
