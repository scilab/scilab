/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing pentagram stars marks
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */



package org.scilab.modules.renderer.utils.MarkDrawing;

import javax.media.opengl.GL;

/**
 * Class specialized in drawing pentagram stars marks
 * The star has ten vertices, 5 are on the outside circle, 5 on the inside,
 * both of the two sets are defining a pentagram centered on (0,0).
 * @author Jean-Baptiste Silvy
 */
public class PentagramMarkDrawer extends MarkDrawingStrategy {

	/** 
	 * Distance of the insid epoints from the center
	 * To compute this, I compute the intersection between two edges of the star
	 * (note that each time, 4 points are aligned). Then I compute its distance from
	 * origin. 1.2 is to make the star a bit larger.
	 */
	private static final double INNER_DISTANCE = 1.2 * 0.3819660;
	private static final double OUTER_DISTANCE = 1.2;
	
	/** Some constants */
	private static final double TEN = 10.0;
	private static final double PI_ON_TEN = Math.PI / TEN;
	private static final double COS_PI_ON_TEN = Math.cos(PI_ON_TEN);
	private static final double SIN_PI_ON_TEN = Math.sin(PI_ON_TEN);
	private static final double COS_3_PI_ON_TEN = Math.cos(3.0 * PI_ON_TEN);
	private static final double SIN_3_PI_ON_TEN = Math.sin(3.0 * PI_ON_TEN);
	
	/** number of vertices */
	private static final int NB_VERTICES = 10;
	
	/** array containing vertices coordinates */
	private static final double[][] VERTICES_COORDINATES 
		= {{0.0                              ,  OUTER_DISTANCE                  }, // angle Pi/2, external circle
		   {-INNER_DISTANCE * COS_3_PI_ON_TEN,  INNER_DISTANCE * SIN_3_PI_ON_TEN}, // angle 7.Pi/10, inner circle
		   {-OUTER_DISTANCE * COS_PI_ON_TEN  ,  OUTER_DISTANCE * SIN_PI_ON_TEN  }, // angle 9.Pi/10 exteral circle
		   {-INNER_DISTANCE * COS_PI_ON_TEN  , -INNER_DISTANCE * SIN_PI_ON_TEN  }, // angle -9.Pi/10, inner circle
		   {-OUTER_DISTANCE * COS_3_PI_ON_TEN, -OUTER_DISTANCE * SIN_3_PI_ON_TEN}, // angle -7.Pi/10, external circle
		   {0.0                              , -INNER_DISTANCE                  }, // angle -Pi/2, inner circle
		   {OUTER_DISTANCE * COS_3_PI_ON_TEN , -OUTER_DISTANCE * SIN_3_PI_ON_TEN}, // angle -3.Pi/10, external circle
		   {INNER_DISTANCE * COS_PI_ON_TEN   , -INNER_DISTANCE * SIN_PI_ON_TEN  }, // angle -Pi/10, inner circle
		   {OUTER_DISTANCE * COS_PI_ON_TEN   ,  OUTER_DISTANCE * SIN_PI_ON_TEN   }, // angle Pi/10, external circle
		   {INNER_DISTANCE * COS_3_PI_ON_TEN ,  INNER_DISTANCE * SIN_3_PI_ON_TEN}  // angle 3.Pi/10, inner circle
		  };
	
	/** The Star can be decomposed in three triangle, here are their vertices */
	private static final int NEEDED_TRIANGLE = 3;
	private static final int NB_COORDINATES = 3;
	private static final int[][] THREE_TRIANGLE = {{0, 4, 7}, {0, 3, 6}, {2, 5 , 8}};
	
	/**
	 * Default constructor
	 */
	public PentagramMarkDrawer() {
		super();
	}
	
	/**
	 * Draw a plus
	 * @param gl OpenGL pipeline to use
	 * @param backColor RGB color of mark background
	 * @param frontColor RGB color of mark foreground
	 */
	public void drawMark(GL gl, double[] backColor, double[] frontColor) {
		
		// inside
    if(!isBackTransparent)
    {
		  gl.glColor3d(backColor[0], backColor[1], backColor[2]);
		  gl.glBegin(GL.GL_TRIANGLES);
		  for (int i = 0; i < NEEDED_TRIANGLE; i++) {
			  for (int j = 0; j < NB_COORDINATES; j++) {
				  gl.glVertex3d(VERTICES_COORDINATES[THREE_TRIANGLE[i][j]][0], VERTICES_COORDINATES[THREE_TRIANGLE[i][j]][1], 0.0);
			  }
		  }
		  gl.glEnd();
    }		
		// outline
    if(!isFrontTransparent)
    {
		  gl.glColor3d(frontColor[0], frontColor[1], frontColor[2]);
		  gl.glBegin(GL.GL_LINE_LOOP);
		  for (int i = 0; i < NB_VERTICES; i++) {
			  gl.glVertex3d(VERTICES_COORDINATES[i][0], VERTICES_COORDINATES[i][1], 0.0);
		  }
		  gl.glEnd();
		}
	}

}
