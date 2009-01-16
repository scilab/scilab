/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - 2008 Digiteo Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.tab;

import java.awt.Color;
import java.awt.Container;
import java.awt.Point;
import java.awt.ScrollPane;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.Graphics;
import java.lang.reflect.InvocationTargetException;

import javax.swing.SwingUtilities;



/**
 * Scroll pane based on AWT technology
 * @author Jean-Baptiste silvy
 */
public class AwtScilabScrollPane implements ScilabScrollPane {

	/** needed */
	private static final long serialVersionUID = -3303313501821629057L;
	
	private AwtCustomScrollPane scrolling;
	
	private SwingScilabTab parentTab;
	
	/**
	 * AWT ScrollPane are not able to change their scrollbars policy once created.
	 * To be able to emulate this behaviour, we need actually change the scrollpane
	 * each time the scrollbar policy needs to change.
	 * This class is the real scrollpane component used.
	 */
	private class AwtCustomScrollPane extends ScrollPane implements ComponentListener {
		
		private static final long serialVersionUID = -127053010360658684L;
		private SwingScilabAxes axes;
		
		/**
		 * Constructor
		 * @param axes axes to scroll
		 * @param scrollBarPolicy if the scrollpane should have scrollbars or not
		 */
		public AwtCustomScrollPane(SwingScilabAxes axes, int scrollBarPolicy) {
			super(scrollBarPolicy);
			this.axes = axes;
			addComponentListener(this);
			add(axes);
		}
		
		/**
		 * Redefine paint children to be sure that AWT components are well painted.
		 * @param g graphic pipeline
		 */
		public void paint(Graphics g) {
			if (axes != null) {
			  // axes won't be displayed automatically
			  axes.paint(g);
			}
			super.paint(g);
		}
		
		/**
		 * Call when the scrollPane become hidden
		 * @param event generated event
		 */
		public void componentHidden(ComponentEvent event) { }


		/**
		 * Call when the scrollPane is moved
		 * @param event generated event
		 */
		public void componentMoved(ComponentEvent event) { }


		/**
		 * Call when the scrollPane is resized
		 * @param event generated event
		 */
		public void componentResized(ComponentEvent event) {
			if (axes.getAutoResizeMode()) {
				// in auto resize mode the viewport axes
				// fill the tab and then the view
				axes.setSize(scrolling.getViewportSize());
			}
			
		}
		
		/**
		 * Call when the scrollPane appears on the screen
		 * @param event generated event
		 */
		public void componentShown(ComponentEvent event) { }
		
		/**
		 * @return the scrolled axes
		 */
		public SwingScilabAxes getAxes() {
			return axes;
		}
		
	}
	
	/**
	 * Create a new Scroll pane around an axes.
	 * @param axes axes to scroll
	 * @param parentTab parentTab of the scrollPane
	 */
	public AwtScilabScrollPane(SwingScilabAxes axes, SwingScilabTab parentTab) {
		//super(ScrollPane.SCROLLBARS_AS_NEEDED);
		if (axes.getAutoResizeMode()) {
			this.scrolling = new AwtCustomScrollPane(axes, ScrollPane.SCROLLBARS_NEVER);
		} else {
			this.scrolling = new AwtCustomScrollPane(axes, ScrollPane.SCROLLBARS_AS_NEEDED);
		}
		this.parentTab = parentTab;
		// use the axes background as default one
		setRealBackground(scrolling.getAxes().getBackground());
	}
	
	/**
	 * @return [x,y,w,h] Array of size 4 containing the position
	 *         width and height of the viewPort
	 */
	public int[] getViewingRegion() {
		int[] res = {scrolling.getScrollPosition().x,
					 scrolling.getScrollPosition().y,
					 scrolling.getViewportSize().width,
					 scrolling.getViewportSize().height};
		return res;
	}

	/**
	 * Set the background of the scrollPane
	 * @param red red channel of the color
	 * @param green green channel
	 * @param blue blue channel
	 */
	public void setBackground(double red, double green, double blue) {
		Color newColor = new Color((float) red, (float) green, (float) blue);
		// change the background of the underlying panel
		setRealBackground(newColor);
	}
	
	/**
	 * Set the displayed background of the scrollpane
	 * @param background color to use as background
	 */
	private void setRealBackground(Color background) {
		scrolling.getComponent(0).setBackground(background);
	}

	/**
	 * Move the viewport
	 * @param xPos the x position to scroll to
	 * @param yPos the y position to scroll to
	 */
	public void setViewPosition(int xPos, int yPos) {
		scrolling.setScrollPosition(new Point(xPos, yPos));
	}
	
	
	/**
	 * Redefine paint children to be sure that AWT components are well painted.
	 * @param g graphic pipeline
	 */
	public void paint(Graphics g) {
		scrolling.paint(g);
	}
	
	/**
	 * Enable or disable auto resize mode
	 * @param autoResizeMode true if autoresize is on
	 */
	public void setAutoResizeMode(boolean autoResizeMode) {
		final SwingScilabAxes movedAxes = scrolling.getAxes();
		final boolean autoResizeModeF = autoResizeMode;
		if (autoResizeMode != movedAxes.getAutoResizeMode()) {
			try {
    			SwingUtilities.invokeAndWait(new Runnable() {
    				public void run() {
    					scrolling.remove(movedAxes);
    					scrolling.getAxes().setSize(scrolling.getViewportSize());
    					if (autoResizeModeF) {
    						// switch to auto resize mode
    						scrolling = new AwtCustomScrollPane(movedAxes, ScrollPane.SCROLLBARS_NEVER);
    					} else {
    						scrolling = new AwtCustomScrollPane(movedAxes, ScrollPane.SCROLLBARS_AS_NEEDED);
    					}
    					movedAxes.setAutoResizeMode(autoResizeModeF);
    					// set the new axes
    					parentTab.setContentPane(scrolling);
    					parentTab.revalidate();
    				}
    			});
    		} catch (InterruptedException e) {
    			e.printStackTrace();
    		} catch (InvocationTargetException e) {
    			e.getCause().printStackTrace();
    		}
			
		}
	}
	
	/**
	 * Enable or disable auto resize mode
	 * @return true if autoresize is on
	 */
	public boolean getAutoResizeMode() {
		return scrolling.getAxes().getAutoResizeMode();
	}
	
	/**
	 * @return Container representation of this object
	 */
	public Container getAsContainer() {
		return scrolling;
	}

}
