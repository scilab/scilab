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
package org.scilab.modules.gui.ged.graphic_objects.surface;

import javax.swing.JPanel;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.properties.ContouredObject;
import org.scilab.modules.gui.ged.graphic_objects.properties.Surface;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: Style/Appeareance.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends SimpleSection {
    private JPanel sectionPanel;
    private static Style instance;
    private Integer parentFigure;
    private Surface surface = new Surface();
    private ContouredObject contouredObject = new ContouredObject();

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of surface.
    */
    public Style(Integer objectID) {
        super(MessagesGED.style_appearance, "surface");
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
    * Add all the properties in this section.
    * @param objectID uid
    */
    @Override
    public final void initComponents(Integer objectID) {
        int row = 0;
        final int leftmargin = 16; //to inner components
        int column = 0; //first column
        parentFigure = (Integer) GraphicController.getController()
                       .getProperty(objectID, GraphicObjectProperties.__GO_PARENT_FIGURE__);

        //Components of the property: Color Flag.
        surface.colorFlag(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Color Mode.
        surface.colorFlag(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Hidden Color.
        surface.hiddenColor(sectionPanel, row++, column, leftmargin, objectID, parentFigure);

        //Components of the property: Foreground Color.
        contouredObject.foregroundColor(sectionPanel, row++, column, leftmargin,
                                        objectID, parentFigure);

        //Components of the property: Thickness.
        contouredObject.thickness(sectionPanel, row++, column, leftmargin, objectID);
    }
}