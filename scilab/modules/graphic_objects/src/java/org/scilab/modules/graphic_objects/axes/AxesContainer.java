/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab enterprises - Bruno JOFRET
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

package org.scilab.modules.graphic_objects.axes;

import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

public interface AxesContainer {

    public Integer getIdentifier();
    public void accept(Visitor visitor);
    public Integer getAntialiasing();
    public Integer[] getAxesSize();
    public Integer[] getChildren();

    public Integer getBackground();
    public ColorMap getColorMap();
    public Integer getParentFigure();
    public Boolean getVisible();
}
