/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
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
package org.scilab.modules.gui.ged.graphic_objects.imageplot;

import javax.swing.JPanel;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.ClippableContouredObject;
import org.scilab.modules.gui.ged.graphic_objects.properties.GraphicObject;
import org.scilab.modules.gui.ged.graphic_objects.properties.Imageplot;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Data Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends SimpleSection {
    private JPanel sectionPanel;
    private static DataProperties instance;
    private ClippableContouredObject clippableContouredObject = new ClippableContouredObject();
    private Imageplot imageplot = new Imageplot();
    private GraphicObject graphicObject = new GraphicObject();


    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of imageplot.
    */
    public DataProperties(Integer objectID) {
        super(MessagesGED.data_properties, "imageplot");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static DataProperties getInstance() {
        return instance;
    }

    /**
    * Add all the properties in this section.
    * @param objectID uid
    */
    @Override
    public final void initComponents(Integer objectID) {
        int row = 0;
        final int leftmargin = 0; //to inner components
        int column = 1; //first column

        clippableContouredObject.clipState(sectionPanel, row++, column, leftmargin, objectID);

        clippableContouredObject.clipBox(sectionPanel, row, column, leftmargin, objectID);
        row += 2;

        //Components of the property: Data Mapping (only to grayplot).
        if (isGrayplot(objectID)) {
            imageplot.dataMapping(sectionPanel, row++, column, leftmargin, objectID);
        }

        graphicObject.tag(sectionPanel, row++, column, leftmargin, objectID);

        graphicObject.userData(sectionPanel, row++, column, leftmargin);
    }

    /**
    * Check type.
    * @param objectID uid
    * @return if true, is grayplot
    */
    private boolean isGrayplot(Integer objectID) {
        Integer type = (Integer) GraphicController.getController()
                       .getProperty(objectID, GraphicObjectProperties.__GO_TYPE__);
        if (type == GraphicObjectProperties.__GO_GRAYPLOT__) {
            return true;
        } else {
            return false;
        }
    }
}