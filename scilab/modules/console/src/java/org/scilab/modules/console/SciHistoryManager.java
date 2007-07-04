
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import com.artenum.console.interfaces.core.HistoryManager;

/**
 * Manage all history actions if not implemented in Scilab core
 * @author Vincent COUVERT
 */
public class SciHistoryManager implements HistoryManager {
    
	private String tmpEntry;
    //private int maxNumberOfEntries; 
    private boolean inHistory;

	/**
	 * Constructor
	 */
	public SciHistoryManager() {
	}

	/**
	 * Add a new line/block in history
	 * @param newEntry data to add to history
	 * @see com.artenum.console.interfaces.core.HistoryManager#addEntry(java.lang.String)
	 */
	public void addEntry(String newEntry) {
		HistoryManagement.AddHistory(newEntry);
	}

	/**
	 * Display Scilab whole history in Scilab console
	 * @see com.artenum.console.interfaces.core.HistoryManager#display()
	 */
	public void display() {
		// TODO Auto-generated method stub
	}

	/**
	 * Gets a line/block in Scilab history by given its index
	 * @param entryIndex the index of the entry
	 * @return the entry
	 * @see com.artenum.console.interfaces.core.HistoryManager#getEntry(int)
	 */
	public String getEntry(int entryIndex) {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Gets next line/block in Scilab history beginning with a given character set
	 * @param beg character set
	 * @return the entry
	 * @see com.artenum.console.interfaces.core.HistoryManager#getNextEntry(int)
	 */
	public String getNextEntry(String beg) {
        /* Ask Scilab history manager for a matching entry */
		return HistoryManagement.getNextEntry(beg);
	}

	/**
	 * Gets previous line/block in Scilab history beginning with a given character set
	 * @param beg character set
	 * @return the entry
	 * @see com.artenum.console.interfaces.core.HistoryManager#getPreviousEntry(int)
	 */
	public String getPreviousEntry(String beg) {
        /* Ask Scilab history manager for a matching entry */
		return HistoryManagement.getPreviousEntry(beg);
	}

	/**
	 * Loads history file form disk
	 * @see com.artenum.console.interfaces.core.HistoryManager#load()
	 */
	public void load() {
		/* Nothing to do in because Scilab core does the work */
	}

	/**
	 * Reset current history
	 * @see com.artenum.console.interfaces.core.HistoryManager#reset()
	 */
	public void reset() {
		/* Nothing to do in because Scilab core does the work */
	}

	/**
	 * Saves history to disk
	 * @see com.artenum.console.interfaces.core.HistoryManager#save()
	 */
	public void save() {
		/* Nothing to do in because Scilab core does the work */
	}

	/**
	 * Sets the maximum number of entries the history has to save before erasing th oldest
	 * @param numberOfEntries the maximum number of entries
	 * @see com.artenum.console.interfaces.core.HistoryManager#setMaxEntryNumber(int)
	 */
	public void setMaxEntryNumber(int numberOfEntries) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Saves the line edited by the user before erasing it with history entries while browsing history
	 * @param currentCommandLine the line edited
	 * @see com.artenum.console.interfaces.core.HistoryManager#setTmpEntry(java.lang.String)
	 */
	public void setTmpEntry(String currentCommandLine) {
		if (currentCommandLine != null && currentCommandLine.trim().equals("")) {
			this.tmpEntry = "";
		} else {
			this.tmpEntry = currentCommandLine;
		}
	}

	/**
	 * Reads the line edited by the user before erasing it with history entries while browsing history
	 * @return the line
	 * @see com.artenum.console.interfaces.core.HistoryManager#getTmpEntry()
	 */
	public String getTmpEntry() {
		return this.tmpEntry;
	}

	/**
	 * Gets the flag saying that we are browsing history or not
	 * @return true is we are browsing history and false else
	 * @see com.artenum.console.interfaces.core.HistoryManager#isInHistory()
	 */
	public boolean isInHistory() {
        return this.inHistory;
}

	/**
	 * Sets the flag saying that we are browsing history or not
	 * @param status true is we begin history browsing and false else
	 * @see com.artenum.console.interfaces.core.HistoryManager#setInHistory(boolean)
	 */
	public void setInHistory(boolean status) {
		if (status) {
			HistoryManagement.setNewSearchInHistory(1);
			
		}
		this.inHistory = status;
	}

	/**
	 * Gets next line/block in Scilab history beginning with a given character set
	 * @param beg character set
	 * @return the entry
	 * @see com.artenum.console.interfaces.core.HistoryManager#getNextEntry(int)
	 */
	public String searchBackward(String beg) {
			return HistoryManagement.searchBackward(beg);
	}
}
