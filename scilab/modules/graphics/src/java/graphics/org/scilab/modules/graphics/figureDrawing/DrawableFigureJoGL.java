/*------------------------------------------------------------------------*/
/* file: DrawableFigureJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        figure object with JoGL                                         */
/*------------------------------------------------------------------------*/

package org.scilab.modules.graphics.figureDrawing;

import java.awt.*;
import java.awt.event.*;
import java.io.* ;

import javax.media.opengl.* ;
import javax.media.opengl.glu.* ;
import com.sun.opengl.util.* ;
import com.sun.opengl.impl.* ;
import javax.swing.JFrame;

public class DrawableFigureJoGL
{
  public DrawableFigureJoGL()
    {
      canvas = null ;
      frame  = null ;
      
    }

  public void initializeDrawing()
  {
    if ( canvas == null ) { return ; }
    
    canvas.getContext().makeCurrent() ;
  }

  public void endDrawing()
  {
    if ( canvas == null ) { return ; }
    // ok the thread can work on other things
    canvas.getContext().release() ;
    // we need to swap the buffer in the same action
    // and not resynchronize with a call of canvas.display
    canvas.swapBuffers() ; //canvas.display() ;
  }

  public void updateInfoMessage()
  {
    if ( canvas == null ) { return ; }
    final GL gl = canvas.getGL();
    GLU glu = new GLU() ;
    gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
    gl.glLoadIdentity();
    gl.glTranslatef(-1.5f, 0.0f, -6.0f);
    gl.glRotatef(0.2f, 0.0f, 1.0f, 0.0f);
    gl.glBegin(GL.GL_TRIANGLES);
    gl.glColor3f(1.0f, 0.0f, 0.0f ) ;
    glu.gluSphere( glu.gluNewQuadric(), 1.0, 32, 32 ) ;

  }

  public void openRenderingCanvas( int figureIndex )
    {
      if ( canvas != null ) { return ; }
      frame = new JFrame( "Java Window number " + figureIndex  ) ;
      GLCapabilities cap = new GLCapabilities();
      cap.setAlphaBits(8);
      cap.setHardwareAccelerated(true);
      cap.setDoubleBuffered(true);
      cap.setDepthBits(8);
      canvas = new GLJPanel(cap) ;
      canvas.addGLEventListener(new SciRenderer());
      
      frame.add(canvas);
      frame.setSize(640, 480);
      frame.addWindowListener(new WindowAdapter()
        {
          public void windowClosing(WindowEvent e)
            {
              closeRenderingCanvas() ;
            }
        });
       frame.show();
       canvas.display() ; // needed for full initialization
       canvas.setAutoSwapBufferMode( false ) ;
       canvas.requestFocus();
    }

  public void closeRenderingCanvas()
    {
      if ( frame != null )
      {
        frame.dispose() ;
        canvas = null ;
        frame = null ;
      }
    }

  protected GLJPanel canvas ;
  protected JFrame frame ;
}
