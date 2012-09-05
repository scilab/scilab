/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import java.util.ArrayList;

import org.scilab.modules.gui.editor.EntityPicker;
import org.scilab.modules.gui.datatip.DatatipCreate;
import org.scilab.modules.gui.datatip.DatatipHighlight;

/**
 * Select a datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipSelect {

    /**
    * Select a datatip when click on it
    *
    * @param figureUid Figure unique identifier.
    * @param coordIntX Integer with pixel mouse position x.
    * @param coordIntY Integer with pixel mouse position y.
    * @return String containing the datatip unique identifier
    */
    public static String selectDatatip(String figureUid, Integer coordIntX, Integer coordIntY) {

        Integer pos[] = {coordIntX, coordIntY};
        String datatip = (new EntityPicker()).pickDatatip(figureUid, pos);
        DatatipHighlight.highlightSelected(datatip);
        return datatip;
    }
}
