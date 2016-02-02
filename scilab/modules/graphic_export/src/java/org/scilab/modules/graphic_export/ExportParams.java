/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
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
