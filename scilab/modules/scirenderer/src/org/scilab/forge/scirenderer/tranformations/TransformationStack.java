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
