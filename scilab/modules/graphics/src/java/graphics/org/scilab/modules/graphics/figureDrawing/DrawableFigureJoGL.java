/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

package org.scilab.modules.graphics.figureDrawing;


import javax.swing.JFrame;

import javax.media.opengl.GLJPanel;
import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;
import javax.media.opengl.GLCapabilities;

import java.awt.event.WindowEvent;
import java.awt.event.WindowAdapter;

/**
 * Class containing functions called by DrawableFigureJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableFigureJoGL {
	
	/** Canvas to draw the figure */
	private GLJPanel canvas;
	/** Window to draw the figure */
	private JFrame frame;
	
	/***
	 * Default Constructor
	 */
  public DrawableFigureJoGL() {
      canvas = null;
      frame  = null;
    }

  /**
   * Function called before beginning to use OpenGL methods.
   */
  public void initializeDrawing() { }

  /**
   * Function called at the end of the OpenGL use.
   */
  public void endDrawing() { }

  /**
   * Display the info message of the figure
   */
  public void updateInfoMessage() {
    if (canvas == null) { return; }
    final GL gl = canvas.getGL();
    GLU glu = new GLU();
    gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
    gl.glLoadIdentity();
    gl.glTranslatef(-1.5f, 0.0f, -6.0f);
    gl.glRotatef(0.2f, 0.0f, 1.0f, 0.0f);
    gl.glColor3f(1.0f, 0.0f, 0.0f);
    glu.gluSphere(glu.gluNewQuadric(), 1.0, 32, 32);

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
      frame = new JFrame("Java Window number " + figureIndex);
      GLCapabilities cap = new GLCapabilities();
      cap.setAlphaBits(8);
      cap.setHardwareAccelerated(true);
      cap.setDoubleBuffered(true);
      cap.setDepthBits(8);
      canvas = new GLJPanel(cap);
      canvas.addGLEventListener(new SciRenderer());
      
      frame.add(canvas);
      frame.setSize(640, 480);
      frame.addWindowListener(new WindowAdapter()
        {
          public void windowClosing(WindowEvent e) {
              closeRenderingCanvas();
          }
        });
       frame.show();
    }

  /**
   * Destroyr the rendering context
   */
  public void closeRenderingCanvas() {
      if (frame != null) {
        frame.dispose();
        canvas = null;
        frame = null;
      }
    }
}
