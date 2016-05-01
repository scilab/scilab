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

/**
 *
 * This is an interface for a transformation manager.
 * The transformation manager is used to manage transformation applied to vertex data sent for drawing.
 *
 * Coordinate are in first multiplied by the matrix at the top of ModelView matrix stack.
 * And then, the result is multiplied by the matrix at the top of Projection matrix stack.
 *
 * 'Projection' and 'ModelView' name refers to OpenGl 1.1.
 *
 * The rendered vertex are in the box [-1, 1]^3.
 *
 * @author Pierre Lando
 */
public interface TransformationManager {

    /**
     * Add a listener.
     * @param listener added.
     */
    void addListener(TransformationManagerListener listener);

    /**
     * Remove a listener.
     * @param listener removed.
     */
    void removeListener(TransformationManagerListener listener);


    /**
     * Return the model view matrix stack.
     * @return the model view matrix stack.
     */
    TransformationStack getModelViewStack();

    /**
     * Return the projection matrix stack.
     * @return the projection matrix stack.
     */
    TransformationStack getProjectionStack();

    /**
     * Return the top scene transformation.
     * @return the top scene transformation.
     */
    Transformation getTransformation();

    /**
     * Return the inverse of window transformation.
     * @return the inverse window transformation.
     */
    Transformation getInverseWindowTransformation();

    /**
     * Return the window transformation.
     * @return the window transformation.
     */
    Transformation getWindowTransformation();

    /**
     * Return the canvas projection matrix.
     * @return the canvas projection matrix.
     */
    Transformation getCanvasProjection();

    /**
     * Return the canvas projection matrix for Graphics2D.
     * @return the canvas projection matrix.
     */
    Transformation getG2DProjection();

    /**
     * Return the projection matrix (no modelView).
     * @return the projection matrix.
     */
    Transformation getG2DSingleProjection();

    /**
     * Return the window projection matrix for Graphics2D.
     * @return the window projection matrix.
     */
    Transformation getG2DWindowProjection();

    /**
     * Clear all stack.
     */
    void reset();


    /**
     * Switch to window coordinate.
     */
    void useWindowCoordinate();

    /**
     * Switch to scene coordinate.
     */
    void useSceneCoordinate();

    /**
     * Return true if is using scene coordinate.
     * @return true if is using scene coordinate.
     */
    boolean isUsingSceneCoordinate();
}
