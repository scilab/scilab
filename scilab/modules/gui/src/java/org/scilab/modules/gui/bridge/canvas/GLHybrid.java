/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.canvas;

import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GraphicsDevice;
import java.awt.Toolkit;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.nio.ByteBuffer;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCanvas;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLCapabilitiesChooser;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLException;
import javax.swing.JPanel;

import com.sun.opengl.util.Animator;
import com.sun.opengl.util.Screenshot;


public class GLHybrid extends JPanel implements GLAutoDrawable {

    private final static boolean VERBOSE = false;
    private final static boolean TIMED = false;
    private final static boolean SHOWCANVAS = true;
    private final static boolean SHOWFRAME = false;
    private final static boolean ACCELERATESCREENSHOT = false;

    private boolean needReDump = true;

    private final GLCanvas canvas;
    private final Frame frame;
    
    private BufferedImage dump;
    private long tic = 0;
    private long tac = 0;
    private long lastGetGLTime = System.currentTimeMillis();

    private void call(String msg) {
	if(VERBOSE) {
	    System.err.println("[GLHybrid] call --> "+msg);
	}
    }
    private void time(String msg, long tic, long tac) {
	if(TIMED) {
	    System.err.println("[GLHybrid] "+msg+" --> "+(tac - tic)+"ms");
	}
    }

    public GLHybrid() {
	this(null);
	call("GLHybrid()");
    }

    public GLHybrid(GLCapabilities capabilities) {
	this(capabilities, null, null, null);
	call("GLHybrid(GLCapabilities capabilities)");
    }

    public GLHybrid(GLCapabilities capabilities,
	    GLCapabilitiesChooser chooser,
	    GLContext shareWith,
	    GraphicsDevice device) {
	super();
	call("GLHybrid(GLCapabilities capabilities, GLCapabilitiesChooser chooser, GLContext shareWith, GraphicsDevice device)");

	canvas = new GLCanvas(capabilities, chooser, shareWith, device);
	frame = new Frame();
	
	Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
	canvas.setSize(screenSize.width, screenSize.height);
	canvas.setLocation(0, 0);
	canvas.setVisible(SHOWCANVAS);
	frame.setSize(screenSize.width, screenSize.height);
	frame.add(canvas);
	frame.setVisible(SHOWFRAME);
	frame.pack();
	
	// Add animator for display
	Animator animator = new Animator(this);
	animator.start();
    }

    public void paintComponent(final Graphics g) {
	call("paintComponent(final Graphics g)");
    }

    public void paint(Graphics g) {
	call("paint(Graphics g)");
	if (needReDump) {
	    dumpCanvas();
	}
	drawDumpedImage(g);
    }

    private void dumpCanvas()
    {
	call("dumpCanvas()");
	/** DISPLAY */
	tic = System.currentTimeMillis();
	canvas.display();
	tac = System.currentTimeMillis();
	time("canvas.display();", tic, tac);

	/** MAKE CURRENT */
	tic = System.currentTimeMillis();
	canvas.getContext().makeCurrent();
	tac = System.currentTimeMillis();
	time("canvas.getContext().makeCurrent();", tic, tac);

	/** SCREEN SHOT*/
	if (ACCELERATESCREENSHOT) {
	    tic = System.currentTimeMillis();
	    /** --- MY BEAUTIFUL SCREENSHOT --- */
	    BufferedImage image = new BufferedImage(getWidth(), getHeight(),  BufferedImage.TYPE_3BYTE_BGR);
	    GL gl = canvas.getGL();

	    /** Set up pixel storage modes */
	    PixelStorageModes psm = new PixelStorageModes();
	    psm.save(gl);

	    /** read the BGR values into the image */
	    gl.glReadPixels(0, 0, getWidth(), getHeight(), GL.GL_BGR,
		    GL.GL_UNSIGNED_BYTE,
		    ByteBuffer.wrap(((DataBufferByte) image.getRaster().getDataBuffer()).getData()));

	    /** Restore pixel storage modes */
	    psm.restore(gl);

	    /** Flip the image vertically */
	    dump = new BufferedImage(getWidth(), getHeight(), BufferedImage.TYPE_3BYTE_BGR);
	    Graphics2D g2d = dump.createGraphics();
	    AffineTransform at = AffineTransform.getTranslateInstance(0, getHeight());
	    at.scale(1, -1);
	    g2d.drawRenderedImage(image, at);
	    g2d.dispose();
	    /** --- MY BEAUTIFUL SCREENSHOT --- */
	    tac = System.currentTimeMillis();
	    time("My screenshot...", tic, tac);
	} else {
	    tic = System.currentTimeMillis();
	    /** --- JOGL SCREENSHOT --- */
	    dump = Screenshot.readToBufferedImage(getWidth(), getHeight());
	    /** --- JOGL SCREENSHOT --- */
	    tac = System.currentTimeMillis();
	    time("Screenshot.readToBufferedImage...", tic, tac);
	}

	/** RELEASE CONTEXT */
	tic = System.currentTimeMillis();
	canvas.getContext().release();	
	tac = System.currentTimeMillis();
	time("canvas.getContext().release();", tic, tac);

	synchronized (this) {
	    needReDump = false;    
	}
    }

    private void drawDumpedImage(Graphics g) {
	call("drawDumpedImage(Graphics g)");
	/** CANVAS DISPLAY */
	tic = System.currentTimeMillis();
	canvas.display();
	tac = System.currentTimeMillis();
	time("canvas.display();", tic, tac);

	/** DRAWIMAGE */
	tic = System.currentTimeMillis();
	g.drawImage(dump, 0, 0, getWidth(), getHeight(), this);
	tac = System.currentTimeMillis();
	time("g.drawImage...", tic, tac);
    }

    public void repaint() {
	super.repaint();
	call("repaint()");
	if (canvas != null) {
	    canvas.repaint();
	}
    }

    public void addNotify() {
	super.addNotify();
	call("addNotify()");
	canvas.addNotify();
    }

    public void addGLEventListener(GLEventListener listener) {
	call("addGLEventListener(GLEventListener listener)");
	canvas.addGLEventListener(listener);
    }

    public boolean getAutoSwapBufferMode() {
	call("getAutoSwapBufferMode()");
	return canvas.getAutoSwapBufferMode();
    }

    public GLContext getContext() {
	call("getContext()");
	return canvas.getContext();
    }

    public GL getGL() {
	call("getGL()");
	synchronized (this) {
	    long currentTime = System.currentTimeMillis();
	    if(currentTime - lastGetGLTime > 50)
	    {
		lastGetGLTime = currentTime;
		this.needReDump = true;
	    }
	}
	return canvas.getGL();
    }

    public void removeGLEventListener(GLEventListener listener) {
	call("removeGLEventListener(GLEventListener listener)");
	canvas.removeGLEventListener(listener);
    }

    public void setAutoSwapBufferMode(boolean arg0) {
	call("setAutoSwapBufferMode(boolean arg0)");
	canvas.setAutoSwapBufferMode(arg0);
    }

    public void setGL(GL gl) {
	call("setGL(GL gl)");
	canvas.setGL(gl);
    }

    public GLContext createContext(GLContext arg0) {
	call("createContext(GLContext arg0)");
	return canvas.createContext(arg0);
    }

    public GLCapabilities getChosenGLCapabilities() {
	call("getChosenGLCapabilities()");
	return canvas.getChosenGLCapabilities();
    }

    public void setRealized(boolean arg0) {
	call("setRealized(boolean arg0)");
	canvas.setRealized(arg0);
    }

    public void swapBuffers() throws GLException {
	call("swapBuffers()");
	canvas.swapBuffers();
    }

    public void display() {
	call("display()");
	canvas.display();
    }

    public void update(Graphics g) {
	super.update(g);
	call("update(Graphics g)");
	canvas.update(g);
    }

    /**
     * Size management
     * Frame must be bigger than canvas.
     */

    public void setSize(int width, int height) {
	super.setSize(width, height);
	call("setSize(int width, int height)");
	
	canvas.setSize(width, height);
	//frame.setSize(width, height);
    }

    public void setSize(Dimension dim) {
	super.setSize(dim);
	call("setSize(Dimension dim)");

	canvas.setSize(dim);
	//frame.setSize(dim);
    }

    private static class PixelStorageModes {
	int packAlignment;
	int packRowLength;
	int packSkipRows;
	int packSkipPixels;
	int packSwapBytes;
	int[] tmp = new int[1];

	void save(GL gl) {
	    gl.glGetIntegerv(GL.GL_PACK_ALIGNMENT, tmp, 0);
	    packAlignment  = tmp[0];
	    gl.glGetIntegerv(GL.GL_PACK_ROW_LENGTH, tmp, 0);
	    packRowLength  = tmp[0];
	    gl.glGetIntegerv(GL.GL_PACK_SKIP_ROWS, tmp, 0);
	    packSkipRows   = tmp[0];
	    gl.glGetIntegerv(GL.GL_PACK_SKIP_PIXELS, tmp, 0);
	    packSkipPixels = tmp[0];
	    gl.glGetIntegerv(GL.GL_PACK_SWAP_BYTES, tmp, 0);
	    packSwapBytes  = tmp[0];

	    gl.glPixelStorei(GL.GL_PACK_ALIGNMENT, 1);
	    gl.glPixelStorei(GL.GL_PACK_ROW_LENGTH, 0);
	    gl.glPixelStorei(GL.GL_PACK_SKIP_ROWS, 0);
	    gl.glPixelStorei(GL.GL_PACK_SKIP_PIXELS, 0);
	    gl.glPixelStorei(GL.GL_PACK_SWAP_BYTES, 0);
	}

	void restore(GL gl) {
	    gl.glPixelStorei(GL.GL_PACK_ALIGNMENT, packAlignment);
	    gl.glPixelStorei(GL.GL_PACK_ROW_LENGTH, packRowLength);
	    gl.glPixelStorei(GL.GL_PACK_SKIP_ROWS, packSkipRows);
	    gl.glPixelStorei(GL.GL_PACK_SKIP_PIXELS, packSkipPixels);
	    gl.glPixelStorei(GL.GL_PACK_SWAP_BYTES, packSwapBytes);
	}
    }

}
