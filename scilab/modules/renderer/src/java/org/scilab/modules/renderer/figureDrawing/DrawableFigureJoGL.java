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
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;

import org.scilab.modules.renderer.ObjectJoGL;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.utils.ColorMap;

/**
 * Class containing functions called by DrawableFigureJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableFigureJoGL extends ObjectJoGL {
	
	/** Canvas to draw the figure */
	private FigureCanvas canvas;
	/** Window to draw the figure */
	private Tab graphicTab;
	/** store the figureIndex */
	private int figureId;
	
	/**
	 * Default Constructor
	 */
	public DrawableFigureJoGL() {
		super();
      	canvas = null;
      	graphicTab  = null;
      	setColorMap(ColorMap.create());
      	figureId = -1; // ffigue ids should be greater than 0.
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
		super.initializeDrawing(parentFigureIndex);
		GL gl = getGL();
	    gl.glLoadIdentity();
	    gl.glTranslatef(0.0f, 0.0f, -6.0f);
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		super.endDrawing();
	}
  
  /**
   * Display the info message of the figure
   * @param infoMessage string to display describing the figure
   */
  public void updateInfoMessage(String infoMessage) {
    if (canvas == null) { return; }
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
      
      canvas = FigureCanvas.create();
      canvas.addGLEventListener(new SciRenderer(figureIndex));
      graphicTab.addMember(canvas);
      canvas.draw();
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
  	 * Get the canvas in which the figure is drawn
  	 * @return canvas in x=which the figur eis drawn or null if none exists
  	 */
  	public FigureCanvas getCanvas() { return canvas; }
  	
  	/**
  	 * Set the background color of the figure
  	 * @param colorIndex index of the colro to use
  	 */
  	public void setBackgroundColor(int colorIndex) {
  		double[] color = getColorMap().getColor(colorIndex);
  		getGL().glClearColor((float) color[0], (float) color[1], (float) color[2], 1.0f); // alpha is set to 1
  		getGL().glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
  	}
  	
}
