/*------------------------------------------------------------------------*/
/* file: FigureMapper.java                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains the mapping between figure number and canvas           */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer;

import java.util.HashMap;

import org.scilab.modules.renderer.figureDrawing.DrawableFigureJoGL;

/**
 * Glbal variable, mapping between Scilab figure number its correspondign canvas
 * @author Jean-Baptiste Silvy
 */
public final class FigureMapper {
	
	/**
	 * We can't use directly HashMap.
	 */
	private static class FigureMap extends HashMap<Integer, DrawableFigureJoGL> {	
		/**
		 * To remove warning
		 */
		private static final long serialVersionUID = 8320117573585296798L;

		/**
		 * Default Constructor
		 */
		public FigureMap() {
			super();
		}
	}
	
	/**
	 * The hashmap containing the mapping between figures id (int) and canvas.
	 */
	private static final FigureMap MAPPER = new FigureMap();
	
	/**
	 * Default constructor. This is a utility class, there should not be any instance
	 */
	private FigureMapper() { }
	
	/**
	 * Add a new mapping between a figure and canvas
	 * @param figureId id of the figure
	 * @param figure corresponding figure
	 */
	public static void addMapping(int figureId, DrawableFigureJoGL figure) {
		MAPPER.put(figureId, figure);
	}
	
	/**
	 * Remove a canvas from the mapping
	 * @param figureId id of the figure
	 */
	public static void removeMapping(int figureId) {
		MAPPER.remove(figureId);
	}
	
	/**
	 * Get the canvas on which the figure is drawn.
	 * @param figureId id of the figure
	 * @return the corresponding canvas
	 */
	public static DrawableFigureJoGL getCorrespondingFigure(int figureId) {
		return MAPPER.get(figureId);
	}
	
	/**
	 * Returns true if the figure already exists in the hashmap
	 * @param figure figure object to be searched
	 * @return true if the figure already exists in the hashmap, false otherwise
	 */
	public static boolean containsFigure(DrawableFigureJoGL figure) {
		return MAPPER.containsValue(figure);
	}
	
}
