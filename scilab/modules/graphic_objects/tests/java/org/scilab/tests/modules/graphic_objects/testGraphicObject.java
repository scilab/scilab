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

package org.scilab.tests.modules.graphic_objects;

import org.testng.annotations.*;

import java.rmi.server.UID;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;

/**
 * GraphicObject test class
 * @author Manuel JULIACHS
 */
public class testGraphicObject {

    public static final String VISIBLE_PROP_NAME = "Visible"; 
    public static final String NONEXISTING_PROP_NAME = "NonExistingProperty";
    public static final String PARENT_PROP_NAME = "Parent";

	@Test
    public void testGraphicObject() throws NullPointerException {
        GraphicController controller = GraphicController.getController();
        
        UID axesID = controller.askObject(Type.AXES);
        GraphicObject axes = (GraphicObject) controller.getObjectFromId(axesID);

        if (axes == null) {
        	throw new NullPointerException();
        }

        UID arcID = controller.askObject(Type.ARC);
        GraphicObject arc = (GraphicObject) controller.getObjectFromId(arcID);

      	if (arc == null) {
      		throw new NullPointerException();
      	}

      	UID axesIDret = axes.getIdentifier();
        UID arcIDret = arc.getIdentifier();

        assert axesID.equals(axesIDret);
        assert arcID.equals(arcIDret);

        Boolean arcVisible = new Boolean(true);

        controller.setPropertyFast(arcID, VISIBLE_PROP_NAME, arcVisible);
        Boolean arcVisibleRet = (Boolean) controller.getPropertyFast(arcID, VISIBLE_PROP_NAME);

        if (arcVisibleRet == null) {
        	throw new NullPointerException();
        }

        assert arcVisible.equals(arcVisibleRet);

        Object nonExistingProp = (Object) controller.getPropertyFast(arcID, NONEXISTING_PROP_NAME);

        assert nonExistingProp == null;

        controller.setPropertyFast(arcID, PARENT_PROP_NAME, axes);

        GraphicObject arcParent = (GraphicObject) controller.getPropertyFast(arcID, PARENT_PROP_NAME);

        if (arcParent == null) {
        	throw new NullPointerException();
        }

        assert arcParent.equals(axes);

    }
}
