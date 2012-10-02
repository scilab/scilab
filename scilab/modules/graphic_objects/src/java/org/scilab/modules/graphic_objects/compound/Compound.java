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

package org.scilab.modules.graphic_objects.compound;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

/**
 * Compound object class
 * @author Manuel JULIACHS
 */
public class Compound extends GraphicObject {

	/** Constructor */
	public Compound() {
		super();
	}

    @Override
    public void accept(Visitor visitor) {
        visitor.visit(this);
    }

    /**
	 * @return Type as String
	 */
	public Integer getType() {
		return GraphicObjectProperties.__GO_COMPOUND__;
	}

}
