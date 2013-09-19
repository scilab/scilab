/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
