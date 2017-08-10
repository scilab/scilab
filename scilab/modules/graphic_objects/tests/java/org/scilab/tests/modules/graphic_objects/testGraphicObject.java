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

package org.scilab.tests.modules.graphic_objects;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;

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
import org.scilab.modules.graphic_objects.imageplot.Matplot;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.rectangle.Rectangle;
import org.scilab.modules.graphic_objects.surface.Fac3d;
import org.scilab.modules.graphic_objects.surface.Plot3d;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;
import org.junit.*;


/**
 * GraphicObject test class
 * @author Manuel JULIACHS
 */
public class testGraphicObject {

    public static final int NONEXISTING_PROP = Integer.MAX_VALUE;

    @Test
    public void simpleTest() throws NullPointerException {
        GraphicController controller = GraphicController.getController();

        Integer axesID = controller.askObject(Type.AXES);
        GraphicObject axes = (GraphicObject) controller.getObjectFromId(axesID);

        assert(axes != null);

        Integer arcID = controller.askObject(Type.ARC);
        GraphicObject arc = (GraphicObject) controller.getObjectFromId(arcID);

        assert(arc != null);

        Integer axesIDret = axes.getIdentifier();
        Integer arcIDret = arc.getIdentifier();

        assert axesID.equals(axesIDret);
        assert arcID.equals(arcIDret);

        Boolean arcVisible = new Boolean(true);

        controller.setProperty(arcID, __GO_VISIBLE__, arcVisible);
        Boolean arcVisibleRet = (Boolean) controller.getProperty(arcID, __GO_VISIBLE__);

        assert (arcVisibleRet != null);

        assert arcVisible.equals(arcVisibleRet);

        Object nonExistingProp = (Object) controller.getProperty(arcID, NONEXISTING_PROP);

        assert nonExistingProp == null;

        controller.setProperty(arcID, __GO_PARENT__, axes.getIdentifier());

        Integer arcParentId = (Integer) controller.getProperty(arcID, __GO_PARENT__);
        GraphicObject arcParent = (GraphicObject) controller.getObjectFromId(arcParentId);

        assert(arcParent != null);

        assert arcParent.equals(axes);

    }

    @Test
    public void arcCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_ARC__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Arc);
    }

    @Test
    public void arcCloneTest() {
        GraphicController controller = GraphicController.getController();
        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_ARC__));
        Integer cloneId = null;
        cloneId = controller.cloneObject(objectId);

        assert(cloneId != null);
        Object graphicObject = controller.getObjectFromId(cloneId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Arc);
    }

    @Test
    public void axesCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_AXES__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Axes);
    }

    @Test
    public void axesCloneTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_AXES__));
        Integer cloneId = null;
        cloneId = controller.cloneObject(objectId);

        assert(cloneId != null);
        Object graphicObject = controller.getObjectFromId(cloneId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Axes);
    }

    @Test
    public void axisCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_AXIS__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Axis);
    }

    @Test
    public void champCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_CHAMP__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Champ);
    }

    @Test
    public void compoundCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_COMPOUND__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Compound);
    }

    @Test
    public void fac3dCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_FAC3D__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Fac3d);
    }

    @Test
    public void fecCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_FEC__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Fec);
    }

    @Test
    public void figureCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_FIGURE__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Figure);
    }

    @Test
    public void grayplotCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_GRAYPLOT__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Grayplot);
    }

    @Test
    public void labelCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_LABEL__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Label);
    }

    @Test
    public void legendCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_LEGEND__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Legend);
    }

    @Test
    public void matplotCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_MATPLOT__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Matplot);
    }

    @Test
    public void plot3dCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_PLOT3D__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Plot3d);
    }

    @Test
    public void polylineCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_POLYLINE__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Polyline);
    }

    @Test
    public void retangleCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_RECTANGLE__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Rectangle);
    }

    @Test
    public void segsCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_SEGS__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Segs);
    }

    @Test
    public void textCreationTest() {
        GraphicController controller = GraphicController.getController();

        Integer objectId = null;
        objectId = controller.askObject(GraphicObject.getTypeFromName(__GO_TEXT__));
        assert(objectId != null);

        Object graphicObject = controller.getObjectFromId(objectId);
        assert(graphicObject != null);
        assert(graphicObject instanceof Text);
    }

}
