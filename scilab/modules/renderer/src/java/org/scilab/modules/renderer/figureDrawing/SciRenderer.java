/*------------------------------------------------------------------------*/
/* file: SciRenderer.java                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : GLEventListener used to retrieve informations from the          */
/*        canvas                                                          */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.figureDrawing;

import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GL;

import org.scilab.modules.renderer.jni.FigureScilabCall;



/**
 * GLEventListener used by Scilab
 * @author Jean-Baptiste Silvy silvy
 */
public class SciRenderer
  implements GLEventListener {
  
	/** To know if the graphic context has already been initialized */
	private boolean isInit;
	
	/** index of the figure to render */
	private int renderedFigure;
	
	/**
	 * Default constructor
	 * @param figureIndex index of the figure to render
	 */
	public SciRenderer(int figureIndex) {
		isInit = false;
		renderedFigure = figureIndex;
	}
	
  /** Called by the drawable to initiate OpenGL rendering by the client.
   * After all GLEventListeners have been notified of a display event, the 
   * drawable will swap its buffers if necessary.
   * @param gLDrawable The GLDrawable object.
   */    
  public void display(GLAutoDrawable gLDrawable) {
    // should call the draw function of the corresponding figure
    gLDrawable.getContext().setSynchronized(true);
    FigureScilabCall.displayFigure(renderedFigure);
  }
    
    
  /** Called when the display mode has been changed.  <B>!! CURRENTLY UNIMPLEMENTED IN JOGL !!</B>
   * @param gLDrawable The GLDrawable object.
   * @param modeChanged Indicates if the video mode has changed.
   * @param deviceChanged Indicates if the video device has changed.
   */
  public void displayChanged(GLAutoDrawable gLDrawable, boolean modeChanged, boolean deviceChanged) { }
	     
  /** Called by the drawable immediately after the OpenGL context is 
   * initialized for the first time. Can be used to perform one-time OpenGL 
   * initialization such as setup of lights and display lists.
   * @param gLDrawable The GLDrawable object.
   */
  public void init(GLAutoDrawable gLDrawable) {
	  
      if (isInit) {
    	  FigureScilabCall.redrawFigure(renderedFigure);
      }
      final GL gl = gLDrawable.getGL();
      gl.glShadeModel(GL.GL_SMOOTH);              // Enable Smooth Shading
      gl.glClearColor(1.0f, 1.0f, 1.0f, 0.0f);    // white Background
      gl.glClearDepth(1.0f);                      // Depth Buffer Setup
      gl.glEnable(GL.GL_DEPTH_TEST);							// Enables Depth Testing
      gl.glDepthFunc(GL.GL_LEQUAL);								// The Type Of Depth Testing To Do
      gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);	// Really Nice Perspective Calculations
      gl.glEnable(GL.GL_LINE_SMOOTH);
      isInit = true;

    }
    
  
  /** Called by the drawable during the first repaint after the component has 
   * been resized. The client can update the viewport and view volume of the 
   * window appropriately, for example by a call to 
   * GL.glViewport(int, int, int, int); note that for convenience the component
   * has already called GL.glViewport(int, int, int, int)(x, y, width, height)
   * when this method is called, so the client may not have to do anything in
   * this method.
   * @param gLDrawable The GLDrawable object.
   * @param x The X Coordinate of the viewport rectangle.
   * @param y The Y coordinate of the viewport rectanble.
   * @param width The new width of the window.
   * @param height The new height of the window.
   */
  public void reshape(GLAutoDrawable gLDrawable, int x, int y, int width, int height)
    {
	  // not needed for now
      //final GL gl = gLDrawable.getGL();
      //float h = 1.0f;

      //if (height > 0) {
        // avoid a divide by zero error!
        //h = (float) height;
      //}
      //h = (float) width / h;
      //gl.glMatrixMode(GL.GL_PROJECTION);
      //gl.glLoadIdentity();
      //gl.glFrustum(0.0, 1.0, 0.0, 1.0, 1.0, 20.0);
      //gl.glMatrixMode(GL.GL_MODELVIEW);
      //gl.glLoadIdentity();

    }


}
