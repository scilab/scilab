package org.scilab.modules.xpad.style;

import javax.swing.undo.CompoundEdit;

public class UpdateManager {
	private static HistoryManager historyManager = new HistoryManager(); 

	private volatile boolean updater    = true;
	private volatile boolean shouldMergeEdits;
	
	private CompoundEdit compoundEdit;
	
	public CompoundEdit getCompoundEdit() {
		return compoundEdit;
	}

	public boolean isUpdater() {
		return updater;
	}

	public void setUpdater(boolean updaterDisabled) {
		this.updater = updaterDisabled;
	}
	
	public void setShouldMergeEdits(boolean b) {
		if (shouldMergeEdits) {
			if (!b) { // ending compound editing
				compoundEdit.end();
				historyManager.getUndo().addEdit(compoundEdit);
				compoundEdit = null;
			}
		} else {
			if (b) { // starting compound editing
				compoundEdit = new CompoundEdit();
			}
		}
		shouldMergeEdits = b;
	}
	
	public boolean getShouldMergeEdits() {
		return shouldMergeEdits;
	}


}
