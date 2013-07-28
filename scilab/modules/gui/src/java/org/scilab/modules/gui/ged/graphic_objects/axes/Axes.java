/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 2013 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.axes;

import java.awt.Dimension;
import java.util.Arrays;
import javax.swing.Box.Filler;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.graphic_objects.SimpleObject;

/**
* Properties of the Axes.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Axes extends ContentLayout implements SimpleObject {
    private String objectID;
    private ContentLayout layout = new ContentLayout();
    private String [] sections;
    private BaseProperties base;
    private Box box;
    private Camera camera;
    private Label label;
    private Position position;

    /**
    * Initializes all sections (JPanel's) and Add in Main JPanel of Object.
    * @param objectID Enters the identification of figure.
    */
    @Override
    public final void initSections(String objectID) {
        setObjectID(objectID);
        base = new BaseProperties(objectID);
        box = new Box(objectID);
        camera = new Camera(objectID);
        label = new Label(objectID);
        position = new Position(objectID);

        layout.addSectionPanel(this, base, getPosition(base.getName()));
        layout.addSectionPanel(this, box, getPosition(box.getName()));
        layout.addSectionPanel(this, camera, getPosition(camera.getName()));
        layout.addSectionPanel(this, label, getPosition(label.getName()));
        layout.addSectionPanel(this, position, getPosition(position.getName()));
        fillerV();
    }

    /**
    * Get the position of section for current language (alphabetic order).
    * @param section Name of section.
    * @return position
    */
    @Override
    public final int getPosition(String section) {
        sections = new String[] {
            base.getName(),
            box.getName(),
            camera.getName(),
            label.getName(),
            position.getName()
        };
        Arrays.sort(sections);
        return Arrays.binarySearch(sections, section);
    }

    /**
     * Add a vertical filler - makes the jpanels are always on top.
     */
    private void fillerV() {
        Filler filler = new Filler(new Dimension(1, 1), new Dimension(1, 1), new Dimension(1, 32767));
        layout.addFiller(this, filler, sections.length);
    }

    /**
    * Set the Object ID.
    * @param objectID Enters the identification of object.
    */
    @Override
    public final void setObjectID(String objectID) {
        this.objectID = objectID;
    }

    /**
    * Get the Object ID.
    * @return Object ID
    */
    @Override
    public final String getObjectID() {
        return objectID;
    }

    /**
    * Get Parent Figure (ID).
    * @param objectID Enters the identification of object.
    * @return FigreID
    */
    public final String getFigueID(String objectID) {
        return (String) GraphicController.getController().
                getProperty(objectID, GraphicObjectProperties.__GO_PARENT_FIGURE__);
    }
}