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

package org.scilab.forge.scirenderer.tranformations;

/**
 * @author Pierre Lando
 */
public interface TransformationStack {

    /**
     * Enum of possible transformation stack event.
     */
    enum TransformationStackEvent {
        /**
         * When a matrix have been popped.
         */
        POPPED,

        /**
         * When a matrix have been pushed.
         */
        PUSHED,

        /**
         * When the stack have been cleared.
         */
        CLEARED
    }


    /**
     * Add a listener.
     * @param listener added listener.
     */
    void addListener(TransformationStackListener listener);

    /**
     * Remove a listener.
     * @param listener removed listener.
     */
    void removeListener(TransformationStackListener listener);

    /**
     * Return the top transformation.
     * @return the top transformation.
     */
    Transformation peek();

    /**
     * Push the given transformation on the stack.
     * @param transformation the given transformation.
     */
    void push(Transformation transformation);

    /**
     * Push the given transformation right time the peek on the stack.
     * @param transformation the given transformation.
     */
    void pushRightMultiply(Transformation transformation);

    /**
     * Push the given transformation left time the peek on the stack.
     * @param transformation the given transformation.
     */
    void pushLeftMultiply(Transformation transformation);

    /**
     * Pop one matrix on the stack.
     * @return the popped matrix.
     */
    Transformation pop();

    /**
     * Pop all matrix on the stack except identity.
     */
    void clear();
}
