/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - 2008 Digitei Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.tab;

import java.awt.AWTEvent;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.security.InvalidParameterException;

import javax.swing.JComponent;
import javax.swing.JLayeredPane;
import javax.swing.Scrollable;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.tree.SwingScilabTree;
import org.scilab.modules.gui.canvas.Canvas;
import org.scilab.modules.gui.events.ScilabEventListener;
import org.scilab.modules.gui.utils.Debug;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;

/**
 * Class defining the content pane of the Tab.
 * We use a JLayeredPane to be able to put the widget above canvases.
 * this object is scrollable
 * @author Jean-Baptiste Silvy
 */
public class SwingScilabAxes extends JLayeredPane implements Scrollable {

	/** Needed */
	private static final long serialVersionUID = 4270905996765800574L;
	private static final int SMALL_DISPLACEMENT = 2;
	private static final int LARGE_DISPLACEMENT = 20;

	// definition of layers from bottom to top
	//private static final Integer BACKGROUND_LAYER = 0;
	private static final Integer CANVAS_LAYER = new Integer(1);
	private static final Integer WIDGET_LAYER = new Integer(2);

	/** Use to put a component above any other object within its layer */
	private static final int TOP_POSITION = 0;
	/** Use to put a component below any other object within its layer */
	private static final int BOTTOM_POSITION = -1;

	private boolean autoResizeMode;

	private ScilabEventListener eventHandler;

	/** An axes may contain at most one canvas for now */
	private SwingScilabCanvas graphicCanvas;

	private int figureId;

	/**
	 * Default constructor
	 * @param figureId id of the displayed figure
	 */
	public SwingScilabAxes(int figureId) {
		super();
		// default behavior
		autoResizeMode = true;

		eventHandler = null;
		graphicCanvas = null;
		this.figureId = figureId;

		// we use a null layout. It's needed for uicontrol so they don't resize when the canvas
		// is resized. However, its imply to set the canvas size by hand.
		setLayout(null);

		// for event handling
		this.setFocusable(true);
		// Enable mouse Events sensitivity...
		this.enableEvents(AWTEvent.MOUSE_EVENT_MASK | AWTEvent.MOUSE_MOTION_EVENT_MASK);

	}

	/**
	 * Override setSize to be sure that setPreferred size is called
	 * @param newSize new size to apply
	 */
	public void setSize(Dimension newSize) {

		// get the greatest size we can use
		/*int[] maxSize = RenderingCapabilities.getMaxCanvasSize();

		// make sure size is not greater than the max size
		Dimension finalDim = new Dimension(Math.min(newSize.width, maxSize[0]),
										   Math.min(newSize.height, maxSize[1]));

		super.setSize(finalDim);
		super.setPreferredSize(finalDim);

		// force resize of canvas also
		// to be sure that the canvas has the same size as the axes
		if (graphicCanvas != null) {
			graphicCanvas.setSize(finalDim);
			}*/
	    Dimension finalDim = (Dimension) newSize.clone();//new Dimension((int) newSize.getWidth(), (int) newSize.getHeight());
	    setSize(finalDim);
	}

	/**
	 * Specify whether the canvas should fit the parent tab size
	 * (and consequently the scrollpane size) or not
	 * @param onOrOff true to enable autoresize mode
	 */
	public void setAutoResizeMode(boolean onOrOff) {
		this.autoResizeMode = onOrOff;
	}

	/**
	 * @return whether the resize mode is on or off
	 */
	public boolean getAutoResizeMode() {
		return autoResizeMode;
	}

	/**
	 * Get the size of the viewport viewing the canvas
	 * @return wanted size
	 */
	public Dimension getPreferredScrollableViewportSize() {
		return getPreferredSize();
	}

	/**
	 * Get the displacement in pixels when user click on the scrollbar track
	 * @param visibleRect The view area visible within the viewport
	 * @param orientation Either SwingConstants.VERTICAL or SwingConstants.HORIZONTAL.
	 * @param direction Less than zero to scroll up/left, greater than zero for down/right.
	 * @return displacement in pixel
	 */
	public int getScrollableBlockIncrement(Rectangle visibleRect, int orientation, int direction) {
		return LARGE_DISPLACEMENT;
	}

	/**
	 * Specify whether the canvas height must match the viewport height
	 * @return true if the canvas must match the viewport height
	 */
	public boolean getScrollableTracksViewportHeight() {
		return autoResizeMode;
	}

	/**
	 * Specify whether the canvas height must match the viewport width
	 * @return true if the canvas must match the viewport width
	 */
	public boolean getScrollableTracksViewportWidth() {
		return getScrollableTracksViewportHeight();
	}

	/**
	 * Get the displacement in pixels when user click on the scrollbar buttons
	 * @param visibleRect The view area visible within the viewport
	 * @param orientation Either SwingConstants.VERTICAL or SwingConstants.HORIZONTAL.
	 * @param direction Less than zero to scroll up/left, greater than zero for down/right.
	 * @return displacement in pixel
	 */
	public int getScrollableUnitIncrement(Rectangle visibleRect, int orientation, int direction) {
		return SMALL_DISPLACEMENT;
	}

	/**
	 * Set the event handler of the Canvas
	 * @param funName the name of the Scilab function to call
	 */
	public void setEventHandler(String funName) {
		disableEventHandler();
		eventHandler = new ScilabEventListener(funName, getFigureId());
	}


	/**
	 * Set the status of the event handler of the Canvas
	 * @param status is true to set the event handler active
	 */
	public void setEventHandlerEnabled(boolean status) {
		if (status) {
			enableEventHandler();
		} else {
			disableEventHandler();
		}
	}

	/**
	 * Turn on event handling.
	 */
	private void enableEventHandler() {
		addKeyListener(eventHandler);
		addMouseListener(eventHandler);
		addMouseMotionListener(eventHandler);
	}

	/**
	 * Turn off event handling
	 */
	private void disableEventHandler() {
		if (eventHandler != null) {
			removeKeyListener(eventHandler);
			removeMouseListener(eventHandler);
			removeMouseMotionListener(eventHandler);
		}
	}

	/**
	 * Set the background color of the axes.
	 * @param red red channel of the color
	 * @param green green channel
	 * @param blue blue channel
	 */
	public void setBackground(double red, double green, double blue) {
		Color color = new Color((float) red, (float) green, (float) (blue));
		setBackground(color);
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * This function is added here to remove direct dependencies from SwingScilab Tab
	 * to GLJPanel via SwingScilabCanvas
	 * @param member the member to add
	 * @return index of member in ArrayList
	 */
	public int addMember(Canvas member) {
		return this.addCanvas((SwingScilabCanvas) member.getAsSimpleCanvas());
	}

	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param canvas the canvas to add
	 * @return index of member in ArrayList
	 */
	public int addCanvas(SwingScilabCanvas canvas) {


		return 0;
	}

	/**
	 * We want to be able to remove directly a Canvas from a Tab.
	 * This function is added here to remove direct dependencies from SwingScilab Tab
	 * to GLJPanel via SwingScilabCanvas
	 * @param member canvas to remove
	 */
	public void removeMember(Canvas member) {
		this.removeCanvas((SwingScilabCanvas) member.getAsSimpleCanvas());
	}

	/**
	 * Remove a Canvas from the axes
	 * @param canvas canvas to remove
	 */
	public void removeCanvas(SwingScilabCanvas canvas) {

		if (canvas != graphicCanvas) {
			// should not happen, no need for localization
			throw new UnsupportedOperationException("Trying to remove an unknown canvas.");
		}

		//ScilabSwingUtilities.removeFromParent(canvas.getAsComponent());

		graphicCanvas = null;

	}

	/**
	 * Add a widget into the axes
	 * @param widget widget to add
	 * @return index of member in ArrayList
	 */
	public int addWidget(JComponent widget) {
		// put the newly added object above any other objects
		//ScilabSwingUtilities.addToParent(widget, this, WIDGET_LAYER, TOP_POSITION);
		this.add(widget, WIDGET_LAYER, TOP_POSITION);
		return getComponentZOrder(widget);
	}

	/**
	 * Remove a widget from the axes
	 * @param widget widget to remove
	 */
	public void removeWidget(JComponent widget) {
		ScilabSwingUtilities.removeFromParent(widget);
	}

	/**
	 * Add a frame to the axis
	 * @param frame frame to add
	 * @return index of member in ArrayList
	 */
	public int addFrame(SwingScilabFrame frame) {
		//ScilabSwingUtilities.addToParent(frame, this, WIDGET_LAYER, BOTTOM_POSITION);
		this.add(frame, WIDGET_LAYER, TOP_POSITION);
		return getComponentZOrder(frame);
	}

	/**
	 * Remove a frame from an axes.
	 * @param frame frame to remove
	 */
	public void removeFrame(SwingScilabFrame frame) {
		ScilabSwingUtilities.removeFromParent(frame);
	}
	
	/**
	 * Add a Jtree to a scrolling pane
	 * @param member a graphical tree
	 * @return an integer
	 */
	public int addTree(SwingScilabTree member) {
		// Add the jtree to a scrolling pane		
		this.add(member.getAsComponent(), WIDGET_LAYER, TOP_POSITION);
		//this.setAutoResizeMode(false);
		return this.getComponentZOrder(member.getAsComponent());
	}

	/**
	 * Remove a Jtree to a scrolling pane
	 * @param member a graphical tree
	 * @return an integer
	 */
	public int removeTree(SwingScilabTree member) {
		// Add the jtree to a scrolling pane
		this.remove(member.getAsComponent());
		return this.getComponentZOrder(member.getAsComponent());
	}
	
	/**
	 * @return the figureId
	 */
	public int getFigureId() {
		return figureId;
	}

	/**
	 * BLOUNO
	 */
	public void repaint() {
	    Debug.DEBUG(this.getClass().getSimpleName(), "repaint");
	    super.repaint();
	}

	/**
	 * Redefine paint children to be sure that AWT components are well painted.
	 * @param g graphic pipeline
	 */
	public void paint(Graphics g) {
		Component[] children = getComponents();
		for (int i = 0; i < children.length; i++) {
			// AWT children don't draw themselves automatically
			// so force their draw
			if (!children[i].isLightweight()) {
				children[i].paint(g);
			}
		}
		super.paint(g);
	}

}
