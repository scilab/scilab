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
package org.scilab.modules.gui.ged.graphic_objects.champ;

import javax.swing.JPanel;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.graphic_objects.properties.Champ;
import org.scilab.modules.gui.ged.graphic_objects.properties.ContouredObject;
import org.scilab.modules.gui.ged.graphic_objects.properties.VectField;

/**
* Construction and startup of all components of the section: Style/Appeareance.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Style extends SimpleSection {
    private JPanel sectionPanel;
    private static Style instance;
    private Champ champ = new Champ();
    private ContouredObject contouredObject = new ContouredObject();
    private VectField vectField = new VectField();

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of object.
    */
    public Style(Integer objectID) {
        super(MessagesGED.style_appearance, "champ");
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

        //Components of the property: Arrow Size
        vectField.arrowSize(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Colored
        champ.colored(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Line Style.
        contouredObject.lineStyle(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Thickness.
        contouredObject.thickness(sectionPanel, row++, column, leftmargin, objectID);
    }
}