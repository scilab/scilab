/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uicontrol.uiimage;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGE__;

import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;

/**
 * @author Vincent COUVERT
 */
public class UiImage extends Uicontrol {

    /**
     * Constructor
     */
    public UiImage() {
        super();
        setStyle(__GO_UI_IMAGE__);
        Double[] imageInfos = {1.0, 1.0, 0.0, 0.0, 0.0}; // {XScale, YScale, XShear, YShear, RotationAngle}
        setUiValue(imageInfos);
    }

}
