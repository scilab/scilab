/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
 package org.scilab.modules.graphic_export;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;

import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.arcDrawing.FastArcRendererFactory;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.figureDrawing.SciRenderer;
import org.scilab.modules.renderer.polylineDrawing.GL2PSShadeFacetDrawer;

/**
 * 
 * @author Sylvestre Koumar
 *
 */
public class GL2PSRenderer extends ExportRenderer {
	
	/** BufferSize WIDTH*/
	public static final int BUFFER_WIDTH = 2048;
	/** BufferSize HEIGHT*/
	public static final int BUFFER_HEIGHT = 2048;
	
	private SciRenderer sciRend;
	private int figureIndex;
	private int format;

	
	/**
	 * GL2PSRenderer
	 * @param figureIndex index of the figure
	 * @param fileName file name
	 * @param fileType file type
	 */
	public GL2PSRenderer(int figureIndex, String fileName, int fileType) {
		super(fileName, fileType);
		this.figureIndex = figureIndex;
		setGL2PSFile(fileName, fileType);		
	}
	
	/**
	 * setGL2PSFile 
	 * @param fileName fileName
	 * @param fileType fileName
	 * @return ExportRenderer.INVALID_FILE if the file is not valid
	 */
	public int setGL2PSFile(String fileName, int fileType) {
		
		switch (getFileType()) {
		case ExportRenderer.EPS_EXPORT:  setFileName(ExportRenderer.getFileName() + ".eps");
									   format = GL2PS.GL2PS_EPS;
		break;
		case ExportRenderer.PDF_EXPORT:  setFileName(ExportRenderer.getFileName() + ".pdf");
									   format = GL2PS.GL2PS_PDF;
		break;
		case ExportRenderer.SVG_EXPORT:  setFileName(ExportRenderer.getFileName() + ".svg");
									   format = GL2PS.GL2PS_SVG;
		break;				  
		case ExportRenderer.PS_EXPORT:  setFileName(ExportRenderer.getFileName() + ".ps");
		   format = GL2PS.GL2PS_PS;
		break;	
		default: return ExportRenderer.INVALID_FILE;
		}
		return ExportRenderer.SUCCESS;			
	}
	
	/**
	 * display
	 * @param gLDrawable gLDrawable
	 */
	public void display(GLAutoDrawable gLDrawable) {
		GL2PS gl2ps = new GL2PS();

		int buffsize = BUFFER_WIDTH * BUFFER_HEIGHT;			
		sciRend = new SciRenderer(this.figureIndex);
		
		//int[] viewPort = {0, 0, gLDrawable.getWidth(), gLDrawable.getHeight()};
				
		//GL gl = gLDrawable.getGL();
		//gl.glGetIntegerv(GL.GL_VIEWPORT, viewPort, 0);
		

		gl2ps.gl2psBeginPage("MyTitle", "MySoftware", null, format, 
							  GL2PS.GL2PS_SIMPLE_SORT, GL2PS.GL2PS_USE_CURRENT_VIEWPORT | GL2PS.GL2PS_BEST_ROOT
							  /*| GL2PS.GL2PS_OCCLUSION_CULL | GL2PS.GL2PS_LANDSCAPE | GL2PS.GL2PS_DRAW_BACKGROUND*/,
							  GL.GL_RGBA, 0, null, null, null, null, 
							  0, 0, 0, buffsize, ExportRenderer.getFileName());		
		
		
		GL gl = gLDrawable.getGL();
		GL2PSGL newGL = new GL2PSGL(gl, gl2ps);
		gLDrawable.setGL(newGL);
		
		DrawableFigureGL exportedFigure = FigureMapper.getCorrespondingFigure(figureIndex);
		exportedFigure.setTextRendererFactory(new PSTextRendererFactory());
		exportedFigure.setArcRendererFactory(new FastArcRendererFactory());
		exportedFigure.setShadeFacetDrawer(new GL2PSShadeFacetDrawer());
		
		sciRend.init(gLDrawable);
		sciRend.display(gLDrawable);
		
		gl2ps.gl2psEndPage();
		
		gLDrawable.setGL(gl);
		exportedFigure.setDefaultArcRendererFactory();
		exportedFigure.setDefaultTextRenderer();
		exportedFigure.setDefaultShadeFacetDrawer();
		
		sciRend.init(gLDrawable);
		sciRend.display(gLDrawable);
	}


	/**
	 * GLEventListener method
	 * @param arg0 GLAutoDrawable
	 * @param arg1 boolean
	 * @param arg2 boolean
	 */
	public void displayChanged(GLAutoDrawable arg0, boolean arg1, boolean arg2) {
		
	}

	/**
	 * GLEventListener method
	 * @param arg0 GLAutoDrawable
	 */
	public void init(GLAutoDrawable arg0) {
		
	}

	/**
	 * GLEventListener method
	 * @param arg0 GLAutoDrawable
	 * @param arg1 int
	 * @param arg2 int
	 * @param arg3 int
	 * @param arg4 int
	 */
	public void reshape(GLAutoDrawable arg0, int arg1, int arg2, int arg3,
			int arg4) {
	}
	
}
