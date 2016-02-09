/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
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

package org.scilab.modules.ui_data.newsfeed;

/**
 * News media content (image essentially)
 */
public class NewsMediaContent {
    private String url;
    private String width;
    private String height;

    public NewsMediaContent(String url, String width, String height) {
        this.url = url;
        this.width = width;
        this.height = height;
    }

    String getURL() {
        return url;
    }

    String getWidth() {
        return width;
    }

    String getHeight() {
        return height;
    }
}
