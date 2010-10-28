/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.forge.scidoc.image;

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