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
 *
 */

package org.scilab.modules.gui.editor;

import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;


/**
 * Implements all legend manipulation functions for the editor.
 *
 * @author Caio Souza <caioc2bolado@gmail.com>
 * @author Pedro Souza <bygrandao@gmail.com>
 *
 * @since 2012-06-01
 */
public class LegendHandler {

    public static Integer searchLegend(Integer uid) {
        if (uid == null) {
            return null;
        }

        int childCount = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
        Integer[] child = (Integer[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN__);
        if (child != null) {
            for (int i = 0; i < childCount; i++) {
                Object o = GraphicController.getController().getProperty(child[i], GraphicObjectProperties.__GO_TYPE__);
                if (o instanceof Integer) {
                    Integer type = (Integer) o;
                    if (type == GraphicObjectProperties.__GO_LEGEND__) {
                        return child[i];
                    }
                }
            }
        }
        return null;
    }

    /**
     * Set or create a legend for one polyline.
     *
     * @param axes The axes that will recieve the legend.
     * @param polyline The polyline to set/create legend.
     * @param text The text for the legend.
     */

    public static void setLegend(Integer legend, Integer axes, Integer polyline, String text) {
        if (text == null || text == "" || polyline == null || axes == null || legend == null) {
            return;
        }

        Integer[] dimension = { 0, 0 };
        ArrayList<String> texts = new ArrayList<String>();
        ArrayList<Integer> links = new ArrayList<Integer>();
        texts.add(text);
        if (legend != null) {
            String[] textOld = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            Integer[] linksOld = (Integer[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_LINKS__);
            int length = linksOld.length;
            for (int i = 0; i < length; i++) {
                if (linksOld[i].equals(polyline)) {
                    textOld[length - i - 1] = text;
                    GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__, textOld);
                    return;
                }
            }

            for (Integer i = 0; i < length; i++) {
                texts.add(textOld[i]);
                links.add(linksOld[i]);
            }
            CommonHandler.delete(legend);
        }
        legend = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_LEGEND__));
        GraphicController.getController().setGraphicObjectRelationship(axes, legend);

        links.add(polyline);
        String[] textToSet = new String[texts.size()];
        texts.toArray(textToSet);
        Integer[] linksToSet = new Integer[links.size()];
        links.toArray(linksToSet);
        dimension[0] = linksToSet.length;
        dimension[1] = 1;
        if (dimension[0] != 0) {
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LINE_MODE__, true);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__, textToSet);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LINKS__, linksToSet);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 0);
        }
    }

    /**
     * Remove a legend from axes.
     *
     * @param axes The axes to remove the legend.
     * @param polyline The polyline that is linked to the legend.
     */

    public static void removeLegend(Integer axes, Integer polyline) {
        if (polyline == null || axes == null) {
            return;
        }

        Integer legend = searchLegend(axes);
        Integer[] dimension = { 0, 0 };
        ArrayList<String> texts = new ArrayList<String>();
        ArrayList<Integer> links = new ArrayList<Integer>();
        if (legend == null) {
            return;
        } else {
            String[] textOld = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__);
            Integer[] linksOld = (Integer[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_LINKS__);
            int remove = -1;
            for (int i = 0; i < linksOld.length; i++) {
                if (polyline != linksOld[i]) {
                    links.add(linksOld[i]);
                } else {
                    remove = i;
                }
            }
            for (int i = 0; i < textOld.length; i++) {
                if (i != (textOld.length - remove - 1)) {
                    texts.add(textOld[i]);
                }
            }
        }
        String[] textToSet = new String[texts.size()];
        texts.toArray(textToSet);
        Integer[] linksToSet = new Integer[links.size()];
        links.toArray(linksToSet);
        dimension[0] = linksToSet.length;
        dimension[1] = 1;
        if (dimension[0] != 0) {
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, dimension);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LINE_MODE__, true);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__, textToSet);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LINKS__, linksToSet);
            GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 0);
        } else {
            CommonHandler.delete(legend);
        }
    }

    /**
     * Get the text of the legend.
     *
     * @param axes The axes where is the legend.
     * @param polyline The polyline that is linked to the legend.
     * @return The text of the legend.
     */

    public static String getLegendText(Integer axes, Integer polyline) {
        if (polyline != null && axes != null) {
            Integer legend = searchLegend(axes);
            if (legend == null) {
                return null;
            } else {
                String[] textOld = (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__);
                Integer[] linksOld = (Integer[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_LINKS__);
                for (int i = 0; i < linksOld.length; i++) {
                    if (linksOld[i] == polyline) {
                        return textOld[linksOld.length - i - 1];
                    }
                }
            }
        }
        return null;
    }

    /**
     * Drag the given legend.
     *
     * @param legend  the legend uid.
     * @param position click position (x,y).
     * @param nextPosition dragged position (x,y).
     */

    public static void dragLegend(Integer legend, Integer[] position, Integer[] nextPosition) {
        ObjectSearcher searcher = new ObjectSearcher();
        Integer axes = searcher.searchParent(legend, GraphicObjectProperties.__GO_AXES__);
        Integer figure = searcher.searchParent(legend, GraphicObjectProperties.__GO_FIGURE__);
        Integer[] axesSize = { 0, 0 };
        Double[] axesBounds = { 0., 0. }, diff = { 0., 0. }, legendPos = { 0., 0. };
        if (legend == null || axes == null || figure == null) {
            return;
        }

        axesSize = (Integer[])GraphicController.getController().getProperty(figure, GraphicObjectProperties.__GO_AXES_SIZE__);
        axesBounds = (Double[])GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_AXES_BOUNDS__);
        legendPos = (Double[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_POSITION__);
        if ( nextPosition[0] > (axesSize[0] - 10) || nextPosition[1] > (axesSize[1] - 10)) {
            return;
        }
        diff[0] = (nextPosition[0] - position[0]) / (axesBounds[2] * axesSize[0]);
        diff[1] = (nextPosition[1] - position[1]) / (axesBounds[3] * axesSize[1]);
        legendPos[0] += diff[0];
        legendPos[1] += diff[1];
        GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_LEGEND_LOCATION__, 10);
        GraphicController.getController().setProperty(legend, GraphicObjectProperties.__GO_POSITION__, legendPos);
    }

    /**
     * Get the links of a Legend
     *
     * @param legend The legend to get the links
     * @return Links of the legend
     */
    public static Integer[] getLinks(Integer legend) {
        if (legend == null) {
            return null;
        } else {
            return (Integer[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_LINKS__);
        }
    }

    /**
     * Get the text of a Legend
     *
     * @param legend The legend to get the text
     * @return Text of the legend
     */
    public static String[] getText(Integer legend) {
        if (legend == null) {
            return null;
        } else {
            return (String[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        }
    }

    /**
     * Get the position of a Legend
     *
     * @param legend The legend to get the position
     * @return The legend position
     */
    public static Double[] getPosition(Integer legend) {
        if (legend == null) {
            return null;
        } else {
            return (Double[])GraphicController.getController().getProperty(legend, GraphicObjectProperties.__GO_POSITION__);
        }
    }
}
