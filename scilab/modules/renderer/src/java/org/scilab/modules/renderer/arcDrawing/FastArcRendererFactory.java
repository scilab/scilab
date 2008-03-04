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

package org.scilab.modules.renderer.arcDrawing;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * FastArcRendererFactory
 * @author Sylvestre Koumar
 *
 */
public class FastArcRendererFactory implements ArcRendererFactory {

	/**
	 * Constructor
	 */
	public FastArcRendererFactory() {
		
	}

	/**
	 * createArcFillRenderer
	 * @param center Vector3D
	 * @param semiMinorAxis Vector3D
	 * @param semiMajorAxis Vector3D
	 * @param startAngle double
	 * @param endAngle double
	 * @return fastArcFillTools ArcFillTools
	 * 
	 */
	public ArcFillTools createArcFillRenderer(Vector3D center,
			Vector3D semiMinorAxis, Vector3D semiMajorAxis, double startAngle,
			double endAngle) {
		FastArcFillTools fastArcFillTools = new FastArcFillTools(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle);
		
		return fastArcFillTools;
	}


	/**
	 * createArcLineRenderer
	 * @param center Vector3D
	 * @param semiMinorAxis Vector3D
	 * @param semiMajorAxis Vector3D
	 * @param startAngle double
	 * @param endAngle double
	 * @return fastArcLineTools ArcFillTools
	 * 
	 */
	public ArcLineTools createArcLineRenderer(Vector3D center,
			Vector3D semiMinorAxis, Vector3D semiMajorAxis, double startAngle,
			double endAngle) {
		FastArcLineTools fastArcLineTools = new FastArcLineTools(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle);
		
		return fastArcLineTools;
	}

}
