/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graphic_objects.imageplot;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MATPLOT_SCALE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MATPLOT_TRANSLATE__;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * ImagePlot class
 * @author Manuel JULIACHS
 */
public abstract class Imageplot extends ClippableContouredObject {
    // TBD
    // Data -> Data Model
    /** Grayplot properties names */
    private enum ImageplotProperty { SCALE, TRANSLATE };
    // Methods: to be done

    private Double[] scale = {1.0, 1.0};
    private Double[] translate = {0.5, 0.5};

    /** Constructor */
    public Imageplot() {
        super();
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_MATPLOT_SCALE__ :
                return ImageplotProperty.SCALE;
            case __GO_MATPLOT_TRANSLATE__ :
                return ImageplotProperty.TRANSLATE;
            default :
                return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == ImageplotProperty.TRANSLATE) {
            return getTranslate();
        } else if (property == ImageplotProperty.SCALE) {
            return getScale();
        } else {
            return super.getProperty(property);
        }
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == ImageplotProperty.TRANSLATE) {
            setTranslate((Double[]) value);
        } else if (property == ImageplotProperty.SCALE) {
            setScale((Double[]) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    public Double[] getScale() {
        return new Double[] {scale[0], scale[1]};
    }

    public UpdateStatus setScale(Double[] scale) {
        this.scale = scale;
        return UpdateStatus.Success;
    }

    public Double[] getTranslate() {
        return new Double[] {translate[0], translate[1]};
    }

    public UpdateStatus setTranslate(Double[] translate) {
        this.translate = translate;
        return UpdateStatus.Success;
    }
}
