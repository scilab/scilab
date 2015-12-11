/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.modules.commons.utils.BlockingResult;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * @author Pierre Lando
 */
public class InteractionManager implements RubberBoxListener {

    /** The rubber box */
    private RubberBox rubberBox;

    /** The drag, zoom, rotate interaction manager */
    private final DragZoomRotateInteraction dragZoomRotateInteraction;

    /** Parent drawer visitor */
    private DrawerVisitor drawerVisitor;

    /**
     * Default constructor.
     * @param drawerVisitor parent drawer visitor.
     */
    public InteractionManager(DrawerVisitor drawerVisitor) {
        this.drawerVisitor = drawerVisitor;
        dragZoomRotateInteraction = new DragZoomRotateInteraction(drawerVisitor);
        dragZoomRotateInteraction.setEnable(true);
    }

    /**
     * Finalize method.
     * @exception Throwable the <code>Exception</code> raised by this method
     */
    public void finalize() throws Throwable {
        if (rubberBox != null) {
            rubberBox.removeListener(this);
        }
        super.finalize();
    }

    /**
     * Called to start zooming.
     */
    public void startInteractiveZoom() {
        final ZoomRubberBox rubberBox = new ZoomRubberBox(drawerVisitor);
        dragZoomRotateInteraction.setEnable(false);

        rubberBox.addListener(new RubberBoxListener() {
            public void rubberBoxEnd() {
                dragZoomRotateInteraction.setEnable(true);
                drawerVisitor.removePostRendering(rubberBox);
            }
        });
        drawerVisitor.addPostRendering(rubberBox);
        rubberBox.setEnable(true);
    }

    public double[] startClickRubberBox(double initialRect[]) {
        final BlockingResult<double []> result = new BlockingResult<double[]>();
        final PointRubberBox rubberBox;
        if (initialRect.length == 0) {
            rubberBox = new TwoPointsRubberBox(drawerVisitor);
        } else {
            rubberBox = new OnePointRubberBox(drawerVisitor, initialRect);
        }

        dragZoomRotateInteraction.setEnable(false);
        rubberBox.addListener(new RubberBoxListener() {
            @Override
            public void rubberBoxEnd() {
                result.setResult(rubberBox.getResults());
                dragZoomRotateInteraction.setEnable(true);
                drawerVisitor.removePostRendering(rubberBox);
            }
        });
        drawerVisitor.addPostRendering(rubberBox);
        rubberBox.setEnable(true);
        return result.getResult();
    }

    public double[] startDragRubberBox() {
        final BlockingResult<double []> result = new BlockingResult<double []>();
        final DragPointRubberBox rubberBox = new DragPointRubberBox(drawerVisitor);

        dragZoomRotateInteraction.setEnable(false);
        rubberBox.addListener(new RubberBoxListener() {
            @Override
            public void rubberBoxEnd() {
                result.setResult(rubberBox.getResults());
                dragZoomRotateInteraction.setEnable(true);
                drawerVisitor.removePostRendering(rubberBox);
            }
        });
        drawerVisitor.addPostRendering(rubberBox);
        rubberBox.setEnable(true);
        return result.getResult();
    }

    @Override
    public void rubberBoxEnd() {
        dragZoomRotateInteraction.setEnable(true);
        rubberBox = null;
    }

    public boolean isInteractiveZoom() {
        return !dragZoomRotateInteraction.isEnable();
    }

    public void setTranslationEnable(boolean b) {
        dragZoomRotateInteraction.setTranslationEnable(b);
    }
}
