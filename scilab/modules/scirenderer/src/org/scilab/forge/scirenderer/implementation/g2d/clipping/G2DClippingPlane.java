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

package org.scilab.forge.scirenderer.implementation.g2d.clipping;

import org.scilab.forge.scirenderer.clipping.ClippingPlane;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector4d;
import org.scilab.forge.scirenderer.implementation.g2d.G2DDrawingTools;

/**
 * @author Pierre Lando
 */
public class G2DClippingPlane implements ClippingPlane {

    /**
     * GL index of this clipping plane.
     */
    private final int index;

    /**
     * Clipping plane enabled status.
     */
    private boolean isEnable;

    private G2DDrawingTools drawingTools;

    /**
     * Clipping plane equation look like: {@code x*a + y*b + z*c + d = 0}.
     * Where {@code equation} is {@code [a, b, c, d]}.
     */
    private Vector4d equation = new Vector4d(0, 0, 0, 0);
    private Vector4d projectedEquation = new Vector4d(0, 0, 0, 0);
    private Transformation transformation = TransformationFactory.getIdentity();
    private Transformation projectionTransf;
    private boolean updated;

    /**
     * Default constructor.
     * @param index the id of this clipping plane.
     */
    public G2DClippingPlane(int index, G2DDrawingTools drawingTools) {
        this.isEnable = false;
        this.index = index;
        this.drawingTools = drawingTools;
    }

    @Override
    public boolean isEnable() {
        return isEnable;
    }

    @Override
    public void setEnable(boolean isEnable) {
        this.isEnable = isEnable;
    }

    @Override
    public void setEquation(Vector4d v) {
        Transformation t = drawingTools.getTransformationManager().getModelViewStack().peek();
        if (t != null) {
            equation = getTransformedEquation(v, t);
        }
    }

    @Override
    public Vector4d getEquation() {
        Transformation t = drawingTools.getMotor3D().getCurrentSingleTransformation();
        if (t != null && projectionTransf != t) {
            projectionTransf = t;
            projectedEquation = getTransformedEquation(equation, t);
        }

        return projectedEquation;
    }

    @Override
    public void setTransformation(Transformation transformation) { }

    @Override
    public Transformation getTransformation() {
        return transformation;
    }

    @Override
    public int getIndex() {
        return index;
    }

    private static final Vector4d getTransformedEquation(Vector4d equation, Transformation transf) {
        double[] matrix = transf.getInverseTransformation().getMatrix();
        double[] v = equation.getData();
        double x = matrix[0] * v[0] + matrix[1] * v[1] +  matrix[2] * v[2] + matrix[3] * v[3];
        double y = matrix[4] * v[0] + matrix[5] * v[1] +  matrix[6] * v[2] + matrix[7] * v[3];
        double z = matrix[8] * v[0] + matrix[9] * v[1] +  matrix[10] * v[2] + matrix[11] * v[3];
        double w = matrix[12] * v[0] + matrix[13] * v[1] +  matrix[14] * v[2] + matrix[15] * v[3];

        return new Vector4d(x, y, z, w);
    }
}
