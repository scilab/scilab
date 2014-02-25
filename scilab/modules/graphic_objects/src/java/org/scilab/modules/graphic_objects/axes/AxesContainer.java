/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
