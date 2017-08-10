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
import org.scilab.modules.gui.ged.graphic_objects.properties.TextObject;

/**
* Construction and startup of all components of the section: Font.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Font extends SimpleSection {
    private JPanel sectionPanel;
    private static Font instance;
    private Label label = new Label();
    private TextObject textObject = new TextObject();

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of object.
    */
    public Font(Integer objectID) {
        super(MessagesGED.font, "label");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Font getInstance() {
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

        //Components of the property: Font Angle
        label.fontAngle(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Font Color.
        textObject.fontColor(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Font Size.
        textObject.fontSize(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Font Style.
        textObject.fontStyle(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Fractional Font.
        textObject.fractionalFont(sectionPanel, row++, column, leftmargin, objectID);
    }
}