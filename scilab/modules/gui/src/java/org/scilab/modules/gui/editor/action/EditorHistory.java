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

import java.util.LinkedList;
import java.util.ArrayDeque;
import java.util.Iterator;

/**
* Implements the editor history, store all action done
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class EditorHistory implements ActionHistory {


    /* store the actions */
    LinkedList<Action> history;

    /* pointer for the action to undo/redo */
    Integer head;

    /* max size of the history */
    private final Integer MAX_HISTORY_SIZE = 100;

    public EditorHistory() {

        /** initialize the history list */
        history = new LinkedList<Action>();
    }

    /**
    * Add an action to list at the position where is the pointer
    * and remove all action that are over the last added
    *
    * @param action the action to add
    */
    public void addAction(Action action) {

        if (head == null) {
            head = 0;
        }
        while (head > 0) {
            /*same as removeFirst without throwing exception if the deque is empty*/
            Action a = history.pollFirst();
            a.dispose();
            --head;
        }

        /*history reached max capacity? remove oldest action*/
        if (history.size() == MAX_HISTORY_SIZE) {
            Action a = history.pollLast();
            a.dispose();
        }
        history.addFirst(action);
    }

    /**
    * Undo the action at head
    */
    public void undo() {

        Action action = history.get(head);
        if (action.exists()) {
            action.undo();
            ++head;
        } else {
            history.remove(head);
        }
    }

    /**
    * Redo the action at position 'head'
    */
    public void redo() {

        if (head > 0) {
            --head;
            Action action = history.get(head);

            if (action.exists()) {
                action.redo();
            } else {
                Action a = history.get(head);
                a.dispose();
                history.remove(head);
            }
        }
    }


    /**
    * Removes the last action in the queue
    */
    public void removeAction() {
        history.pollLast();
    }

    /**
    * Clean the list and dispose all objects involved
    */
    public void dispose() {

        Iterator i = history.iterator();
        while (i.hasNext()) {
            Action action = (Action)i.next();
            action.dispose();
        }
        history.clear();
    }

    /**
    * Checks if undo is enabled
    *
    * @return true/false if is enabled or not
    */
    public boolean isUndoEnabled() {

        if (history.size() > 0 && head < history.size() && head >= 0) {
            return true;
        }
        return false;
    }


    /**
    * Checks if redo is enabled
    *
    * @return true/false if is enabled or not
    */
    public boolean isRedoEnabled() {

        if (history.size() > 0 && head > 0 && head <= history.size()) {
            return true;
        }
        return false;
    }
}
