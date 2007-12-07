/*------------------------------------------------------------------------*/
/* file: XTicksDrawerGL.java                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing ticks for the X axis                              */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import javax.media.opengl.GL;


/**
 * Class drawing ticks for the X axis
 * @author Jean-Baptiste Silvy
 */
public class XTicksDrawerGL extends TicksDrawerGL {

	/**
	 * Default consturctor.
	 */
	public XTicksDrawerGL() {
		super();
	}
	
	/**
	 * Draw ticks from the recorded data.
	 */
	public void drawTicks() {
		GL gl = getGL();
		
		gl.glColor3d(1.0, 0.0, 0.0);
		gl.glPointSize(5.0f);
		gl.glBegin(GL.GL_POINTS);
		for (int i = 0; i < getNbTicks(); i++) {
			gl.glVertex3d(getTickPosition(i), 0.0, 0.0);
		}
		gl.glEnd();
		
		
		gl.glPointSize(2.5f);
		gl.glBegin(GL.GL_POINTS);
		for (int i = 0; i < getNbSubticks(); i++) {
			gl.glVertex3d(getSubtickPosition(i), 0.0, 0.0);
		}
		gl.glEnd();
	}

}
