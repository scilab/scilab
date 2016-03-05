/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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
package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.BaseAction;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.gui.editor.LegendHandler;

/**
* Implements the object to store legend related actions
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class ActionLegend extends BaseAction {

    /* Store the old links and text of the legend */
    Integer[] oldLinks, newLinks;

    /* Store the current links and text of the legend */
    String[] oldText, newText;

    /* Store the position of the legend */
    Double[] position;

    /**
    * Constructor
    *
    * @param parent, the axes where are the legend
    * @param links the old links from the legend
    * @param text the old text of the legend
    * @param pos The legend position
    */
    public ActionLegend(Integer parent, Integer[] links, String[] text, Double[] pos) {

        super(null, parent);
        if ( links != null ) {
            oldLinks = new Integer[links.length];
            oldText = new String[links.length];
            for (Integer i = 0; i < links.length; i++ ) {
                oldLinks[i] = links[i];
                oldText[i] = text[i];
            }
        }
        objectUID = LegendHandler.searchLegend(parent);
        if (objectUID != null) {
            newLinks = (Integer[])GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_LINKS__);
            newText = (String[])GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        }
        if (pos != null) {
            position = new Double[2];
            this.position[0] = pos[0];
            this.position[1] = pos[1];
        } else {
            position = null;
        }
    }

    /**
    * Undo, set the legend links to the old ones, if the old ones are null then remove the legend
    */
    public void undo() {

        objectUID = LegendHandler.searchLegend(parentUID);
        if (position == null) {
            position = (Double[])GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_POSITION__);
        }
        if (objectUID == null) {
            objectUID = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_LEGEND__));
            Integer[] dimension = {oldLinks.length, 1};
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINE_MODE__, true);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, oldText);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINKS__, oldLinks);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 10);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_POSITION__, position);
            setRelation(objectUID, parentUID);
        } else {
            if (oldLinks == null) {
                GraphicController.getController().removeRelationShipAndDelete(objectUID);
            } else {
                Integer[] dimension = {oldLinks.length, 1};
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, oldText);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINKS__, oldLinks);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 10);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_POSITION__, position);
            }
        }
    }

    /**
    * Redo, set the legend links to the new ones, if the new ones are null then remove the legend
    */
    public void redo() {

        objectUID = LegendHandler.searchLegend(parentUID);
        if (position == null) {
            position = (Double[])GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_POSITION__);
        }
        if (objectUID == null) {
            objectUID = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_LEGEND__));
            Integer[] dimension = {newLinks.length, 1};
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINE_MODE__, true);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, newText);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINKS__, newLinks);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 10);
            GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_POSITION__, position);
            setRelation(objectUID, parentUID);
        } else {
            if (newLinks == null) {
                GraphicController.getController().removeRelationShipAndDelete(objectUID);
            } else {
                Integer[] dimension = {newLinks.length, 1};
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_TEXT_STRINGS__, newText);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LINKS__, newLinks);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 10);
                GraphicController.getController().setProperty(objectUID, GraphicObjectProperties.__GO_POSITION__, position);
            }
        }
    }

    public void dispose() {

        objectUID = null;
        parentUID = null;
    }

    @Override
    public boolean exists() {

        GraphicObject object = GraphicController.getController().getObjectFromId(parentUID);
        if (object != null) {
            return true;
        } else {
            return false;
        }
    }
}