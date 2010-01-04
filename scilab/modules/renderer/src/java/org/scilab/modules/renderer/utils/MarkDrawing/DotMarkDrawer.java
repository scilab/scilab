/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing dot marks
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.MarkDrawing;

import javax.media.opengl.glu.GLU;
import javax.media.opengl.GL;


/**
 * Class specialized in drawing dot marks 
 * @author Jean-Baptiste Silvy
 */
public class DotMarkDrawer extends MarkDrawingStrategy  {

	
	private static final int NB_SLICES = 64;
	
	/**
	 * Default constructor
	 */
	public DotMarkDrawer() {
		super();
	}
	
	/**
	 * Draw a dot
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public void drawMark(GL gl, double[] backColor, double[] frontColor) {
    if(!isFrontTransparent)
    {
		  gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		  GLU glu = new GLU();
		  glu.gluDisk(glu.gluNewQuadric(), 0.0, 1.0, NB_SLICES, 1);
    }
	}
}
