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
package org.scilab.modules.gui.ged.graphic_objects.axes;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.Axes;
import org.scilab.modules.gui.ged.graphic_objects.properties.TextObject;

/**
* Construction and startup of all components of the section: Font.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Tick extends SimpleSection {
    private JPanel sectionPanel;
    private static Tick instance;
    private Axes axes = new Axes();
    private TextObject textObject = new TextObject();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public Tick(Integer objectID) {
        super(MessagesGED.tick, "axes");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Tick getInstance() {
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

        //Components of the property:  Auto Ticks
        axes.autoTicks(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Font Color.
        textObject.fontColor(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Font Size.
        textObject.fontSize(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Font Style.
        textObject.fontStyle(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Fractional Font.
        textObject.fractionalFont(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Sub Ticks
        axes.subTicks(sectionPanel, row++, column, leftmargin, objectID);
        row++;

        //Components of the property: Axis Location
        axes.axisLocation(sectionPanel, row++, column, leftmargin, objectID);
        row++;
    }
}