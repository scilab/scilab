/*------------------------------------------------------------------------*/
/* file: FigureCanvasMapper.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains the mapping between figure number and canvas           */
/*------------------------------------------------------------------------*/

package org.scilab.modules.graphics;

import java.util.HashMap;
import org.scilab.modules.gui.bridge.SwingScilabCanvas;

/**
 * Glbal variable, mapping between Scilab figure number its correspondign canvas
 * @author Jean-Baptiste Silvy
 */
public final class FigureCanvasMapper {
	
	/**
	 * We can't use directly HashMap.
	 */
	private static class FigureCanvasMap extends HashMap<Integer, SwingScilabCanvas> {	
		/**
		 * To remove warning
		 */
		private static final long serialVersionUID = 8320117573585296798L;

		/**
		 * Default Constructor
		 */
		public FigureCanvasMap() {
			super();
		}
	}
	
	/**
	 * The hashmap containing the mapping between figures id (int) and canvas.
	 */
	private static FigureCanvasMap mapper = new FigureCanvasMap();
	
	/**
	 * Default constructor. This is a utility class, there should not be any instance
	 */
	private FigureCanvasMapper() { }
	
	/**
	 * Add a new mapping between a figure and canvas
	 * @param figureId id of the figure
	 * @param canvas corresponding canvas
	 */
	public static void addMapping(int figureId, SwingScilabCanvas canvas) {
		mapper.put(figureId, canvas);
	}
	
	/**
	 * Remove a canvas from the mapping
	 * @param figureId id of the figure
	 */
	public static void removeMapping(int figureId) {
		mapper.remove(figureId);
	}
	
	/**
	 * Get the canvas on which the figure is drawn.
	 * @param figureId id of the figure
	 * @return the corresponding canvas
	 */
	public static SwingScilabCanvas getCorrespondingCanvas(int figureId) {
		return mapper.get(figureId);
	}
	
}
