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
package org.scilab.modules.gui.ged.graphic_objects.label;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.Label;

/**
* Construction and startup of all components of the section: Position.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Position extends SimpleSection {
    private JPanel sectionPanel;
    private static Position instance;
    private Label label = new Label();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public Position(Integer objectID) {
        super(MessagesGED.position, "label");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Position getInstance() {
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

        //Components of the property: Auto Rotation.
        label.autoRotation(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Auto Position.
        label.autoPosition(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Position.
        label.position(sectionPanel, row++, column, leftmargin, objectID);
    }
}