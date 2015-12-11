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
* Construction and startup of all components of the section: Base Properties.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class BaseProperties extends SimpleSection {
    private static BaseProperties instance;
    private JPanel sectionPanel;
    private Figure figure = new Figure();
    private GraphicObject graphicObject = new GraphicObject();


    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public BaseProperties(Integer objectID) {
        super(MessagesGED.base_properties, "figure");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static BaseProperties getInstance() {
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

        //Components of the property: Anti-aliasing.
        figure.antiAliasing(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Auto-Resize.
        figure.autoResize(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Immediate Drawing
        figure.immediateDrawing(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Visible.
        graphicObject.visible(sectionPanel, row++, column, leftmargin, objectID);
    }
}
