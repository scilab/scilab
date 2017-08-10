/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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


/* Copyright INRIA 2007 */

package org.scilab.modules.console;


import org.scilab.modules.history_manager.HistoryManagement;
import com.artenum.rosetta.interfaces.core.HistoryManager;

/**
 * Manage all history actions if not implemented in Scilab core
 * @author Vincent COUVERT
 */
public class SciHistoryManager implements HistoryManager {

    private boolean isInHistory = false;

    /**
     * Constructor
     */
    public SciHistoryManager() {
        HistoryManagement.setSearchedTokenInScilabHistory("");
    }

    /**
     * Add a new line/block in history
     * @param newEntry data to add to history
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#addEntry(java.lang.String)
     */
    public void addEntry(String newEntry) {
        HistoryManagement.appendLineToScilabHistory(newEntry);
    }

    /**
     * Display Scilab whole history in Scilab console
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#display()
     */
    public void display() {
        HistoryManagement.displayScilabHistory();
    }

    /**
     * Gets a line/block in Scilab history by given its index
     * @param entryIndex the index of the entry
     * @return the entry
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#getEntry(int)
     */
    public String getEntry(int entryIndex) {
        return HistoryManagement.getNthLineInScilabHistory(entryIndex);
    }

    /**
     * Gets next line/block in Scilab history beginning with a given character set
     * @param beg character set
     * @return the entry
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#getNextEntry(int)
     */
    public String getNextEntry(String beg) {
        /* Ask Scilab history manager for a matching entry */
        if (HistoryManagement.getSearchedTokenInScilabHistory() != beg) {
            //HistoryManagement.resetSearchedTokenInScilabHistory();
            HistoryManagement.setSearchedTokenInScilabHistory(beg);
        }
        return HistoryManagement.getNextLineInScilabHistory();
    }

    /**
     * Gets previous line/block in Scilab history beginning with a given character set
     * @param beg character set
     * @return the entry
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#getPreviousEntry(int)
     */
    public String getPreviousEntry(String beg) {
        /* Ask Scilab history manager for a matching entry */
        if (HistoryManagement.getSearchedTokenInScilabHistory() != beg) {
            //HistoryManagement.resetSearchedTokenInScilabHistory();
            HistoryManagement.setSearchedTokenInScilabHistory(beg);
        }
        return HistoryManagement.getPreviousLineInScilabHistory();
    }

    /**
     * Loads history file form disk
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#load()
     */
    public void load() {
        /* Nothing to do in because Scilab core does the work */
    }

    /**
     * Reset current history
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#reset()
     */
    public void reset() {
        HistoryManagement.resetScilabHistory();
    }

    /**
     * Saves history to disk
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#save()
     */
    public void save() {
        /* Nothing to do in because Scilab core does the work */
    }

    /**
     * Sets the maximum number of entries the history has to save before erasing th oldest
     * @param numberOfEntries the maximum number of entries
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#setMaxEntryNumber(int)
     */
    public void setMaxEntryNumber(int numberOfEntries) {
        // TODO Auto-generated method stub

    }

    /**
     * Saves the line edited by the user before erasing it with history entries while browsing history
     * @param currentCommandLine the line edited
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#setTmpEntry(java.lang.String)
     */
    public void setTmpEntry(String currentCommandLine) {
        if (currentCommandLine != null && currentCommandLine.trim().equals("")) {
            HistoryManagement.resetSearchedTokenInScilabHistory();
            HistoryManagement.setSearchedTokenInScilabHistory("");
        } else {
            HistoryManagement.resetSearchedTokenInScilabHistory();
            HistoryManagement.setSearchedTokenInScilabHistory(currentCommandLine);
        }
    }

    /**
     * Reads the line edited by the user before erasing it with history entries while browsing history
     * @return the line
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#getTmpEntry()
     */
    public String getTmpEntry() {
        return HistoryManagement.getSearchedTokenInScilabHistory();
    }

    /**
     * Gets the flag saying that we are browsing history or not
     * @return true is we are browsing history and false else
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#isInHistory()
     */
    public boolean isInHistory() {
        /* Nothing to do in because Scilab core does the work */
        return isInHistory;
    }

    /**
     * Sets the flag saying that we are browsing history or not
     * @param status true is we begin history browsing and false else
     * @see com.artenum.rosetta.interfaces.core.HistoryManager#setInHistory(boolean)
     */
    public void setInHistory(boolean status) {
        /* Nothing to do in because Scilab core does the work */
        isInHistory = status;
    }
}
