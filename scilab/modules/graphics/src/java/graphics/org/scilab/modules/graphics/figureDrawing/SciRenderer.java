/*------------------------------------------------------------------------*/
/* file: SciRenderer.java                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : GLEventListener used to retrieve informations from the          */
/*        canvas                                                          */
/*------------------------------------------------------------------------*/

package org.scilab.modules.graphics.figureDrawing;

import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;

/**
 * GLEventListener used by Scilab
 * @author Jean-Baptiste Silvy silvy
 */
public class SciRenderer
  implements GLEventListener, KeyListener {
  
	private boolean isInit;
	
	/**
	 * Default constructor
	 */
	public SciRenderer() {
		isInit = false;
	}
	
  /** Called by the drawable to initiate OpenGL rendering by the client.
   * After all GLEventListeners have been notified of a display event, the 
   * drawable will swap its buffers if necessary.
   * @param gLDrawable The GLDrawable object.
   */    
  public void display(GLAutoDrawable gLDrawable) {
      // should call the draw function of the corresponding figure
	  gLDrawable.getContext().setSynchronized(true);
	  FigureScilabCall call = new FigureScilabCall();
	  call.redrawFigure(0);
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
      if (isInit) { return; }
      System.out.println("toto");
      final GL gl = gLDrawable.getGL();
      gl.glShadeModel(GL.GL_SMOOTH);              // Enable Smooth Shading
      gl.glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
      gl.glClearDepth(1.0f);                      // Depth Buffer Setup
      gl.glEnable(GL.GL_DEPTH_TEST);							// Enables Depth Testing
      gl.glDepthFunc(GL.GL_LEQUAL);								// The Type Of Depth Testing To Do
      gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);	// Really Nice Perspective Calculations
      gLDrawable.addKeyListener(this);
      
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
      final GL gl = gLDrawable.getGL();
      final GLU glu = new GLU();
      float h = 1.0f; 

      if (height > 0) {
        // avoid a divide by zero error!
        h = (float) height;
      }
      h = (float) width / h;
      gl.glMatrixMode(GL.GL_PROJECTION);
      gl.glLoadIdentity();
      glu.gluPerspective(45.0f, h, 1.0, 20.0);
      gl.glMatrixMode(GL.GL_MODELVIEW);
      gl.glLoadIdentity();

    }

  /** Invoked when a key has been pressed.
   * See the class description for {@link KeyEvent} for a definition of
   * a key pressed event.
   * @param e The KeyEvent.
   */
  public void keyPressed(KeyEvent e) {
	  if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
        System.out.println("Escape key pressed");
	  } else if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
		  System.out.println("Right Key pressed");
      } else if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
        System.out.println("Left Key pressed");
      }
    }
    
  /** Invoked when a key has been released.
   * See the class description for {@link KeyEvent} for a definition of
   * a key released event.
   * @param e The KeyEvent.
   */
  public void keyReleased(KeyEvent e) { }
    
  /** Invoked when a key has been typed.
   * See the class description for {@link KeyEvent} for a definition of
   * a key typed event.
   * @param e The KeyEvent.
   */
  public void keyTyped(KeyEvent e) { }


}
