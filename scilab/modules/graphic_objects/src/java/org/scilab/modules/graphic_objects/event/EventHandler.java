/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
