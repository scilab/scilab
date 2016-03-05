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

import java.util.EventListener;

/**
 * The interface KeywordListener is useful to listen to a keyword event.
 * @author Calixte DENIZET
 */
public interface KeywordListener extends EventListener {

    /**
     * ONMOUSECLICKED
     */
    int ONMOUSECLICKED = 1;

    /**
     * ONMOUSEOVER
     */
    int ONMOUSEOVER = 2;

    /**
     * Called when a keyword is caught
     * @param e a KeywordEvent
     */
    void caughtKeyword(KeywordEvent e);

    /**
     * @return the type of the listener
     */
    int getType();
}
