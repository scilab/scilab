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

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCanvas;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLCapabilitiesChooser;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLException;
import javax.swing.JPanel;

import com.sun.opengl.util.Screenshot;


public class GLHybrid extends JPanel implements GLAutoDrawable {


	private Frame frame;
	private Panel panel;

	private GLCanvas canvas;

	private static int WIDTH_OFFSET = 20;
	private static int HEIGHT_OFFSET = 40;

	public GLHybrid() {
		this(null);
	}

	public GLHybrid(GLCapabilities capabilities) {
		this(capabilities, null, null, null);
	}

	public GLHybrid(GLCapabilities capabilities,
			GLCapabilitiesChooser chooser,
			GLContext shareWith,
			GraphicsDevice device) {
		super();

		canvas = new GLCanvas(capabilities, chooser, shareWith, device);


		frame = new Frame();
		panel = new Panel();
		panel.add(canvas);
		frame.add(panel);
		frame.setVisible(false);

		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		System.err.println("Width = "+screenSize.width);
		System.err.println("Height = "+screenSize.height);
		canvas.setSize(screenSize.width, screenSize.height);
		canvas.setLocation(0, 0);
		panel.setSize(screenSize.width, screenSize.height);
		panel.setLocation(0, 0);
		frame.setSize(screenSize.width + WIDTH_OFFSET, screenSize.height + HEIGHT_OFFSET);
		frame.setLocation(0, -HEIGHT_OFFSET / 2);
		frame.pack();
	}

	public void paintComponent(final Graphics g) {
		super.paintComponent(g);
		canvas.display();
	}

	public void paint(Graphics g) {
		super.paint(g);

		canvas.display();
		canvas.getContext().makeCurrent();
		g.drawImage(Screenshot.readToBufferedImage(getWidth(), getHeight()), 0, 0, getWidth(), getHeight(), this);
		canvas.getContext().release();
	}

	public void repaint() {
		super.repaint();
		if (canvas != null) {
			canvas.repaint();
		}
	}

	public void addNotify() {
		super.addNotify();
		canvas.addNotify();
	}

	public void addGLEventListener(GLEventListener listener) {
		canvas.addGLEventListener(listener);
	}

	public boolean getAutoSwapBufferMode() {
		return canvas.getAutoSwapBufferMode();
	}

	public GLContext getContext() {
		return canvas.getContext();
	}

	public GL getGL() {
		return canvas.getGL();
	}

	public void removeGLEventListener(GLEventListener listener) {
		canvas.removeGLEventListener(listener);
	}

	public void setAutoSwapBufferMode(boolean arg0) {
		canvas.setAutoSwapBufferMode(arg0);
	}

	public void setGL(GL gl) {
		canvas.setGL(gl);
	}

	public GLContext createContext(GLContext arg0) {
		return canvas.createContext(arg0);
	}

	public GLCapabilities getChosenGLCapabilities() {
		return canvas.getChosenGLCapabilities();
	}

	public void setRealized(boolean arg0) {
		canvas.setRealized(arg0);
	}

	public void swapBuffers() throws GLException {
		canvas.swapBuffers();
	}

	public void display() {
		canvas.display();
	}

	public void update(Graphics g) {
		super.update(g);
		canvas.update(g);
	}

	/**
	 * Size management
	 * Frame must be bigger than canvas.
	 */

	public void setSize(int width, int height) {
		super.setSize(width, height);
		canvas.setSize(width, height);
	}

	public void setSize(Dimension dim) {
		super.setSize(dim);
		canvas.setSize(dim);
	}

}
