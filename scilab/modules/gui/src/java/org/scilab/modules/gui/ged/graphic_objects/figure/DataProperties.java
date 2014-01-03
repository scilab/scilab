/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.figure;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.Figure;
import org.scilab.modules.gui.ged.graphic_objects.properties.GraphicObject;

/**
* Construction and startup of all components of the section: DataProperties.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends SimpleSection {
    private static DataProperties instance;
    private JPanel sectionPanel;
    private Figure figure = new Figure();
    private GraphicObject graphicObject = new GraphicObject();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public DataProperties(Integer objectID) {
        super(MessagesGED.data_properties, "figure");
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
        final int leftmargin = 16; //to inner components
        int column = 0; //first column

        //Components of the property: Figure ID
        figure.figureID(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Figure Name.
        figure.figureName(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Info Message.
        figure.infoMessage(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Tag.
        graphicObject.tag(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: User Data.
        graphicObject.userData(sectionPanel, row++, column, leftmargin);
    }
}