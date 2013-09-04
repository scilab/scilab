/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import java.awt.Graphics2D;
import java.awt.RenderingHints;

public class ExportParams {

    /** Orientation of the exported figure */
    public static final int PORTRAIT = 0;
    public static final int LANDSCAPE = 1;

    public float compressionQuality = -1;
    public int orientation = PORTRAIT;
    public boolean antialiasing = false;

    public ExportParams() { }

    public ExportParams(float compressionQuality, int orientation, boolean antialiasing) {
        this.compressionQuality = compressionQuality;
        this.orientation = orientation;
        this.antialiasing = antialiasing;
    }

    public void setParamsOnGraphics(Graphics2D g2d) {
        if (antialiasing) {
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        } else {
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
        }
    }
}
