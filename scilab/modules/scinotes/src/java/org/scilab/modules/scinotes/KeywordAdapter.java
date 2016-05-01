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

/**
 * An adaptater for the interface KeywordListener
 * @author Calixte DENIZET
 */
public abstract class KeywordAdapter implements KeywordListener {

    private int type;

    /**
     * Constructor
     * @param type the type of listener (ONMOUSECLICKED or ONMOUSEOVER)
     */
    protected KeywordAdapter(int type) {
        this.type = type;
    }

    /**
     * @return the type of this listener
     */
    public int getType() {
        return type;
    }

    /**
     * Called when a keyword is caught
     * @param e a KeywordEvent
     */
    public abstract void caughtKeyword(KeywordEvent e);

    /**
     * Class to have a KeywordListener attached to a MouseClicked event
     */
    public abstract static class MouseClickedAdapter extends KeywordAdapter {

        /**
         * Constructor
         */
        public MouseClickedAdapter() {
            super(ONMOUSECLICKED);
        }
    }

    /**
     * Class to have a KeywordListener attached to a MouseOver event
     */
    public abstract static class MouseOverAdapter extends KeywordAdapter {

        /**
         * Constructor
         */
        public MouseOverAdapter() {
            super(ONMOUSEOVER);
        }
    }
}
