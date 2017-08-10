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

import java.util.Date;

/**
 * News data
 */
public class News {

    private String title;
    private Date date;
    private String description;
    private String content;
    private NewsMediaContent mediaContent;
    private String link;

    public News(String title, Date date, String description, String content, NewsMediaContent mediaContent, String link) {
        this.title = title;
        this.date = date;
        this.description = description;
        this.content = content;
        this.mediaContent = mediaContent;
        this.link = link;
    }

    String getTitle() {
        return title;
    }

    Date getDate() {
        return date;
    }

    String getDescription() {
        return description;
    }

    String getContent() {
        return content;
    }

    NewsMediaContent getMediaContent() {
        return mediaContent;
    }

    String getLink() {
        return link;
    }
}
