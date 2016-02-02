/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
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
