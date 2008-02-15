/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Static class used to create file export of graphic figures
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

/**
 * Static class used to create file export of graphic figures
 * @author Jean-Baptiste Silvy
 */
public class FileExporter {

	/**
	 * Defualt constuctor
	 */
	protected FileExporter() {
		
	}
	
	/**
	 * Export a figure into a file
	 * @param figureIndex index of tghe figure to export
	 * @param fileName name of the file to create
	 * @param fileType kind of the file
	 * @return 0 if everything worked fine, a non null integer if an exception occured
	 *         depending on the kind of error
	 */
	public static int fileExport(int figureIndex, String fileName, int fileType) {
		DrawableFigureGL exportedFig = FigureMapper.getCorrespondingFigure(figureIndex);
		
		if (exportedFig == null) {
			// figure no longer exists
			return ExportRenderer.IOEXCEPTION_ERROR;
		}		
		
		ExportRenderer export;		
		export = ExportRenderer.createExporter(figureIndex, fileName, fileType);
				
		exportedFig.getRenderingTarget().addGLEventListener(export);
		exportedFig.drawCanvas();
		exportedFig.getRenderingTarget().removeGLEventListener(export);

		return ExportRenderer.SUCCESS;		
	}	
}
