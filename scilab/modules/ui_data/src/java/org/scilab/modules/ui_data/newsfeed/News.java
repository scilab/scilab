/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
    private String link;

    public News(String title, Date date, String description, String link) {
        this.title = title;
        this.date = date;
        this.description = description;
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

    String getLink() {
        return link;
    }
}
