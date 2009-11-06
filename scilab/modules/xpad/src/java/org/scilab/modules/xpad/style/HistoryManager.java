package org.scilab.modules.xpad.style;

import javax.swing.event.DocumentEvent;
import javax.swing.event.UndoableEditEvent;
import javax.swing.undo.UndoManager;

public class HistoryManager {

	private static UndoManager undo;
	
	public HistoryManager(){
			undo = new UndoManager();
				
	}
			
	public UndoManager getUndo(){
		return undo;
	}
	

}
