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
package org.scilab.modules.gui.ged.graphic_objects.axes;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.Axes;

/**
* Construction and startup of all components of the section: Label.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Label extends SimpleSection {
    private JPanel sectionPanel;
    private static Label instance;
    private Axes axes = new Axes();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public Label(Integer objectID) {
        super(MessagesGED.label, "axes");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Label getInstance() {
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

        //Components of the property: Title
        axes.title(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Label
        axes.label(sectionPanel, row++, column, leftmargin, objectID);
    }
}