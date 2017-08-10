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

package org.scilab.modules.graphic_objects.surface;

import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

/**
 * Plot3d class
 * @author Manuel JULIACHS
 */
public class Plot3d extends Surface {
    // TBD Data -> Data Model
    /* TBD: properties relative to the data model */

    /** Constructor */
    public Plot3d() {
        super();
        // TODO Auto-generated constructor stub
    }

    @Override
    public void accept(Visitor visitor) throws ObjectRemovedException {
        visitor.visit(this);
    }

    /**
     * @return Type as String
     */
    public Integer getType() {
        return GraphicObjectProperties.__GO_PLOT3D__;
    }

}
