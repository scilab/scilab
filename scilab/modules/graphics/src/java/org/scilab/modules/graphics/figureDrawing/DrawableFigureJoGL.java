/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

package org.scilab.modules.graphics.figureDrawing;


import javax.media.opengl.GL;
import javax.media.opengl.GLCapabilities;

import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.bridge.SwingScilabCanvas;


import org.scilab.modules.gui.utils.Size;

import org.scilab.modules.graphics.DrawableObjectJoGL;
import org.scilab.modules.graphics.FigureCanvasMapper;

/**
 * Class containing functions called by DrawableFigureJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableFigureJoGL extends DrawableObjectJoGL {
	
	/** Canvas to draw the figure */
	private SwingScilabCanvas canvas;
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
    }
	
	/**
	 * Set figureId property
	 * @param figureId new figure Id
	 */
	public void setFigureId(int figureId) {
		this.figureId = figureId;
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		GL gl = getGL();
	    gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
	    gl.glLoadIdentity();
	    gl.glTranslatef(0.0f, 0.0f, -6.0f);
	    gl.glColor3f(1.0f, 1.0f, 1.0f);
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
      System.out.println("openRenderingCanvas");
      Window graphicView = ScilabWindow.createWindow();
      graphicView.draw();
      graphicView.setTitle("Graphic window number " + figureIndex);
      
      /* FIGURE */
      /* Create the tab to put a canvas into */
      graphicTab = ScilabTab.createTab("");
      graphicTab.setName("");
      graphicView.addTab(graphicTab);
      
      GLCapabilities cap = new GLCapabilities();
      cap.setAlphaBits(8);
      cap.setHardwareAccelerated(true);
      cap.setDoubleBuffered(true);
      cap.setDepthBits(8);
      canvas = new SwingScilabCanvas(cap);
      canvas.addGLEventListener(new SciRenderer());
      figureId = figureIndex;
      FigureCanvasMapper.addMapping(figureIndex, canvas);
      
      /* Create the canvas */
      //canvas = (SwingScilabCanvas) ScilabCanvas.createCanvas();
      canvas.setDims(new Size(640, 480));
      //canvas.draw();
      graphicTab.addMember(canvas);
      canvas.draw();
      //graphicTab.draw();
      // graphicView.draw();
      System.out.println("end openRenderingCanvas");
    }

  /**
   * Destroyr the rendering context
   */
  public void closeRenderingCanvas() {
      if (canvas != null) {
    	FigureCanvasMapper.removeMapping(figureId);
        canvas = null;
        graphicTab = null;
      }
    }
}
