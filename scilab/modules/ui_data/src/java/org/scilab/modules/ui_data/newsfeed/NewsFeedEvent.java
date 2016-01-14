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

import java.util.EventObject;

/**
 * News feed event
 */
public class NewsFeedEvent extends EventObject {
    public static final int NEWS_CHANGED = 1;
    public static final int NEWSFEED_UPDATED = 2;
    public static final int NEWSFEED_ERROR = 3;

    private int eventType;

    public NewsFeedEvent(Object source, int eventType) {
        super(source);
        this.eventType = eventType;
    }

    public int getEventType() {
        return eventType;
    }
}
