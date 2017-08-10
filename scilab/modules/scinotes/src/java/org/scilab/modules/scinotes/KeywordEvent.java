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

package org.scilab.modules.scinotes;

import java.util.EventObject;

import javax.swing.text.Document;
import javax.swing.text.BadLocationException;

/**
 * Used to handle an event generated on a keyword
 * @author Calixte DENIZET
 */
public class KeywordEvent extends EventObject {

    private static final long serialVersionUID = 4505744754595572108L;

    private int start;
    private int length;
    private int type;
    private EventObject event;

    /**
     * Constructor
     * @param source the Object where the event occurred
     * @param event the MouseEvent which generated this event
     * @param type the type of the keyword
     * @param start the position of the keyword in the doc
     * @param length the length of the keyword
     */
    public KeywordEvent(Object source, EventObject event, int type, int start, int length) {
        super(source);
        this.start = start;
        this.length = length;
        this.type = type;
        this.event = event;
    }

    /**
     * @return the position of the keyword in the doc
     */
    public int getStart() {
        return start;
    }

    /**
     * @return the length of the keyword
     */
    public int getLength() {
        return length;
    }

    /**
     * @return the type of the keyword
     */
    public int getType() {
        return type;
    }

    /**
     * @return the event which generated this event
     */
    public EventObject getEvent() {
        return event;
    }

    /**
     * {@inheritDoc}
     */
    public String toString() {
        ScilabEditorPane sep = (ScilabEditorPane) getSource();
        Document doc = sep.getDocument();
        try {
            return doc.getText(start, length) + " at position " + start + " with type " + type;
        } catch (BadLocationException e) {
            System.err.println(e);
        }
        return "";
    }
}
