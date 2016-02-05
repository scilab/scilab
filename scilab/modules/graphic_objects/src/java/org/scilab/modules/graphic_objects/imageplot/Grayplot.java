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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_DATA_MAPPING__;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.utils.DataMapping;
/**
 * Grayplot class
 * @author Manuel JULIACHS
 */
public class Grayplot extends Imageplot {
    /* TBD: properties relative to the data model */
    /** Grayplot properties names */
    private enum GrayplotProperty { DATAMAPPING };

    /** Specifies how colors are mapped to values */
    private DataMapping dataMapping;

    /** Constructor */
    public Grayplot() {
        super();
        dataMapping = DataMapping.DIRECT;
    }

    @Override
    public void accept(Visitor visitor) throws ObjectRemovedException {
        visitor.visit(this);
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        if (propertyName == __GO_DATA_MAPPING__) {
            return GrayplotProperty.DATAMAPPING;
        } else {
            return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == GrayplotProperty.DATAMAPPING) {
            return getDataMapping();
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
        if (property == GrayplotProperty.DATAMAPPING) {
            setDataMapping(DataMapping.intToEnum((Integer) value));
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * @return the dataMapping
     */
    public DataMapping getDataMapping() {
        return dataMapping;
    }

    /**
     * @param dataMapping the dataMapping to set
     */
    public UpdateStatus setDataMapping(DataMapping dataMapping) {
        this.dataMapping = dataMapping;
        return UpdateStatus.Success;
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_GRAYPLOT__;
    }

}
