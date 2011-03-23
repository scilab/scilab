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

import java.nio.Buffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import javax.media.opengl.DebugGL;
import javax.media.opengl.GL;

/**
 * GL2PSGL
 * @author Sylvestre Koumar
 *
 */
public class GL2PSGL extends DebugGL {
	
	private GL gl;
	private GL2PS gl2ps;
	
	/**
	 * 
	 * @param gl GL
	 * @param gl2ps GL2PS
	 */
	public GL2PSGL(GL gl, GL2PS gl2ps) {
		super(gl);
		this.gl = gl;
		this.gl2ps = gl2ps;	
	}

	/**
	 * GL2PS getter
	 * @return gl2ps GL2PS
	 */
	public GL2PS getGl2ps() {
		return gl2ps;
	}

	/**
	 * GL2PS setter
	 * @param gl2ps GL2PS
	 */
	public void setGl2ps(GL2PS gl2ps) {
		this.gl2ps = gl2ps;
	}


	/**
	 * glEnable
	 * @param enable enable value
	 */
	public void glEnable(int enable) {
		gl.glEnable(enable);
		switch (enable) {
		case GL.GL_LINE_STIPPLE:
			gl2ps.gl2psEnable(GL2PS.GL2PS_LINE_STIPPLE);
			break;
		// we use simple line offset in gl2ps begin page, so disable it
		/*case GL.GL_POLYGON_OFFSET_FILL:
			gl2ps.gl2psEnable(GL2PS.GL2PS_POLYGON_OFFSET_FILL);
			break;*/
		case GL.GL_BLEND:
			gl2ps.gl2psEnable(GL2PS.GL2PS_BLEND);
			break;
	    // not yet implemented in GL2PS
		//case GL.GL_POLYGON_BOUNDARY):
			//gl2ps.gl2psEnable(GL2PS.GL2PS_POLYGON_BOUNDARY);
			//break;
		default:
			break;
		}
	}
	
	/**
	 * glDisable
	 * @param disable disable value
	 */
	public void glDisable(int disable) {
		gl.glDisable(disable);
		
		switch (disable) {
		case GL.GL_LINE_STIPPLE:
			gl2ps.gl2psDisable(GL2PS.GL2PS_LINE_STIPPLE);
			break;
		// we use simple line offset in gl2ps begin page, so disable it
		/*case GL.GL_POLYGON_OFFSET_FILL:
			gl2ps.gl2psDisable(GL2PS.GL2PS_POLYGON_OFFSET_FILL);
			break;*/
		case GL.GL_BLEND:
			gl2ps.gl2psDisable(GL2PS.GL2PS_BLEND);
			break;
	    // not yet implemented in GL2PS
		//case GL.GL_POLYGON_BOUNDARY):
			//gl2ps.gl2psDisable(GL2PS.GL2PS_POLYGON_BOUNDARY);
			//break;
		default:
			break;
		}

	}
	
	/**
	 * glPointSize
	 * @param pointSize point size value
	 */
	public void glPointSize(float pointSize) {
		gl.glPointSize(pointSize);
		gl2ps.gl2psPointSize(pointSize);	
	}
	
	/**
	 * glLineWidth
	 * @param lineWidht line width value
	 */
	public void glLineWidth(float lineWidht) {
		gl.glLineWidth(lineWidht);
		gl2ps.gl2psLineWidth(lineWidht);			
	}
	
	/**
	 * glBlendFunc
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glBlendFunc(int arg0, int arg1) {
		gl.glBlendFunc(arg0, arg1);
		gl2ps.gl2psBlendFunc(arg0, arg1);
	}
	
	/**
	 * glDeleteLists
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glDeleteLists(int arg0, int arg1) {		
		gl.glDeleteLists(arg0, arg1);
	}
	
	/**
	 * glGenLists
	 * @param arg0 blend function value
	 * @return 0
	 */
	public int glGenLists(int arg0) {
		return gl.glGenLists(arg0);
	}
	
	/**
	 * glEndList
	 */
	public void glEndList() {		
		gl.glEndList();
	}
	
	/**
	 * glCallList
	 * @param arg0 blend function value
	 */
	public void glCallList(int arg0) {
		gl.glCallList(arg0);	
	}
	
	/**
	 * glColor3d
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glColor3d(double arg0, double arg1, double arg2) {
		gl.glColor3d(arg0, arg1, arg2);		
	}
	
	/**
	 * glPushMatrix
	 */
	public void glPushMatrix() {
		gl.glPushMatrix();		
	}
	
	/**
	 * glPopMatrix
	 */
	public void glPopMatrix() {
		gl.glPopMatrix();	
	}
	
	/**
	 * glTranslated
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glTranslated(double arg0, double arg1, double arg2) {
		gl.glTranslated(arg0, arg1, arg2);	
	}
	
	/**
	 * glMultMatrixd
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glMultMatrixd(double[] arg0, int arg1) {
		gl.glMultMatrixd(arg0, arg1);		
	}
	
	/**
	 * glScaled
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glScaled(double arg0, double arg1, double arg2) {
		gl.glScaled(arg0, arg1, arg2);	
	}
	
	/**
	 * glRotated
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 */
	public void glRotated(double arg0, double arg1, double arg2, double arg3) {
		gl.glRotated(arg0, arg1, arg2, arg3);	
	}
	
	/**
	 * glBegin
	 * @param arg0 blend function value
	 */
	public void glBegin(int arg0) {
		gl.glBegin(arg0);		
	}
	
	/**
	 * glEnd
	 */
	public void glEnd() {
		gl.glEnd();		
	}
	
	/**
	 * glVertex3d
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glVertex3d(double arg0, double arg1, double arg2) {
		gl.glVertex3d(arg0, arg1, arg2);		
	}
	
	/**
	 * glClear
	 * @param arg0 blend function value
	 */
	public void glClear(int arg0) {
		gl.glClear(arg0);		
	}
	
	/**
	 * glClearDepth
	 * @param arg0 blend function value
	 */
	public void glClearDepth(double arg0) {
		gl.glClearDepth(arg0);		
	}
	
	/**
	 * glClearColor
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 */
	public void glClearColor(float arg0, float arg1, float arg2, float arg3) {
		gl.glClearColor(arg0, arg1, arg2, arg3);		
	}
	
	/**
	 * glLogicOp
	 * @param arg0 blend function value
	 */
	public void glLogicOp(int arg0) {
		gl.glLogicOp(arg0);		
	}
	
	/**
	 * glShadeModel
	 * @param arg0 blend function value
	 */
	public void glShadeModel(int arg0) {
		gl.glShadeModel(arg0);	
		
	}
	
	/**
	 * glDepthFunc
	 * @param arg0 blend function value
	 */
	public void glDepthFunc(int arg0) {
		gl.glDepthFunc(arg0);		
	}
	
	/**
	 * glHint
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glHint(int arg0, int arg1) {
		gl.glHint(arg0, arg1);		
	}
	
	/**
	 * glVertex3dv
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glVertex3dv(double[] arg0, int arg1) {
		gl.glVertex3dv(arg0, arg1);		
	}
	
	/**
	 * glCullFace
	 * @param arg0 blend function value
	 */
	public void glCullFace(int arg0) {
		gl.glCullFace(arg0);		
	}
	
	/**
	 * glPolygonOffset
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glPolygonOffset(float arg0, float arg1) {
		gl.glPolygonOffset(arg0, arg1);			
	}
	
	/**
	 * glMatrixMode
	 * @param arg0 blend function value
	 */
	public void glMatrixMode(int arg0) {
		gl.glMatrixMode(arg0);		
	}
	
	/**
	 * glLoadIdentity
	 */
	public void glLoadIdentity() {
		gl.glLoadIdentity();		
	}
	
	/**
	 * glGetDoublev
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glGetDoublev(int arg0, double[] arg1, int arg2) {
		gl.glGetDoublev(arg0, arg1, arg2);			
	}
	
	/**
	 * glTexCoord1d
	 * @param arg0 blend function value
	 */
	public void glTexCoord1d(double arg0) {
		gl.glTexCoord1d(arg0);			
	}
	
	/**
	 * glClipPlane
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glClipPlane(int arg0, double[] arg1, int arg2) {
		gl.glClipPlane(arg0, arg1, arg2);		
	}
	
	/**
	 * glLineStipple
	 * @param arg0 stipple factor.
	 * @param arg1 stipple patern.
	 */
	public void glLineStipple(int arg0, short arg1) {
		gl.glLineStipple(arg0, arg1);		
		gl2ps.gl2psLineStipple(arg0, arg1);
	}
	
	/**
	 * glOrtho
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 * @param arg4 blend function value
	 * @param arg5 blend function value
	 */
	public void glOrtho(double arg0, double arg1, double arg2, double arg3,
			double arg4, double arg5) {
		gl.glOrtho(arg0, arg1, arg2, arg3, arg4, arg5);		
	}
	
	/**
	 * glGetIntegerv
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glGetIntegerv(int arg0, int[] arg1, int arg2) {
		gl.glGetIntegerv(arg0, arg1, arg2);
	}
	
	/**
	 * glColor4f
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 */
	public void glColor4f(float arg0, float arg1, float arg2, float arg3) {
		gl.glColor4f(arg0, arg1, arg2, arg3);
	}
	
	/**
	 * glPushAttrib
	 * @param arg0 blend function value
	 */
	public void glPushAttrib(int arg0) {
		gl.glPushAttrib(arg0);
	}
	
	/**
	 * glGenTextures
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glGenTextures(int arg0, int[] arg1, int arg2) {
		gl.glGenTextures(arg0, arg1, arg2);
	}
	
	/**
	 * isExtensionAvailable
	 * @param arg0 blend function value
	 * @return gl.isExtensionAvailable(arg0)
	 */
	public boolean isExtensionAvailable(String arg0) {
		return gl.isExtensionAvailable(arg0);
	}
	
	/**
	 * glBindTexture
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glBindTexture(int arg0, int arg1) {
		gl.glBindTexture(arg0, arg1);
	}
	
	/**
	 * glTexImage2D
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 * @param arg4 blend function value
	 * @param arg5 blend function value
	 * @param arg6 blend function value
	 * @param arg7 blend function value
	 * @param arg8 blend function value
	 * 
	 */
	public void glTexImage2D(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, long arg8) {
		gl.glTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	}
	
	/**
	 * glTexImage2D
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 * @param arg4 blend function value
	 * @param arg5 blend function value
	 * @param arg6 blend function value
	 * @param arg7 blend function value
	 * @param arg8 blend function value
	 */
	public void glTexImage2D(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, Buffer arg8) {
		gl.glTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	}
	
	/**
	 * glIsEnabled
	 * @param arg0 blend function value
	 * @return gl.glIsEnabled(arg0)
	 */
	public boolean glIsEnabled(int arg0) {
		return gl.glIsEnabled(arg0);
	}
	
	/**
	 * glNewList
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glNewList(int arg0, int arg1) {
		gl.glNewList(arg0, arg1);
	}
	
	/**
	 * glPixelStorei
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glPixelStorei(int arg0, int arg1) {
		gl.glPixelStorei(arg0, arg1);
	}
	
	/**
	 * glTexSubImage2D
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 * @param arg4 blend function value
	 * @param arg5 blend function value
	 * @param arg6 blend function value
	 * @param arg7 blend function value
	 * @param arg8 blend function value
	 */
	public void glTexSubImage2D(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, long arg8) {
		gl.glTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	}
	
	/**
	 * glTexSubImage2D
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 * @param arg4 blend function value
	 * @param arg5 blend function value
	 * @param arg6 blend function value
	 * @param arg7 blend function value
	 * @param arg8 blend function value
	 */
	public void glTexSubImage2D(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, Buffer arg8) {
		gl.glTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	}
	
	/**
	 * glTexParameteri
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glTexParameteri(int arg0, int arg1, int arg2) {
		gl.glTexParameteri(arg0, arg1, arg2);
	}
	
	/**
	 * glTexEnvi
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glTexEnvi(int arg0, int arg1, int arg2) {
		gl.glTexEnvi(arg0, arg1, arg2);
	}
	
	/**
	 * glPushClientAttrib
	 * @param arg0 blend function value
	 */
	public void glPushClientAttrib(int arg0) {
		gl.glPushClientAttrib(arg0);
	}
	
	/**
	 * glGenBuffersARB
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glGenBuffersARB(int arg0, int[] arg1, int arg2) {
		gl.glGenBuffersARB(arg0, arg1, arg2);
	}
	
	/**
	 * glGenBuffersARB
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glGenBuffersARB(int arg0, IntBuffer arg1) {
		gl.glGenBuffersARB(arg0, arg1);
	}
	
	/**
	 * glPopClientAttrib
	 */
	public void glPopClientAttrib() {
		gl.glPopClientAttrib();
	}
	
	/**
	 * glPopAttrib
	 */
	public void glPopAttrib() {
		gl.glPopAttrib();
	}
	
	/**
	 * glBindBuffer
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 */
	public void glBindBuffer(int arg0, int arg1) {
		gl.glBindBuffer(arg0, arg1);
	}
	
	/**
	 * glBufferData
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 */
	public void glBufferData(int arg0, int arg1, Buffer arg2, int arg3) {
		gl.glBufferData(arg0, arg1, arg2, arg3);
	}
	
	/**
	 * glBufferSubData
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 */
	public void glBufferSubData(int arg0, int arg1, int arg2, Buffer arg3) {
		gl.glBufferSubData(arg0, arg1, arg2, arg3);
	}
	
	/**
	 * glEnableClientState
	 * @param arg0 blend function value
	 */
	public void glEnableClientState(int arg0) {
		gl.glEnableClientState(arg0);
	}
	
	/**
	 * glVertexPointer
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 */
	public void glVertexPointer(int arg0, int arg1, int arg2, long arg3) {
		gl.glVertexPointer(arg0, arg1, arg2, arg3);
	}
	
	/**
	 * glVertexPointer
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 */
	public void glVertexPointer(int arg0, int arg1, int arg2, Buffer arg3) {
		gl.glVertexPointer(arg0, arg1, arg2, arg3);
	}
	
	/**
	 * glClientActiveTexture
	 * @param arg0 blend function value
	 */
	public void glClientActiveTexture(int arg0) {
		gl.glClientActiveTexture(arg0);
	}
	
	/**
	 * glTexCoordPointer
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 */
	public void glTexCoordPointer(int arg0, int arg1, int arg2, long arg3) {
		gl.glTexCoordPointer(arg0, arg1, arg2, arg3);
	}
	
	/**
	 * glTexCoordPointer
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 * @param arg3 blend function value
	 */
	public void glTexCoordPointer(int arg0, int arg1, int arg2, Buffer arg3) {
		gl.glTexCoordPointer(arg0, arg1, arg2, arg3);
	}
	
	/**
	 * glDrawArrays
	 * @param arg0 blend function value
	 * @param arg1 blend function value
	 * @param arg2 blend function value
	 */
	public void glDrawArrays(int arg0, int arg1, int arg2) {
		gl.glDrawArrays(arg0, arg1, arg2);
	}	
	
	/**
	 * glPassThrough
	 * @param arg0 blend function value
	 */
	public void glPassThrough(float arg0) {
		gl.glPassThrough(arg0);
	}
	
	/**
	 * glRenderMode
	 * @param arg0 blend function value
	 * @return gl.glRenderMode(arg0)
	 */
	public int glRenderMode(int arg0) {
		return gl.glRenderMode(arg0);
	}
	
	/**
	 * glGetFloatv
	 * @param arg0 int
	 * @param arg1 FloatBuffer
	 */
	public void glGetFloatv(int arg0, FloatBuffer arg1) {
		gl.glGetFloatv(arg0, arg1);
	}
	
	/**
	 * glGetFloatv
	 * @param arg0 int
	 * @param arg1 float[]
	 * @param arg2 int
	 */
	public void glGetFloatv(int arg0, float[] arg1, int arg2) {
		gl.glGetFloatv(arg0, arg1, arg2);
	}
	
	/**
	 * glRasterPos3d
	 * @param arg0 double
	 * @param arg1 double
	 * @param arg2 double
	 */
	public void glRasterPos3d(double arg0, double arg1, double arg2) {
		gl.glRasterPos3d(arg0, arg1, arg2);
	}
	
	/**
	 * glGetBooleanv
	 * @param arg0 int 
	 * @param arg1 java.nio.ByteBuffer
	 */
	public void glGetBooleanv(int arg0, java.nio.ByteBuffer arg1) {
		gl.glGetBooleanv(arg0, arg1);
	}
	
	/**
	 * glGetBooleanv
	 * @param arg0 int 
	 * @param arg1 byte[]
	 * @param arg2 int 
	 */
	public void glGetBooleanv(int arg0, byte[] arg1, int arg2) {
		gl.glGetBooleanv(arg0, arg1, arg2);
	}
	
	/**
	 * glRasterPos2d
	 * @param arg0 double
	 * @param arg1 double
	 */
	public void glRasterPos2d(double arg0, double arg1) {
		gl.glRasterPos2d(arg0, arg1);
	}
	
	/**
	 * glRasterPos3dv
	 * @param arg0 double[]
	 * @param arg1 int
	 */
	public void glRasterPos3dv(double[] arg0, int arg1) {
		gl.glRasterPos3dv(arg0, arg1);
	}
	
	/**
	 * glNormal3f
	 * @param arg0 float
	 * @param arg1 float
	 * @param arg2 float
	 */
	public void glNormal3f(float arg0, float arg1, float arg2) {
		gl.glNormal3f(arg0, arg1, arg2);
	}
	
	/**
	 * glColorMask
	 * @param arg0 boolean
	 * @param arg1 boolean
	 * @param arg2 boolean
	 * @param arg3 boolean
	 */
	public void glColorMask(boolean arg0, boolean arg1, boolean arg2, boolean arg3) {
		gl.glColorMask(arg0, arg1, arg2, arg3);
	}
	
	/**
	 * glIsList
	 * @param arg0 int
	 * @return gl.glIsList(arg0) boolean
	 */
	public boolean glIsList(int arg0) {
		return gl.glIsList(arg0);
	}
	
	/**
	 * glColor3dv
	 * @param arg0 double[]
	 * @param arg1 int
	 */
	public void glColor3dv(double[] arg0, int arg1) {
		gl.glColor3dv(arg0, arg1);
	}
	
	/**
	 * glDeleteTextures
	 * @param arg0 int
	 * @param arg1 int[]
	 * @param arg2 int
	 */
	public void glDeleteTextures(int arg0, int[] arg1, int arg2) {
		gl.glDeleteTextures(arg0, arg1, arg2);
	}
	
}
