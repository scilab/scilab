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
