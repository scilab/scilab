package org.scilab.modules.gui.bridge.canvas;

import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Event;
import java.awt.Font;
import java.awt.Image;
import java.awt.MenuComponent;
import java.awt.MenuContainer;
import java.awt.event.ComponentListener;
import java.awt.event.FocusListener;
import java.awt.event.HierarchyBoundsListener;
import java.awt.event.HierarchyListener;
import java.awt.event.InputMethodListener;
import java.awt.event.KeyListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;
import java.awt.image.ImageObserver;
import java.beans.PropertyChangeListener;
import java.io.Serializable;

import javax.accessibility.Accessible;
import javax.accessibility.AccessibleContext;
import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCanvas;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLException;
import javax.media.opengl.GLJPanel;


public class SwingScilabCanvasImpl implements GLAutoDrawable, ImageObserver, MenuContainer, Accessible, Serializable {

    final static boolean enableGLCanvas = false;
    GLCanvas realGLCanvas;
    GLJPanel realGLJPanel;

    public SwingScilabCanvasImpl() {
	if (enableGLCanvas) {
	    System.err.println("[SwingScilabCanvasImpl] is GLCanvas..");
	    realGLCanvas = new GLCanvas();
	}
	else {
	    System.err.println("[SwingScilabCanvasImpl] is GLJPanel..");
	    realGLJPanel= new GLJPanel();
	}
    }

    public SwingScilabCanvasImpl(GLCapabilities cap) {
	if (enableGLCanvas) {
	    System.err.println("[SwingScilabCanvasImpl] is GLCanvas..");
	    realGLCanvas = new GLCanvas(cap);
	}
	else {
	    System.err.println("[SwingScilabCanvasImpl] is GLJPanel..");
	    realGLJPanel = new GLJPanel(cap);
	}
    }

    private GLAutoDrawable getAsGL() {
	if (enableGLCanvas) {
	    return realGLCanvas;
	}
	else {
	    return realGLJPanel;
	}
    }

    public Component getAsComponent() {
	if (enableGLCanvas) {
	    return realGLCanvas;
	}
	else {
	    return realGLJPanel;
	}
    }

    public void addGLEventListener(GLEventListener arg0) {
	getAsGL().addGLEventListener(arg0);
    }

    public void display() {
	getAsGL().display();
    }

    public boolean getAutoSwapBufferMode() {
	return getAsGL().getAutoSwapBufferMode();
    }

    public GLContext getContext() {
	return getAsGL().getContext();
    }

    public GL getGL() {
	return getAsGL().getGL();
    }

    public void removeGLEventListener(GLEventListener arg0) {
	getAsGL().removeGLEventListener(arg0);
    }

    public void repaint() {
	getAsGL().repaint();
    }

    public void setAutoSwapBufferMode(boolean arg0) {
	getAsGL().setAutoSwapBufferMode(arg0);
    }

    public void setGL(GL arg0) {
	getAsGL().setGL(arg0);
    }

    public GLContext createContext(GLContext arg0) {
	return getAsGL().createContext(arg0);
    }

    public GLCapabilities getChosenGLCapabilities() {
	return getAsGL().getChosenGLCapabilities();
    }

    public int getHeight() {
	return getAsGL().getHeight();
    }

    public int getWidth() {
	return getAsGL().getWidth();
    }

    public void setRealized(boolean arg0) {
	getAsGL().setRealized(arg0);
    }

    public void setSize(int arg0, int arg1) {
	getAsGL().setSize(arg0, arg1);
    }

    public void swapBuffers() throws GLException {
	getAsGL().swapBuffers();
    }

    public void addComponentListener(ComponentListener arg0) {
	getAsGL().addComponentListener(arg0);
    }

    public void addFocusListener(FocusListener arg0) {
	getAsGL().addFocusListener(arg0);
    }

    public void addHierarchyBoundsListener(HierarchyBoundsListener arg0) {
	getAsGL().addHierarchyBoundsListener(arg0);
    }

    public void addHierarchyListener(HierarchyListener arg0) {
	getAsGL().addHierarchyListener(arg0);
    }

    public void addInputMethodListener(InputMethodListener arg0) {
	getAsGL().addInputMethodListener(arg0);
    }

    public void addKeyListener(KeyListener arg0) {
	getAsGL().addKeyListener(arg0);
    }

    public void addMouseListener(MouseListener arg0) {
	getAsGL().addMouseListener(arg0);
    }

    public void addMouseMotionListener(MouseMotionListener arg0) {
	getAsGL().addMouseMotionListener(arg0);
    }

    public void addMouseWheelListener(MouseWheelListener arg0) {
	getAsGL().addMouseWheelListener(arg0);
    }

    public void addPropertyChangeListener(PropertyChangeListener arg0) {
	getAsGL().addPropertyChangeListener(arg0);
    }

    public void addPropertyChangeListener(String arg0,
	    PropertyChangeListener arg1) {
	getAsGL().addPropertyChangeListener(arg0, arg1);
    }

    public void removeComponentListener(ComponentListener arg0) {
	getAsGL().removeComponentListener(arg0);
    }

    public void removeFocusListener(FocusListener arg0) {
	getAsGL().removeFocusListener(arg0);
    }

    public void removeHierarchyBoundsListener(HierarchyBoundsListener arg0) {
	getAsGL().removeHierarchyBoundsListener(arg0);
    }

    public void removeHierarchyListener(HierarchyListener arg0) {
	getAsGL().removeHierarchyListener(arg0);
    }

    public void removeInputMethodListener(InputMethodListener arg0) {
	getAsGL().removeInputMethodListener(arg0);
    }

    public void removeKeyListener(KeyListener arg0) {
	getAsGL().removeKeyListener(arg0);
    }

    public void removeMouseListener(MouseListener arg0) {
	getAsGL().removeMouseListener(arg0);
    }

    public void removeMouseMotionListener(MouseMotionListener arg0) {
	getAsGL().removeMouseMotionListener(arg0);
    }

    public void removeMouseWheelListener(MouseWheelListener arg0) {
	getAsGL().removeMouseWheelListener(arg0);
    }

    public void removePropertyChangeListener(PropertyChangeListener arg0) {
	getAsGL().removePropertyChangeListener(arg0);
    }

    public void removePropertyChangeListener(String arg0,
	    PropertyChangeListener arg1) {
	getAsGL().removePropertyChangeListener(arg0, arg1);
    }

    public boolean imageUpdate(Image arg0, int arg1, int arg2, int arg3,
	    int arg4, int arg5) {
	return getAsComponent().imageUpdate(arg0, arg1, arg2, arg3, arg4, arg5);
    }

    public Font getFont() {
	return getAsComponent().getFont();
    }

    public boolean postEvent(Event arg0) {
	return getAsComponent().postEvent(arg0);
    }

    public void remove(MenuComponent arg0) {
	getAsComponent().remove(arg0);
    }

    public AccessibleContext getAccessibleContext() {
	return getAsComponent().getAccessibleContext();
    }

    public void setVisible(boolean arg0) {
	getAsComponent().setVisible(arg0);
    }

    public void doLayout() {
	getAsComponent().doLayout();
    }

    public int getX() {
	return getAsComponent().getX();
    }

    public int getY() {
	return getAsComponent().getY();
    }

    public void setFocusable(boolean arg0) {
	getAsComponent().setFocusable(arg0);
    }

    public void enableEvents(long arg0) {
	//realGLCanvas.enableEvents(arg0);
	//realGLJPanel.enableEvents(arg0);
    }

    public void setSize(Dimension arg0) {
	getAsComponent().setSize(arg0);
    }

    public void setLocation(int arg0, int arg1) {
	getAsComponent().setLocation(arg0, arg1);
    }

    public void setBackground(Color arg0) {
	getAsComponent().setBackground(arg0);
    }

    public void setPreferredSize(Dimension arg0) {
	getAsComponent().setPreferredSize(arg0);
    }

    public Dimension getPreferredSize() {
	return getAsComponent().getPreferredSize();
    }

    public void setCursor(Cursor arg0) {
	getAsComponent().setCursor(arg0);
    }

}
