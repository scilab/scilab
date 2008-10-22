/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Jean-Baptiste Silvy
 * desc : GLEventListener used to retrieve informations from the
 * canvas
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.figureDrawing;

import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GL;

import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.jni.FigureScilabCall;
import org.scilab.modules.renderer.utils.RenderingCapabilities;



/**
 * GLEventListener used by Scilab
 * @author Jean-Baptiste Silvy silvy
 */
public class SciRenderer
  implements GLEventListener {
	
	/** index of the figure to render */ 
	private int renderedFigure;
	
	
	
	/**
	 * Default constructor
	 * @param figureIndex index of the figure to render
	 */
	public SciRenderer(int figureIndex) {
		renderedFigure = figureIndex;
	}
	
  /** Called by the drawable to initiate OpenGL rendering by the client.
   * After all GLEventListeners have been notified of a display event, the 
   * drawable will swap its buffers if necessary.
   * @param gLDrawable The GLDrawable object.
   */    
  public void display(GLAutoDrawable gLDrawable) {

	DrawableFigureGL curFigure = FigureMapper.getCorrespondingFigure(renderedFigure);
	
	if (curFigure == null) {
		// figure has been destroyed
		// nothing to render
		return;
	}
	GL gl = gLDrawable.getGL();
	
	if (!curFigure.getRenderingRequested()) {
		// figure rendering not requested by Scilab
		// This is a swing event so just keep the buffer
		// keep previous buffer
		gl.glLogicOp(GL.GL_NOOP);
		gl.glEnable(GL.GL_COLOR_LOGIC_OP);
	} else {
		// the requested display has been done, next one won't chnage display unless
		// it is also requested
		curFigure.setRenderingRequested(false);
		if (curFigure.isRubberBoxModeOn()) {
			curFigure.getRubberBox().drawInContext();
		} else {
			// draw the hierarchy
			
			// to enable anti_aliasing
			//gl.glEnable(GL.GL_MULTISAMPLE);
			
			FigureScilabCall.displayFigure(renderedFigure);
			//gl.glDisable(GL.GL_MULTISAMPLE);
		}
	}
	
	// seems that buffers will be swaped any way with GLJPanel
//	if (!gLDrawable.getAutoSwapBufferMode()) {
//		gLDrawable.swapBuffers();
//	}
          

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
	  DrawableFigureGL curFigure = FigureMapper.getCorrespondingFigure(renderedFigure);
	  
	  if (curFigure == null) {
		  // figure has been destroyed
		  // nothing to render
		  return;
	  }
	  //gLDrawable.setAutoSwapBufferMode(false);
	  GL gl = gLDrawable.getGL();
	  
	  // update the maximum size of a panel if needed
	  RenderingCapabilities.updateMaxCanvasSize(gl);
	  
      gl.glShadeModel(GL.GL_SMOOTH);              // Enable Smooth Shading
      gl.glClearColor(1.0f, 1.0f, 1.0f, 0.0f);    // white Background
      gl.glClear(GL.GL_COLOR_BUFFER_BIT); // clear here othrwise we don't know what we will have
      gl.glClearDepth(1.0f);                      // Depth Buffer Setup
      gl.glEnable(GL.GL_DEPTH_TEST);							// Enables Depth Testing
      gl.glDepthFunc(GL.GL_LEQUAL);								// The Type Of Depth Testing To Do
      gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_FASTEST);	// Really fast
      gl.glDisable(GL.GL_LINE_SMOOTH); // we prefer thin line
      // Color of texture will not be 
		// mixed with the color of the polygon
      gl.glTexEnvi(GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_MODE, GL.GL_DECAL);

      gl.glEnable(GL.GL_COLOR_LOGIC_OP); // to use pixel drawing mode
      gl.glLogicOp(GL.GL_COPY);
      
      curFigure.getColorMap().clearTexture();
      // free all the text renderers
      curFigure.getTextRendererCreator().clear();
      FigureScilabCall.redrawFigure(renderedFigure);
		  
	  // we need to redraw the figure so request one
	  curFigure.setRenderingRequested(true);

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
  public void reshape(GLAutoDrawable gLDrawable, int x, int y, int width, int height)	{
	  DrawableFigureGL curFigure = FigureMapper.getCorrespondingFigure(renderedFigure);
	  
	  if (curFigure == null) {
		  return;
	  }
	  // if autoresize mode is off we don't actually need to do something
	  // the reshape is not effective
	  if (curFigure.getAutoResizeMode()) {
		  // Axes ticks may change with new shape so redraw all subwins
		  FigureScilabCall.redrawSubwins(renderedFigure);
	  
		  // we need to redraw the figure so request one
	  
		  curFigure.setRenderingRequested(true);
	  }
	  
	  if (curFigure.isRubberBoxModeOn()) {
		// in rubber box mode we don't automatically redraw the figure
		// so we need to force a redraw to get a consistent background
		// previous rubber box will be then cleared, so we must erase it
		curFigure.getRubberBox().doNotErase();
		FigureScilabCall.displayFigure(renderedFigure);
	  }
  }


}
