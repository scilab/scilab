/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction.util;

import org.scilab.forge.scirenderer.tranformations.Vector3d;

/**
 * @author Pierre Lando
 */
public interface PointComputer {

    /** The number of axis */
    static final int AXIS_NUMBER = 3;

    /**
     * Validity getter.
     * @return true if the clicked point is valid.
     */
    public abstract boolean isValid();

    /**
     * First position getter.
     * @return the first position.
     */
    public abstract Vector3d getFirstPosition();

    /**
     * Second position getter.
     * @return the second position.
     */
    public abstract Vector3d getSecondPosition();

    /**
     * First axis index getter.
     * @return the first axis index.
     */
    public abstract int getFirstAxisIndex();

    /**
     * 2D status getter.
     * @return true if the zoom is in 2D mode.
     */
    public boolean is2D();
}
