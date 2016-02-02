/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.forge.scirenderer.implementation.jogl;

import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.Semaphore;

import javax.media.opengl.DebugGL2;
import javax.media.opengl.GL2;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLDrawableFactory;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLException;
import javax.media.opengl.GLOffscreenAutoDrawable;
import javax.media.opengl.GLProfile;
import javax.swing.SwingUtilities;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.implementation.jogl.buffers.JoGLBuffersManager;
import org.scilab.forge.scirenderer.implementation.jogl.picking.JoGLPickingManager;
import org.scilab.forge.scirenderer.implementation.jogl.renderer.JoGLRendererManager;
import org.scilab.forge.scirenderer.implementation.jogl.texture.JoGLTextureManager;
import org.scilab.forge.scirenderer.picking.PickingManager;

import com.jogamp.opengl.util.awt.AWTGLReadBufferUtil;

/**
 * JoGL implementation of a Canvas.
 *
 * @author Pierre Lando
 */
public final class JoGLCanvas implements Canvas, GLEventListener {

    private static final double[][][] ANTI_ALIASING_JITTER = {
        {{0.0, 0.0}},
        {{0.5, 0.5}, {0.5, -0.5}},
        {
            { -0.25, -0.5}, {0.25, 0.5}, {0.75, -0.5}, {0.25, 0.5}
        },
        {
            {0.125, -0.125}, { -0.875, 0.875}, { -0.375, 0.375}, {0.375, 0.625},
            {0.625, -0.625}, {0.875, 0.125}, { -0.125, -0.875}, { -0.625, -0.375}
        },
        {
            { -0.25, -0.125}, {0.25, -0.875}, {0.75, -0.625}, { -0.75, -0.875},
            { -0.25, 0.375}, {0.75, -0.125}, {0.25, 0.125}, { -0.25, 0.875},
            {0.25, -0.375}, { -0.75, 0.125}, { -0.75, 0.625}, { -0.25, -0.625},
            {0.75, 0.875}, {0.75, 0.375}, { -0.75, -0.375}, {0.25, 0.625}
        }
    };

    private final GLAutoDrawable autoDrawable;

    private final JoGLDrawingTools drawingTools;
    private final JoGLParameters parameters;
    private final JoGLBuffersManager buffersManager;
    private final JoGLRendererManager rendererManager;
    private final JoGLPickingManager pickingManager;
    private final JoGLTextureManager textureManager;

    private final CanvasAnimator canvasAnimator;
    private boolean isOffscreen;
    private DebugGL2 debug;
    private boolean isValid = true;
    private boolean displayFinished;


    /** The current mainDrawer. */
    private Drawer mainDrawer;

    /** The anti-aliasing level */
    private int antiAliasingLevel = 0;

    /**
     * Default constructor.
     * @param autoDrawable the JoGL autoDrawable this canvas depend on.
     */
    JoGLCanvas(GLAutoDrawable autoDrawable) {
        this.autoDrawable = autoDrawable;
        parameters = new JoGLParameters();
        buffersManager = new JoGLBuffersManager();
        rendererManager = new JoGLRendererManager();
        drawingTools = new JoGLDrawingTools(this);
        pickingManager = new JoGLPickingManager(this);
        textureManager = new JoGLTextureManager(this);

        autoDrawable.addGLEventListener(this);
        canvasAnimator = new CanvasAnimator(autoDrawable);
        canvasAnimator.redraw();
    }

    /**
     * Constructor for offscreen rendering
     * @param width the width
     * @param height the height
     */
    JoGLCanvas(int width, int height) {
        this(getOffscreenDrawable(width, height));
        isOffscreen = true;
    }

    public void setDebugMode(boolean debug) {
        if (debug) {
            this.debug = new DebugGL2(autoDrawable.getGL().getGL2());
        } else {
            this.debug = null;
        }
    }

    // Implementation of getter & setter from Canvas.

    @Override
    public void setMainDrawer(Drawer mainDrawer) {
        this.mainDrawer = mainDrawer;
    }

    @Override
    public Drawer getMainDrawer() {
        return mainDrawer;
    }

    @Override
    public JoGLRendererManager getRendererManager() {
        return rendererManager;
    }

    @Override
    public JoGLBuffersManager getBuffersManager() {
        return buffersManager;
    }

    @Override
    public PickingManager getPickingManager() {
        return pickingManager;
    }

    @Override
    public JoGLTextureManager getTextureManager() {
        return textureManager;
    }

    @Override
    public int getWidth() {
        return autoDrawable.getSurfaceWidth();
    }

    @Override
    public int getHeight() {
        return autoDrawable.getSurfaceHeight();
    }

    @Override
    public Dimension getDimension() {
        return new Dimension(autoDrawable.getSurfaceWidth(), autoDrawable.getSurfaceHeight());
    }

    @Override
    public void redraw() {
        canvasAnimator.redraw();
    }

    @Override
    public void redrawAndWait() {
        if (SwingUtilities.isEventDispatchThread()) {
            if (autoDrawable != null) {
                autoDrawable.display();
            }
            return;
        }
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                public void run() {
                    autoDrawable.display();
                }
            });
        } catch (Exception e) { }
    }

    @Override
    public void waitImage() {
        canvasAnimator.waitEndOfDrawing();
    }

    @Override
    public int getAntiAliasingLevel() {
        return antiAliasingLevel;
    }

    @Override
    public void setAntiAliasingLevel(int antiAliasingLevel) {
        this.antiAliasingLevel = antiAliasingLevel;
    }

    // JoGLCanvas specific getter.

    /**
     * Return the OpenGl context.
     * @return the OpenGl context.
     */
    public GL2 getGl() {
        if (debug == null) {
            return autoDrawable.getGL().getGL2();
        } else {
            return debug;
        }
    }

    /**
     * Return the rendering parameters.
     * @return the rendering parameters.
     */
    public JoGLParameters getJoGLParameters() {
        return parameters;
    }

    /**
     * Get an image from the autoDrawable
     * @return an image
     */
    public BufferedImage getImage() {
        return getImage(true);
    }

    /**
     * Get an image from the autoDrawable
     * @return an image
     */
    public BufferedImage getImage(final boolean alpha) {
        while (!canvasAnimator.isDrawFinished() || !displayFinished) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                break;
            }
        }

        final BufferedImage[] image = new BufferedImage[1];
        final GLContext context = autoDrawable.getContext();

        if (SwingUtilities.isEventDispatchThread()) {
            context.makeCurrent();
            AWTGLReadBufferUtil buffer = new AWTGLReadBufferUtil(GLProfile.getDefault(), alpha);
            image[0] = buffer.readPixelsToBufferedImage(getGl(), 0, 0, autoDrawable.getSurfaceWidth(), autoDrawable.getSurfaceHeight(), true);
            context.release();
        } else {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        context.makeCurrent();
                        AWTGLReadBufferUtil buffer = new AWTGLReadBufferUtil(GLProfile.getDefault(), alpha);
                        image[0] = buffer.readPixelsToBufferedImage(getGl(), 0, 0, autoDrawable.getSurfaceWidth(), autoDrawable.getSurfaceHeight(), true);
                        context.release();
                    }
                });
            } catch (InterruptedException e) {

            } catch (InvocationTargetException e) {
                System.err.println(e);
                e.printStackTrace();
            }
        }

        return image[0];
    }

    /**
     * Destroy the GLPbuffer
     */
    public void destroy() {
        if (isOffscreen) {
            ((GLOffscreenAutoDrawable) autoDrawable).destroy();
        }
        try {
            isValid = false;
            canvasAnimator.finalize();//Thread.dumpStack();
        } catch (Throwable e) {
            // TODO: handle exception
        }
    }

    /**
     * Creates a GLPbuffer for an offscreen rendering
     * @param width the width
     * @param height the height
     * @return a GLPbuffer
     */
    private static GLAutoDrawable getOffscreenDrawable(int width, int height) {
        GLDrawableFactory factory = GLDrawableFactory.getDesktopFactory();

        GLCapabilities capabilities = new GLCapabilities(GLProfile.getDefault());
        capabilities.setPBuffer(true);

        return factory.createOffscreenAutoDrawable(null, capabilities, null, width, height);
    }

    // Implementation of function from GLEventListener.
    @Override
    public void display(GLAutoDrawable glAutoDrawable) {
        if (isValid) {
            displayFinished = false;
            GL2 gl = getGl().getGL2();
            buffersManager.glSynchronize(gl);
            rendererManager.glSynchronize(gl);
            drawingTools.glSynchronize(gl);

            if (mainDrawer != null) {
                gl.glEnable(GL2.GL_DEPTH_TEST);
                gl.glDepthFunc(GL2.GL_LEQUAL); // Set to less equal to allow last drawn object to be on the top.

                if ((antiAliasingLevel > 0) && (antiAliasingLevel < 5) && (drawingTools.getGLCapacity().isAccumulationBufferPresent())) {
                    org.scilab.forge.scirenderer.renderer.Renderer renderer = rendererManager.createRenderer();
                    renderer.setDrawer(mainDrawer);

                    double[][] jitter = ANTI_ALIASING_JITTER[antiAliasingLevel];
                    Dimension dimension = getDimension();
                    gl.glClear(GL2.GL_ACCUM_BUFFER_BIT);
                    for (double[] aJitter : jitter) {
                        drawingTools.glSynchronize(gl);

                        gl.glMatrixMode(GL2.GL_PROJECTION);
                        gl.glLoadIdentity();
                        gl.glTranslated(aJitter[0] / dimension.getWidth(), aJitter[1] / dimension.getHeight(), 0.);

                        gl.glClear(GL2.GL_DEPTH_BUFFER_BIT);
                        rendererManager.draw(drawingTools, renderer);
                        //mainDrawer.draw(drawingTools);
                        gl.glReadBuffer(GL2.GL_BACK);
                        gl.glAccum(GL2.GL_ACCUM, 1f / jitter.length);
                    }

                    rendererManager.dispose(drawingTools, renderer);

                    gl.glDrawBuffer(GL2.GL_BACK);
                    gl.glAccum(GL2.GL_RETURN, 1.0f);
                } else {
                    gl.glMatrixMode(GL2.GL_PROJECTION);
                    gl.glLoadIdentity();
                    gl.glClear(GL2.GL_DEPTH_BUFFER_BIT);
                    mainDrawer.draw(drawingTools);
                }
            }

            pickingManager.glConsume(drawingTools);
            displayFinished = true;
        }
    }

    @Override
    public void init(GLAutoDrawable glAutoDrawable) {
        textureManager.glReload();
        buffersManager.glReload();
        rendererManager.glReload();
    }

    @Override
    public void reshape(GLAutoDrawable glAutoDrawable, int x, int y, int width, int height) {
    }

    @Override
    public void dispose(GLAutoDrawable drawable) { }

    /**
     * this class manage asynchronous scene drawing.
     */
    private class CanvasAnimator implements Runnable {

        private final Semaphore semaphore = new Semaphore(1);
        private final Thread thread;
        private final GLAutoDrawable autoDrawable;
        private boolean running = true;

        public CanvasAnimator(GLAutoDrawable autoDrawable) {
            this.autoDrawable = autoDrawable;
            this.thread = new Thread(this);
            thread.start();
            //System.err.println("[DEBUG] nb threads = "+Thread.activeCount());
        }

        @Override
        public void finalize() throws Throwable {
            running = false;
            // we increment the semaphore to allow run() to unlock it and to be sure
            // to go out.
            semaphore.release();
            autoDrawable.destroy();
            super.finalize();
        }

        public synchronized boolean isDrawFinished() {
            return semaphore.availablePermits() == 0;
        }

        /** Ask the animator to perform a draw later. */
        public synchronized void redraw() {
            semaphore.release();
        }

        /** Wait until a drawing has been performed */
        public void waitEndOfDrawing() {
            semaphore.drainPermits();
        }

        @Override
        public void run() {
            while (running) {
                try {
                    semaphore.acquire();
                    semaphore.drainPermits();
                    if (running) {
                        autoDrawable.display();
                    }
                } catch (InterruptedException e) {
                    if (running) {
                        Thread.currentThread().interrupt();
                    }
                    break;
                } catch (GLException e) {
                    if (running) {
                        throw e;
                    }
                    break;
                }
            }
        }
    }
}
