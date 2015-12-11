/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.arc;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.Arc;

/**
* Construction and startup of all components of the section: Data Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends SimpleSection {
    private JPanel sectionPanel;
    private static DataProperties instance;
    private Arc arc = new Arc();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public DataProperties(Integer objectID) {
        super(MessagesGED.data_properties, "arc");
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

        //Components of the property: Data
        //Upper Left Point
        arc.upperLeftPoint(sectionPanel, row++, column, leftmargin, objectID);
        row++;

        //Width
        arc.width(sectionPanel, row++, column, leftmargin, objectID);

        //Height
        arc.height(sectionPanel, row++, column, leftmargin, objectID);

        //Start Angle
        arc.startAngle(sectionPanel, row++, column, leftmargin, objectID);

        //End Angle
        arc.endAngle(sectionPanel, row++, column, leftmargin, objectID);
    }
}