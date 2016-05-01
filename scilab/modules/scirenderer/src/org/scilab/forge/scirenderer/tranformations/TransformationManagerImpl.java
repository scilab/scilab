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

package org.scilab.forge.scirenderer.tranformations;

import org.scilab.forge.scirenderer.Canvas;

import javax.swing.event.EventListenerList;

/**
 * Default {@link org.scilab.forge.scirenderer.tranformations.TransformationManager} implementation.
 * @author Pierre Lando
 */
public class TransformationManagerImpl implements TransformationManager {

    /**
     * We use swing EventListenerList for facility.
     */
    private final EventListenerList listeners;

    private final Canvas canvas;
    private final TransformationStack modelViewStack;
    private final TransformationStack projectionStack;

    /**
     * The peek transformation value.
     * Set to null if it must be recalculate.
     */
    private Transformation topTransformation;
    private boolean usingSceneCoordinate = true;


    /**
     * Standard constructor.
     * @param canvas the canvas.
     */
    public TransformationManagerImpl(Canvas canvas) {
        this.canvas = canvas;
        listeners = new EventListenerList();
        modelViewStack = new TransformationStackImpl();
        projectionStack = new TransformationStackImpl();
        topTransformation = null;


        final TransformationStackListener listener = new TransformationStackListener() {
            @Override
            public void changed(TransformationStack transformationStack, TransformationStack.TransformationStackEvent event, Transformation top) {
                topTransformation = null;
                fireTransformationChanged();
            }
        };

        modelViewStack.addListener(listener);
        projectionStack.addListener(listener);
    }


    @Override
    public void addListener(TransformationManagerListener listener) {
        listeners.add(TransformationManagerListener.class, listener);
    }

    @Override
    public void removeListener(TransformationManagerListener listener) {
        listeners.remove(TransformationManagerListener.class, listener);
    }

    @Override
    public TransformationStack getModelViewStack() {
        return modelViewStack;
    }

    @Override
    public TransformationStack getProjectionStack() {
        return projectionStack;
    }

    @Override
    public Transformation getTransformation() {
        if (topTransformation == null) {
            final Transformation modelView = modelViewStack.peek();
            final Transformation projection = projectionStack.peek();
            final Transformation returnedTransformation = projection.rightTimes(modelView);
            topTransformation = returnedTransformation;
            return returnedTransformation;
        } else {
            return topTransformation;
        }
    }

    @Override
    public Transformation getWindowTransformation() {
        double w = 2.0 / canvas.getWidth();
        double h = 2.0 / canvas.getHeight();
        try {
            return TransformationFactory.getAffineTransformation(
                       new Vector3d(w, h, 1),
                       new Vector3d(-1, -1, 0)
                   );
        } catch (DegenerateMatrixException e) {
            // Should not occur as long as canvas have not infinite size.
            throw new Error("Canvas is to big.", e);
        }
    }

    @Override
    public Transformation getInverseWindowTransformation() {
        double w = canvas.getWidth() / 2;
        double h = canvas.getHeight() / 2;
        try {
            return TransformationFactory.getAffineTransformation(
                       new Vector3d(w, h, 1),
                       new Vector3d(w, h, 0)
                   );
        } catch (DegenerateMatrixException e) {
            // Should not occur as long as canvas have not infinite size.
            throw new Error("Canvas is to big.", e);
        }
    }

    @Override
    public Transformation getCanvasProjection() {
        double w = canvas.getWidth() / 2.0;
        double h = canvas.getHeight() / 2.0;
        try {
            Transformation windowTransformation = TransformationFactory.getAffineTransformation(
                    new Vector3d(w, h, 1),
                    new Vector3d(w, h, 0)
                                                  );
            return windowTransformation.rightTimes(getTransformation());
        } catch (DegenerateMatrixException e) {
            // Should not occur as long as canvas have not infinite size.
            throw new Error("Canvas is to big.", e);
        }
    }

    @Override
    public Transformation getG2DProjection() {
        double w = canvas.getWidth() / 2.0;
        double h = canvas.getHeight() / 2.0;
        try {
            Transformation windowTransformation = TransformationFactory.getAffineTransformation(
                    new Vector3d(w, -h, 1),
                    new Vector3d(w, h, 0)
                                                  );
            return windowTransformation.rightTimes(getTransformation());
        } catch (DegenerateMatrixException e) {
            // Should not occur as long as canvas have not infinite size.
            throw new Error("Canvas is to big.", e);
        }
    }

    @Override
    public Transformation getG2DSingleProjection() {
        double w = canvas.getWidth() / 2.0;
        double h = canvas.getHeight() / 2.0;
        try {
            Transformation windowTransformation = TransformationFactory.getAffineTransformation(
                    new Vector3d(w, -h, 1),
                    new Vector3d(w, h, 0)
                                                  );
            return windowTransformation.rightTimes(projectionStack.peek());
        } catch (DegenerateMatrixException e) {
            // Should not occur as long as canvas have not infinite size.
            throw new Error("Canvas is to big.", e);
        }
    }

    @Override
    public Transformation getG2DWindowProjection() {
        double h = canvas.getHeight();
        try {
            return TransformationFactory.getAffineTransformation(
                       new Vector3d(1, -1, 1),
                       new Vector3d(0, h, 0)
                   );
        } catch (DegenerateMatrixException e) {
            // Should not occur as long as canvas have not infinite size.
            throw new Error("Canvas is to big.", e);
        }
    }

    @Override
    public void reset() {
        modelViewStack.clear();
        projectionStack.clear();
    }

    @Override
    public void useWindowCoordinate() {
        if (isUsingSceneCoordinate()) {
            usingSceneCoordinate = false;
            fireTransformationChanged();
        }
    }

    @Override
    public void useSceneCoordinate() {
        if (!isUsingSceneCoordinate()) {
            usingSceneCoordinate = true;
            fireTransformationChanged();
        }
    }

    @Override
    public boolean isUsingSceneCoordinate() {
        return usingSceneCoordinate;
    }

    /**
     * Notify listeners the top transformation have changed.
     */
    protected void fireTransformationChanged() {
        for (TransformationManagerListener listener : listeners.getListeners(TransformationManagerListener.class)) {
            listener.transformationChanged(this);
        }
    }
}
