/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains the mapping between figure number and canvas
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer;

import java.util.HashMap;

import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

/**
 * Glbal variable, mapping between Scilab figure number its correspondign canvas
 * @author Jean-Baptiste Silvy
 */
public final class FigureMapper {
	
	/**
	 * We can't use directly HashMap.
	 */
	private static class FigureMap extends HashMap<Integer, DrawableFigureGL> {	
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
	public static synchronized void addMapping(int figureId, DrawableFigureGL figure) {
		// chack if the figure is not already in the list
		if (!containsFigure(figure)) {
			MAPPER.put(figureId, figure);
		}
	}
	
	/**
	 * Remove a canvas from the mapping
	 * @param figureId id of the figure
	 */
	public static synchronized void removeMapping(int figureId) {
		MAPPER.remove(figureId);
	}
	
	/**
	 * Get the canvas on which the figure is drawn.
	 * @param figureId id of the figure
	 * @return the corresponding canvas
	 */
	public static synchronized DrawableFigureGL getCorrespondingFigure(int figureId) {
		return MAPPER.get(figureId);
	}
	
	/**
	 * Returns true if the figure already exists in the hashmap
	 * @param figure figure object to be searched
	 * @return true if the figure already exists in the hashmap, false otherwise
	 */
	public static synchronized boolean containsFigure(DrawableFigureGL figure) {
		return MAPPER.containsValue(figure);
	}
	
}
