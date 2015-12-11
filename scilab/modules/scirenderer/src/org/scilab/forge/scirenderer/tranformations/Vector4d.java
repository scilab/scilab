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
public class Vector4d {
    private final double[] data;


    public Vector4d(double x, double y, double z, double w) {
        data = new double[] {x, y, z, w};
    }

    public double[] getData() {
        return data.clone();
    }

    public String toString() {
        return "[" + data[0] + ", " + data[1] + ", " + data[2] + ", " + data[3] + "]";
    }
}
