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

import javax.swing.JColorChooser;
import javax.swing.JPanel;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.datatip.DatatipManagerMode;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.customComponents.JComboBox.MarkStyleRenderer;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.ContouredObject;

/**
* Construction and startup of all components of the section: Mark Properties.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Mark extends SimpleSection {
    private JPanel sectionPanel;
    private static Mark instance;
    private final MarkStyleRenderer markStyleRenderer = new MarkStyleRenderer();
    private static JColorChooser chooserMarkBackground;
    private static JColorChooser chooserMarkForeground;
    private ContouredObject contouredObject = new ContouredObject();

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of datatip.
    */
    public Mark(Integer objectID) {
        super(MessagesGED.mark, "datatip");
        instance = this;
        sectionPanel = getSectionPanel();
        chooserMarkBackground = new JColorChooser();
        chooserMarkForeground = new JColorChooser();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Mark getInstance() {
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

        //Components of the property: Mark Mode.
        contouredObject.markMode(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Mark Background.
        contouredObject.markBackgroundColor(sectionPanel, markStyleRenderer, chooserMarkBackground,
                                            row++, column, leftmargin,
                                            objectID, parentFigure, "datatip.Mark", this);

        //Components of the property: Mark Foreground.
        contouredObject.markForegroundColor(sectionPanel, markStyleRenderer, chooserMarkForeground,
                                            row++, column, leftmargin,
                                            objectID, parentFigure, "datatip.Mark", this);

        //Components of the property: Mark Size.
        contouredObject.markSize(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Mark Size Unit.
        contouredObject.markSizeUnit(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Mark Style.
        contouredObject.markStyle(sectionPanel, markStyleRenderer, row++, column, leftmargin, objectID);
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setMarkBackground(int scilabColor, String UID) {
        DatatipManagerMode.getInstance().setMarkColor(scilabColor);
        //update color of graphics in MarkStyle ComboBox
        markStyleRenderer.setMarkBackground(chooserMarkBackground.getColor());
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setMarkForeground(int scilabColor, Integer UID) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_MARK_FOREGROUND__, scilabColor);
        //update color of graphics in MarkStyle ComboBox
        markStyleRenderer.setMarkForeground(chooserMarkForeground.getColor());
    }
}