/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.examples.utils;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.tranformations.Rotation;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

/**
 * This mouse adapter generate a {@link org.scilab.forge.scirenderer.tranformations.Rotation}, depending on mouse drags.
 *
 * @author Pierre Lando
 */
public class MouseRotationAdapter implements MouseListener, MouseMotionListener {
    private static final float SPEED_RATIO = 128f;

    private Rotation currentRotation = new Rotation(Math.toRadians(0), new Vector3d(1, 0, 0));
    private Rotation startRotation;
    private Point startPoint;

    /**
     * The canvas will be notified for update.
     */
    private Canvas canvas;

    /**
     * Default constructor.
     * @param startRotation the starting rotation.
     */
    public MouseRotationAdapter(Rotation startRotation) {
        currentRotation = startRotation;
        this.canvas = null;
    }

    /**
     * A constructor.
     * The given canvas will be notified for when the current rotation change.
     * @param rotation the starting rotation.
     * @param canvas the canvas to be notified on rotation changes.
     */
    public MouseRotationAdapter(Rotation rotation, Canvas canvas) {
        currentRotation = rotation;
        this.canvas = canvas;
    }

    @Override
    public void mousePressed(MouseEvent e) {
        startRotation = currentRotation;
        startPoint = e.getPoint();
        e.getComponent().addMouseMotionListener(this);
    }

    @Override
    public void mouseReleased(MouseEvent e) {
        e.getComponent().removeMouseMotionListener(this);
    }

    @Override
    public void mouseDragged(MouseEvent e) {
        int deltaX = e.getX() - startPoint.x;
        int deltaY = e.getY() - startPoint.y;

        Rotation delta = new Rotation(deltaY / SPEED_RATIO, new Vector3d(1, 0, 0));
        delta.multiply(new Rotation(deltaX / SPEED_RATIO, new Vector3d(0, 1, 0)));

        currentRotation = startRotation.times(delta);

        if (canvas != null) {
            canvas.redraw();
        }
    }

    /**
     * Return the current rotation.
     * @return the current rotation.
     */
    public Rotation getRotation() {
        return currentRotation;
    }

    @Override
    public void mouseClicked(MouseEvent e) {
    }

    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

    @Override
    public void mouseMoved(MouseEvent e) {
    }
}
