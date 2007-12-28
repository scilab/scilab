package org.scilab.modules.gui.bridge;

import java.awt.Component;
import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;

/**
 * 
 *
 */
class GLCanvasMouseListener implements MouseListener {
  private Component watchedComponent ;

    /**
     * @TODO add comment
     *
     * @param watchedComponent  
     * @return ReturnValue
	 */
  public GLCanvasMouseListener(Component watchedComponent) {
      System.out.println("GLCanvasMouseListener");
	  this.watchedComponent = watchedComponent;
      
    }

    /**
     * @TODO add comment
     *
     * @param e 
	 */
  public void mouseClicked(MouseEvent e) {
      System.out.println("mouseClicked");
      watchedComponent.requestFocus();
    }


    /**
     * @TODO add comment
     *
     * @param e 
	 */
  public void mouseEntered(MouseEvent e) { }

    /**
     * @TODO add comment
     *
     * @param e 
	 */
  public void mouseReleased(MouseEvent e) { }

    /**
     * @TODO add comment
     *
     * @param e 
	 */
	public void mouseExited(MouseEvent e) { }
	
    /**
     * @TODO add comment
     *
     * @param e 
	 */
	public void mousePressed(MouseEvent e) { }
  
}
