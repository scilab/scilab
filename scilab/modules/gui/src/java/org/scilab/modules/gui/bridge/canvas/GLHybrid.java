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
import java.awt.GraphicsDevice;
import java.awt.Panel;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCanvas;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLCapabilitiesChooser;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLException;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import com.sun.opengl.util.Screenshot;


public class GLHybrid extends JPanel implements GLAutoDrawable {


    private final static int WIDTH_OFFSET = 20;
    private final static int HEIGHT_OFFSET = 40;

    private final static boolean VERBOSE = true;
    private final static boolean TIMED = false;
    
    private boolean needReDump = true;

    private JFrame frame;
    private JPanel panel;

    private GLCanvas canvas;
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

	//frame = new JFrame();
	//panel = new JPanel();
	//panel.add(canvas);
	//frame.add(panel);
	//frame.setVisible(false);

	Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
	canvas.setSize(screenSize.width, screenSize.height);
	canvas.setLocation(0, 0);
	canvas.setVisible(false);
	//panel.setSize(screenSize.width, screenSize.height);
	//panel.setLocation(0, 0);
	//frame.setSize(screenSize.width + WIDTH_OFFSET, screenSize.height + HEIGHT_OFFSET);
	//frame.setLocation(0, - HEIGHT_OFFSET / 2);
	//frame.pack();

	//Animator animator = new Animator(canvas);
	//animator.setRunAsFastAsPossible(true);
	//animator.start();
    }

    public void paintComponent(final Graphics g) {
	//super.paintComponents(g);
	call("paintComponent(final Graphics g)");
	//if (needReDump) {
	//    dumpCanvas();
	//}
	//drawDumpedImage(g);
    }

    public void paint(Graphics g) {
	//super.paint(g);
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
	tic = System.currentTimeMillis();
	dump = Screenshot.readToBufferedImage(getWidth(), getHeight());
	tac = System.currentTimeMillis();
	time("Screenshot.readToBufferedImage...", tic, tac);
	
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
    }

    public void setSize(Dimension dim) {
	super.setSize(dim);
	call("setSize(Dimension dim)");
	canvas.setSize(dim);
    }

}
