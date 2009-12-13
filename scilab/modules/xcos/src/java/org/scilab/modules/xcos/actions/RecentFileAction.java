package org.scilab.modules.xcos.actions;

import java.io.File;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.ConfigXcosManager;


public class RecentFileAction extends DefaultAction {

	private File recentFile ;

	private RecentFileAction(ScilabGraph scilabGraph , File f) {
		super(f.getName(),scilabGraph);
		recentFile = f ;
	}
	
	public void doAction() {
		ConfigXcosManager.saveToRecentOpenedFiles(recentFile.getPath());
		if (getGraph(null) == null) { // Called from palettes 
			Xcos.xcos(recentFile.getPath());
		} else {
			((XcosDiagram) getGraph(null)).openDiagramFromFile(recentFile.getPath());
		}
	};

	public static MenuItem createMenu(ScilabGraph scilabGraph, File f) {
	    return createMenu(f.getName(), null, new RecentFileAction(scilabGraph, f), null);
	}
}
