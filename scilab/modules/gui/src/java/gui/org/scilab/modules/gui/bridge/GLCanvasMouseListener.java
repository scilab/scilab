package org.scilab.modules.gui.bridge;

import java.awt.*;
import java.awt.event.*;
import java.io.* ;

class GLCanvasMouseListener implements MouseListener
{

  public GLCanvasMouseListener( Component watchedComponent )
    {
      System.out.println("GLCanvasMouseListener");
	  this.watchedComponent = watchedComponent ;
      
    }

  public void mouseClicked(MouseEvent e)
    {
      System.out.println("mouseClicked");
      watchedComponent.requestFocus() ;
    }


  public void mouseEntered(MouseEvent e) {}

  public void mouseReleased(MouseEvent e) {}
  public void mouseExited(MouseEvent e) {}
  public void mousePressed(MouseEvent e) {}


  protected Component watchedComponent ;
  
}
