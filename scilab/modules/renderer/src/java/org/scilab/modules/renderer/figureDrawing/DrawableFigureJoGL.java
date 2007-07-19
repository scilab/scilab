/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.figureDrawing;


import javax.media.opengl.GL;

import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.canvas.ScilabCanvas;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;

import org.scilab.modules.renderer.ObjectJoGL;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.utils.ColorMap;
import org.scilab.modules.renderer.ObjectJoGLCleaner;

import org.scilab.modules.gui.utils.Size;


/**
 * Class containing functions called by DrawableFigureJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableFigureJoGL extends ObjectJoGL {
	
	/** Canvas to draw the figure */
	private Canvas canvas;
	/** Window to draw the figure */
	private Tab graphicTab;
	/** store the figureIndex */
	private int figureId;
	/** To get all the objects which needs to be destroyed */
	private ObjectJoGLCleaner destroyedObjects;
	
	/**
	 * Default Constructor
	 */
	public DrawableFigureJoGL() {
		super();
      	canvas = null;
      	graphicTab  = null;
      	setColorMap(ColorMap.create());
      	figureId = -1; // figure ids should be greater than 0.
      	destroyedObjects = new ObjectJoGLCleaner();
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
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		// destroy all the objects
		destroyedObjects.destroyAll(parentFigureIndex);
		super.initializeDrawing(parentFigureIndex);
		getGL().glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
		getGL().glClearDepth(1.0f);
		getGL().glEnable(GL.GL_DEPTH_TEST);
		getGL().glDepthFunc(GL.GL_LEQUAL);
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
		return graphicTab.getName();
	}
	
  /**
   * Display the info message of the figure
   * @param infoMessage string to display describing the figure
   */
  public void setInfoMessage(String infoMessage) {
    graphicTab.setName(infoMessage);
  }

  /**
   * Force the display of the canvas
   * Called from C to be sure to be in the right context
   */
  public void display() {
	  canvas.repaint();
  }
  
  /**
   * If needed create a new context to draw the figure
   * @param figureIndex number of the figurewhich will be displayed in the canvas
   */
  public void openRenderingCanvas(int figureIndex) {
      if (canvas != null) { return; }
      Window graphicView = ScilabWindow.createWindow();
      graphicView.draw();
      graphicView.setTitle("Graphic window number " + figureIndex);
      
      /* FIGURE */
      /* Create the tab to put a canvas into */
      graphicTab = ScilabTab.createTab("");
      graphicTab.setName("");
      graphicView.addTab(graphicTab);
 
      canvas = ScilabCanvas.createCanvas(figureIndex);
      //canvas.addGLEventListener(new SciRenderer(figureIndex));
      graphicTab.addMember(canvas);
      canvas.display();
    }

  /**
   * Destroyr the rendering context
   */
  public void closeRenderingCanvas() {
      if (canvas != null) {
    	FigureMapper.removeMapping(figureId);
        canvas = null;
        graphicTab = null;
      }
    }
  
  /**
   * Set a new colormap to the figure
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
  	public ObjectJoGLCleaner getObjectCleaner() {
  		return destroyedObjects;
  	}
  	
  	/**
  	 * Get the canvas in which the figure is drawn
  	 * @return canvas in x=which the figur eis drawn or null if none exists
  	 */
  	public Canvas getCanvas() { return canvas; }
  	
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
	}
	
	
	/**
	 * @return width of the rendering canvas
	 */
	public int getCanvasWidth() {
		return canvas.getDims().getWidth();
	}
	
	/**
	 * @return height of the rendering canvas
	 */
	public int getCanvasHeight() {
		return canvas.getDims().getHeight();
	}
	
	/**
	 * Set the rendering canvas size.
	 * @param width new width in pixels
	 * @param height new height in pixels
	 */
	public void setCanvasSize(int width, int height) {
		canvas.setDims(new Size(width, height));
	}
	
	/**
	 * @return width of the rendering window
	 */
	public int getWindowWidth() {
		return graphicTab.getDims().getWidth();
	}
	
	/**
	 * @return height of the rendering window
	 */
	public int getWindowHeight() {
		return graphicTab.getDims().getHeight();
	}
	
	/**
	 * Set the rendering window size.
	 * @param width new width in pixels
	 * @param height new height in pixels
	 */
	public void setWindowSize(int width, int height) {
		graphicTab.getDims().setWidth(width);
		graphicTab.getDims().setHeight(height);
	}
	
	/**
	 * Get the position of the window enclosing the figure
	 * @return X coordinate in pixels of the window
	 */
	public int getWindowPosX() {
		return graphicTab.getPosition().getX();
	}
	
	/**
	 * Get the position of the window enclosing the figure
	 * @return Y coordinate in pixels of the window
	 */
	public int getWindowPosY() {
		return graphicTab.getPosition().getY();
	}
	
	/**
	 * Get the position of the window enclosing the figure
	 * @param posX X coordinate in pixels of the window
	 * @param posY Y coordinate in pixels of the window
	 */
	public void setWindowPosition(int posX, int posY) {
		graphicTab.getPosition().setX(posX);
		graphicTab.getPosition().setY(posY);
	}
  	
}
