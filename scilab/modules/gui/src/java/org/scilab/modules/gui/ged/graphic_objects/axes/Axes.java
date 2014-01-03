/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 2013 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.axes;

import org.scilab.modules.gui.ged.graphic_objects.SimpleObject;

/**
* Properties of the Axes.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class Axes extends SimpleObject {
    private BaseProperties base;
    private Box box;
    private Camera camera;
    private Tick tick;
    private Label label;
    private Position position;
    private Style style;

    /**
    * Initializes all sections (JPanel's) and Add in Main JPanel of Object.
    * @param objectID Enters the identification of object.
    */
    public Axes(Integer objectID) {
        super(objectID);
        base = new BaseProperties(objectID);
        box = new Box(objectID);
        camera = new Camera(objectID);
        tick = new Tick(objectID);
        label = new Label(objectID);
        position = new Position(objectID);
        style = new Style(objectID);

        addSectionPanel(this, base, getPosition(base.getName()));
        addSectionPanel(this, box, getPosition(box.getName()));
        addSectionPanel(this, camera, getPosition(camera.getName()));
        addSectionPanel(this, tick, getPosition(tick.getName()));
        addSectionPanel(this, label, getPosition(label.getName()));
        addSectionPanel(this, position, getPosition(position.getName()));
        addSectionPanel(this, style, getPosition(style.getName()));
        this.fillerV(getSectionsName());
    }

    /**
    * Get the name of all sections.
    * @return sections name
    */
    @Override
    public final String[] getSectionsName() {
        return new String[] {
                   base.getName(),
                   box.getName(),
                   camera.getName(),
                   tick.getName(),
                   label.getName(),
                   position.getName(),
                   style.getName()
               };
    }
}