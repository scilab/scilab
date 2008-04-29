/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw inside
 * of an arc object  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.arcDrawing;


import javax.media.opengl.GL;

import org.scilab.modules.renderer.drawers.FillDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing the driver dependant routines to draw inside of an arc object.
 * @author Jean-Baptiste Silvy
 */
public class ArcFillDrawerGL extends FillDrawerGL implements ArcDrawerStrategy {

	
	private ArcFillTools drawer;
	
	/**
	 * Default constructor
	 */
	public ArcFillDrawerGL() {
		super();
		drawer = null;
	}
	
	/**
	 * create the display list for the arc
	 * @param centerX X coordinate of the arc center
	 * @param centerY Y coordinate of the arc center
	 * @param centerZ Z coordinate of the arc center
	 * @param semiMinorAxisX X coordinate of the ellipse minor axis
	 * @param semiMinorAxisY Y coordinate of the ellipse minor axis
	 * @param semiMinorAxisZ Z coordinate of the ellipse minor axis
	 * @param semiMajorAxisX X coordinate of the ellipse major axis
	 * @param semiMajorAxisY Y coordinate of the ellipse minor axis
	 * @param semiMajorAxisZ Z coordinate of the ellipse minor axis
	 * @param startAngle lowest angle in radian, relative to the semi-major axis of the ellipse.
     *                   Start of the angular part to draw.
	 * @param endAngle highest angle in radian, relative to the semi-major axis of the ellipse.
     *                 End of the angular part to draw
	 */
	public void drawArc(double centerX, double centerY, double centerZ,
						double semiMinorAxisX, double semiMinorAxisY, double semiMinorAxisZ,
						double semiMajorAxisX, double semiMajorAxisY, double semiMajorAxisZ,
						double startAngle, double endAngle) {
		
		Vector3D center = new Vector3D(centerX, centerY, centerZ);
		Vector3D semiMinorAxis = new Vector3D(semiMinorAxisX, semiMinorAxisY, semiMinorAxisZ);
		Vector3D semiMajorAxis = new Vector3D(semiMajorAxisX, semiMajorAxisY, semiMajorAxisZ);
	
		ArcRendererFactory arcFactory = getParentFigureGL().getArcRendererFactory();
		drawer = arcFactory.createArcFillRenderer(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle);
		
		drawArc();
		
	}
	
	/**
	 * Redraw the arc using precomputed data
	 */
	public void redrawArc() {
		drawArc();
	}
	
	/**
	 * Call gl routine to draw to the inside of the arc.
	 */
	public void drawArc() {
		GL gl = getGL();
		
		drawer.beginRendering(gl, getBackColor());
		drawer.drawCircle(gl);
		drawer.endRendering(gl);
	}
	
}
