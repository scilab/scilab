/*------------------------------------------------------------------------*/
/* file: FigureCanvas.java                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Canvas used to draw a figure object                             */
/*------------------------------------------------------------------------*/


package org.scilab.modules.graphics.figureDrawing;

import org.scilab.modules.gui.bridge.SwingScilabCanvas;
import javax.media.opengl.GLCapabilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Canvas Canvas used to draw a figure object
 * @author Jean-Baptiste Silvy
 */
public class FigureCanvas extends SwingScilabCanvas {

	/** To avoid warnings */
	private static final long serialVersionUID = -1644301797376550220L;
	
	/** Default size for the canvas */
	private static final Size DEFAULT_SIZE = new Size(640, 480);
	
	/**
	 * Constructor
	 * @param cap GLCapabilities associated to the GLJPanel
	 */
	public FigureCanvas(GLCapabilities cap) {
		super(cap);
	}
	
	/**
	 * Create a Scilab Canvas
	 * @return the created canvas
	 */
	public static FigureCanvas create() {
		GLCapabilities cap = new GLCapabilities();
		cap.setDoubleBuffered(true);
		FigureCanvas res = new FigureCanvas(cap);
		res.setDims(DEFAULT_SIZE);
		return res;
	}
	
	
}
