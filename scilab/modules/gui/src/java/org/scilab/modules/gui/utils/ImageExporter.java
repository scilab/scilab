/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Image;

import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;


/**
 * Class specialized in exporting graphic rendering into an image.
 * @author Jean-Baptiste Silvy
 */
public final class ImageExporter {

	/**
	 * Default consturctor
	 */
	protected ImageExporter() {
		
	}
	
	/**
	 * Export a figure into an Image.
	 * Might be used to copy it into a file or clipboard
	 * @param figureIndex index of the figure to export
	 * @return copy of the OpenGL canvas of the figure or null if an error occured
	 */
	public static Image imageExport(int figureIndex) {
		DrawableFigureGL exportedFig = FigureMapper.getCorrespondingFigure(figureIndex);
		
		if (exportedFig == null) {
			// figure no longer exists
			return null;
		}	
	
		ImageExportRenderer exporter = new ImageExportRenderer();
		
		// export canvas
		return exporter.exportToImage(exportedFig.getRenderingTarget());
	}
	
}
