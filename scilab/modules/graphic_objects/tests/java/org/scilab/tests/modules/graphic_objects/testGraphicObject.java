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

import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.imageplot.Grayplot;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.surface.Fac3d;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.testng.annotations.Test;


/**
 * GraphicObject test class
 * @author Manuel JULIACHS
 */
public class testGraphicObject {

    public static final String VISIBLE_PROP_NAME = "Visible"; 
    public static final String NONEXISTING_PROP_NAME = "NonExistingProperty";
    public static final String PARENT_PROP_NAME = "Parent";

	@Test
    public void simpleTest() throws NullPointerException {
        GraphicController controller = GraphicController.getController();
        
        String axesID = controller.askObject(Type.AXES);
        GraphicObject axes = (GraphicObject) controller.getObjectFromId(axesID);

        assert(axes != null);

        String arcID = controller.askObject(Type.ARC);
        GraphicObject arc = (GraphicObject) controller.getObjectFromId(arcID);

      	assert(arc != null);

      	String axesIDret = axes.getIdentifier();
      	String arcIDret = arc.getIdentifier();

        assert axesID.equals(axesIDret);
        assert arcID.equals(arcIDret);

        Boolean arcVisible = new Boolean(true);

        controller.setProperty(arcID, VISIBLE_PROP_NAME, arcVisible);
        Boolean arcVisibleRet = (Boolean) controller.getProperty(arcID, VISIBLE_PROP_NAME);

        assert (arcVisibleRet != null);

        assert arcVisible.equals(arcVisibleRet);

        Object nonExistingProp = (Object) controller.getProperty(arcID, NONEXISTING_PROP_NAME);

        assert nonExistingProp == null;

        controller.setProperty(arcID, PARENT_PROP_NAME, axes);

        GraphicObject arcParent = (GraphicObject) controller.getProperty(arcID, PARENT_PROP_NAME);

        assert(arcParent != null);

        assert arcParent.equals(axes);

    }
	
	@Test
    public void arcCreationTest() {
	    GraphicController controller = GraphicController.getController();
	    
	    String objectId = null;
	    objectId = controller.askObject(Type.ARC);
	    assert(objectId != null);
	    
	    Object graphicObject = controller.getObjectFromId(objectId);
	    assert(graphicObject != null);
	    assert(graphicObject instanceof Arc);
	}

    @Test
    public void axesCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.AXES);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Axes);
    }
    
    @Test
    public void axisCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.AXIS);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Axis);
    }

    @Test
    public void champCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.CHAMP);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Champ);
    } 

    @Test
    public void compoundCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.COMPOUND);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Compound);
    } 
    
    @Test
    public void fac3dCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.FAC3D);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Fac3d);
    }

    @Test
    public void fecCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.FEC);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Fec);
    } 
    
    @Test
    public void figureCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.FIGURE);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Figure);
    }
    
    @Test
    public void grayplotCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.GRAYPLOT);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Grayplot);
    }
    
    @Test
    public void labelCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.LABEL);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Label);
    } 

    @Test
    public void legendCreationTest() {
        GraphicController controller = GraphicController.getController();
        
        String objectId = null;
        objectId = controller.askObject(Type.LEGEND);
        assert(objectId != null);
        
        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Legend);
    }   
    
}
