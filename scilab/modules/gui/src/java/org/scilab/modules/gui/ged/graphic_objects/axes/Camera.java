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
import org.scilab.modules.gui.ged.graphic_objects.properties.CameraProperties;

/**
* Construction and startup of all components of the section: Camera.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Camera extends SimpleSection {
    private JPanel sectionPanel;
    private static Camera instance;
    private CameraProperties camera = new CameraProperties();

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of object.
    */
    public Camera(Integer objectID) {
        super(MessagesGED.camera, "axes");
        instance = this;
        sectionPanel = getSectionPanel();
        initComponents(objectID);
    }

    /**
     * Get instance
     * @return instance
     */
    public static Camera getInstance() {
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

        //Components of the property: Cube Scaling
        camera.cubeScaling(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Isometric View
        camera.isometricView(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: Rotation Angles.
        camera.rotationAngles(sectionPanel, row++, column, leftmargin, objectID);

        //Components of the property: View.
        camera.view(sectionPanel, row++, column, leftmargin, objectID);
    }
}