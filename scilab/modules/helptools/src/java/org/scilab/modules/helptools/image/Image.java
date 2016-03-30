/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.helptools.image;

import javax.swing.Icon;

public class Image {

    public Icon icon;

    public int width;

    public int height;

    public int ascent;

    public int descent;

    public Image(Icon icon, int width, int height, int ascent, int descent) {
        this.icon = icon;
        this.width = width;
        this.height = height;
        this.ascent = ascent;
        this.descent = descent;
    }
}