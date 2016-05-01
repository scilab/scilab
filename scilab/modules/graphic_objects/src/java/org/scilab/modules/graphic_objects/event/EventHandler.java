/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
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

package org.scilab.modules.graphic_objects.event;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

public class EventHandler {
    /** Event handler string */
    private String eventHandler = "";

    /** Specifies whether the event handler is enabled or not */
    private Boolean eventHandlerEnabled = false;

    /**
     * Default constructor
     */
    public EventHandler() {
        eventHandler = "";
        eventHandlerEnabled = false;
    }

    /**
     * Copy constructor
     * @param eventHandler the EventHandler to copy
     */
    public EventHandler(EventHandler eventHandler) {
        this.eventHandler = eventHandler.eventHandler;
        this.eventHandlerEnabled = eventHandler.eventHandlerEnabled;
    }

    public Boolean getEventHandlerEnabled() {
        return eventHandlerEnabled;
    }

    public UpdateStatus setEventHandlerEnabled(Boolean eventHandlerEnabled) {
        if (eventHandlerEnabled == this.eventHandlerEnabled) {
            return UpdateStatus.NoChange;
        }
        this.eventHandlerEnabled = eventHandlerEnabled;
        return UpdateStatus.Success;
    }

    public String getEventHandlerString() {
        return eventHandler;
    }

    public UpdateStatus setEventHandlerString(String eventHandler) {
        if (this.eventHandler.compareTo(eventHandler) == 0) {
            return UpdateStatus.NoChange;
        }
        this.eventHandler = eventHandler;
        return UpdateStatus.Success;
    }
}
