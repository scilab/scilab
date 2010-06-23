/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
