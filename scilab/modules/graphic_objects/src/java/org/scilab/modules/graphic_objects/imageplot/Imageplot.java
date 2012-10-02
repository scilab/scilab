/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.imageplot;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MATPLOT_SCALE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_MATPLOT_TRANSLATE__;

import org.scilab.modules.graphic_objects.contouredObject.ClippableContouredObject;

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
        return scale;
    }

    public void setScale(Double[] scale) {
        this.scale = scale;
    }
    
    public Double[] getTranslate() {
        return translate;
    }

    public void setTranslate(Double[] translate) {
        this.translate = translate;
    }
}
