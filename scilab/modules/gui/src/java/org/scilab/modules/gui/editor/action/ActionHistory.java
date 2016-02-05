/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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
