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
package org.scilab.modules.gui.ged.graphic_objects.arc;

import javax.swing.JPanel;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.ContouredObject;

/**
* Construction and startup of all components of the section: Style/Appeareance.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends SimpleSection {
    private JPanel sectionPanel;
    private static Style instance;
    private ContouredObject contouredObject = new ContouredObject();

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of object.
    */
    public Style(Integer objectID) {
        super(MessagesGED.style_appearance, "arc");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Style getInstance() {
        return instance;
    }

    /**
    * Initialize the Components.
    */
    @Override
    public final void initComponents(Integer objectID) {
        int row = 0;
        final int leftmargin = 16; //to inner components
        int column = 0; //first column

        Integer parentFigure = (Integer) GraphicController.getController()
                               .getProperty(objectID, GraphicObjectProperties.__GO_PARENT_FIGURE__);

        //Components of the property: Fill Mode
        contouredObject.fillMode(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Background Color
        contouredObject.backgroundColor(sectionPanel, row++, column, leftmargin, objectID, parentFigure);

        //Components of the property: Line Mode
        contouredObject.lineMode(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Foreground Color
        contouredObject.lineColor(sectionPanel, row++, column, leftmargin,
                                  objectID, parentFigure, "arc.Style", this);

        //Components of the property: Line Style
        contouredObject.lineStyle(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Thickness
        contouredObject.thickness(sectionPanel, row++, column, leftmargin, objectID);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    * @param UID objectID.
    */
    public void setForegroundColor(int scilabColor, Integer UID) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    }
}