package org.scilab.modules.gui.bridge.tab;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Rectangle;
import java.lang.reflect.InvocationTargetException;

import javax.swing.JComponent;
import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.Scrollable;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.events.AxesRotationTracker;
import org.scilab.modules.gui.events.ScilabEventListener;

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
	
	private static final int BACKGROUND_LAYER = 0;
	private static final int CANVAS_LAYER = 1;
	private static final int WIDGET_LAYER = 2;
	
	private static final int TOP_POSITION = 0;
	private static final int BOTTOM_POSITION = -1;
	
	private boolean autoResizeMode;
	
	private ScilabEventListener eventHandler;
	
	/** Needed to be able to change the background */
	private JPanel backgroundFiller;
	
	private AxesRotationTracker rotationTracker;
	
	/**
	 * Default constructor
	 */
	public SwingScilabAxes() {
		super();
		autoResizeMode = true;
		eventHandler = null;
		
		setLayout(null);
		
		// adding an object to fill the background
		backgroundFiller = new JPanel();
		add(backgroundFiller);
		setLayer(backgroundFiller, BACKGROUND_LAYER , BOTTOM_POSITION);
		
		rotationTracker = new AxesRotationTracker(backgroundFiller);
	}
	
	/**
	 * Override setSize to be sure that setPreferred size is called
	 * @param newSize new size to apply
	 */
	public void setSize(Dimension newSize) {
		super.setSize(newSize);
		super.setPreferredSize(newSize);
		backgroundFiller.setSize(newSize);
	}
	
	/**
	 * Specify wether the canvas should fit the parent tab size
	 * (and consequently the scrollpane size) or not
	 * @param onOrOff true to enable autoresize mode
	 */
	public void setAutoResizeMode(boolean onOrOff) {
		this.autoResizeMode = onOrOff;
	}
	
	/**
	 * @return wether the resize mode is on or off
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
		return autoResizeMode;
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
		eventHandler = new ScilabEventListener(funName, 0);
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
		backgroundFiller.setBackground(color);
	}
	
	/**
	 * Add a member (dockable element) to container and returns its index
	 * @param canvas the canvas to add
	 * @return index of member in ArrayList
	 */
	public int addCanvas(SwingScilabCanvas canvas) {
		final SwingScilabCanvas canvasf = canvas;
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				public void run() {
					add(canvasf);
					setLayer(canvasf, CANVAS_LAYER, TOP_POSITION);
					revalidate();
				}
			});
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.getCause().printStackTrace();
		}
		
		return getComponentZOrder(canvas);
	}
	
	/**
	 * Remove a Canvas from the axes
	 * @param canvas canvas to remove
	 */
	public void removeCanvas(SwingScilabCanvas canvas) {
		final SwingScilabCanvas canvasf = canvas;
		try {
			SwingUtilities.invokeAndWait(new Runnable() {
				public void run() {
					remove(canvasf);
					revalidate();
				}
			});
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			e.getCause().printStackTrace();
		}
	}
	
	/**
	 * Add a widget into the axes
	 * @param widget widget to add
	 * @return index of member in ArrayList
	 */
	public int addWidget(JComponent widget) {
		add(widget);
		// put the newly added object above any other objects
		setLayer(widget, WIDGET_LAYER, TOP_POSITION);
		widget.setBounds(0, 0, 0, 0);
		repaint();
		return getComponentZOrder(widget);
	}
	
	/**
	 * Remove a widget from the axes
	 * @param widget widget to remove
	 */
	public void removeWidget(JComponent widget) {
		Component[] component = getComponentsInLayer(WIDGET_LAYER);
		synchronized (getTreeLock()) {
	        for (int i = 0; i < getComponentCountInLayer(WIDGET_LAYER); ++i) {
	            if (component[i] == widget) {
	            	remove(i);
	                break;
	              }
	          }
	      }
		this.revalidate(); // If do not revalidate then the component do not disappear
	}
	
	/**
	 * Add a frame to the axis
	 * @param frame frame to add
	 * @return index of member in ArrayList
	 */
	public int addFrame(SwingScilabFrame frame) {
		add(frame);
		// put the newly added object above any other objects
		setLayer(frame, WIDGET_LAYER, BOTTOM_POSITION);
		revalidate(); // If do not revalidate then the component do not appear
		return getComponentZOrder(frame);
	}

	/**
	 * Remove a frame from an axes.
	 * @param frame frame to remove
	 */
	public void removeFrame(SwingScilabFrame frame) {
		// for now no difference from removing a widget
		remove((JComponent) frame);
	}
	
	/**
	 * Get the displacement in pixel that should be used for rotating axes
	 * @param displacement out parameter, [x,y] array of displacement in pixels
	 * @return true if the displacement recording continue, false otherwise
	 */
	public boolean getRotationDisplacement(int[] displacement) {
		return rotationTracker.getDisplacement(displacement);
	}
	
	/**
	 * Ansynchrnous stop of rotation tracking.
	 */
	public void stopRotationRecording() {
		rotationTracker.cancelRecording();
	}

}
