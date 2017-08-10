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
package org.scilab.modules.gui.ged.graphic_objects.datatip;

import javax.swing.JPanel;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.ContouredObject;
import org.scilab.modules.gui.ged.graphic_objects.properties.Datatip;
import org.scilab.modules.gui.ged.graphic_objects.properties.Font;

/**
* Construction and startup of all components of the section: Label.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Label extends SimpleSection {
    private JPanel sectionPanel;
    private static Label instance;
    private ContouredObject contouredObject = new ContouredObject();
    private Datatip datatip = new Datatip();
    private Font font = new Font();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of datatip.
    */
    public Label(Integer objectID) {
        super(MessagesGED.label, "datatip");
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
        Integer parentFigure = (Integer) GraphicController.getController()
                               .getProperty(objectID, GraphicObjectProperties.__GO_PARENT_FIGURE__);

        //Components of the property: Tip Box Mode.
        datatip.tipBoxMode(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Background Color.
        contouredObject.backgroundColor(sectionPanel, row++, column, leftmargin, objectID, parentFigure);

        //Components of the property: Foreground Color.
        contouredObject.lineColor(sectionPanel, row++, column, leftmargin,
                                  objectID, parentFigure, "datatip.Label", this);

        //Components of the property: Font Foreground.
        font.fontColor(sectionPanel, row++, column, leftmargin, objectID, parentFigure);
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