/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.forge.scirenderer.implementation.g2d;

import java.awt.Dimension;
import java.awt.Graphics2D;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.implementation.g2d.buffers.G2DBuffersManager;
import org.scilab.forge.scirenderer.implementation.g2d.motor.Motor3D;
import org.scilab.forge.scirenderer.implementation.g2d.renderer.G2DRendererManager;
import org.scilab.forge.scirenderer.implementation.g2d.texture.G2DTextureManager;
import org.scilab.forge.scirenderer.picking.PickingManager;
import org.scilab.forge.scirenderer.picking.PickingTask;

/**
 * G2D implementation of a Canvas.
 *
 * @author Calixte DENIZET
 */
public final class G2DCanvas implements Canvas {

    private final G2DDrawingTools drawingTools;
    private final G2DBuffersManager buffersManager;
    private final G2DRendererManager rendererManager;
    private final G2DTextureManager textureManager;

    private final Motor3D motor;
    private final Dimension dimension;

    private boolean drawEnabled = true;

    /** The anti-aliasing level */
    private int antiAliasingLevel = 0;

    private static final PickingManager PICKINGMANAGER = new PickingManager() {

        @Override
        public void addPickingTask(PickingTask pickingTask) { }
    };

    /**
     * The current mainDrawer.
     */
    private Drawer mainDrawer;

    /**
     * Default constructor.
     * @param autoDrawable the G2D autoDrawable this canvas depend on.
     */
    G2DCanvas(Graphics2D g2d, int width, int height) {
        this.dimension = new Dimension(width, height);
        this.motor = new Motor3D(this, g2d, dimension);

        buffersManager = new G2DBuffersManager();
        rendererManager = new G2DRendererManager();
        drawingTools = new G2DDrawingTools(this);
        motor.setClippingPlanes(drawingTools.getClippingManager().getClippingPlanes());
        textureManager = new G2DTextureManager(this);
    }

    public DrawingTools getDrawingTools() {
        return drawingTools;
    }

    public void setGraphics(Graphics2D g2d, int width, int height) {
        this.dimension.width = width;
        this.dimension.height = height;
        this.motor.setGraphics(g2d);
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
    public G2DRendererManager getRendererManager() {
        return rendererManager;
    }

    @Override
    public G2DBuffersManager getBuffersManager() {
        return buffersManager;
    }

    @Override
    public PickingManager getPickingManager() {
        return PICKINGMANAGER;
    }

    @Override
    public G2DTextureManager getTextureManager() {
        return textureManager;
    }

    @Override
    public int getWidth() {
        return dimension.width;
    }

    @Override
    public int getHeight() {
        return dimension.height;
    }

    @Override
    public Dimension getDimension() {
        return dimension;
    }

    @Override
    public int getAntiAliasingLevel() {
        return antiAliasingLevel;
    }

    @Override
    public void setAntiAliasingLevel(int antiAliasingLevel) {
        this.antiAliasingLevel = antiAliasingLevel;
    }

    @Override
    public void redraw() {
        draw();
    }

    @Override
    public void redrawAndWait() {
        redraw();
    }

    @Override
    public void waitImage() {
    }

    // G2DCanvas specific getter.

    public void disableDraw() {
        drawEnabled = false;
    }

    public void enableDraw() {
        drawEnabled = true;
    }

    public void draw() {
        if (drawEnabled) {
            try {
                mainDrawer.draw(drawingTools);
                getMotor3D().setAntialiased(antiAliasingLevel != 0);
                getMotor3D().draw();
            } catch (Exception e) {
                System.out.println(e + "::::" + mainDrawer);
                e.printStackTrace();
            }
        }
    }

    /**
     * Return the OpenGl context.
     * @return the OpenGl context.
     */
    public Motor3D getMotor3D() {
        return motor;
    }

    @Override
    public void destroy() {
        getMotor3D().clean();
    }
}
