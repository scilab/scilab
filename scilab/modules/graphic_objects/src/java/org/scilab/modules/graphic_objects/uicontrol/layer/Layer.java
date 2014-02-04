/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uicontrol.layer;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LAYER__;

import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;

/**
 * @author Vincent COUVERT
 */
public class Layer extends Uicontrol {

    /**
     * Constructor
     */
    public Layer() {
        super();
        setStyle(__GO_UI_LAYER__);
    }
}
