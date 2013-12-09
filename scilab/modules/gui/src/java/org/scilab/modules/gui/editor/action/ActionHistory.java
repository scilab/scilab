/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.editor.action;

/**
* Implements the interface for user action history
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public interface ActionHistory {

    /**
    * Undo the last Action
    */
    void undo();

    /**
    * Redo last undo action
    */
    void redo();

    /**
    * Add an action to history
    *
    * @param action The action to add
    */
    void addAction(Action action);

    /**
    * remove the last action in the queue
    */
    void removeAction();

    /**
    * Clean the hitory
    */
    void dispose();
}
