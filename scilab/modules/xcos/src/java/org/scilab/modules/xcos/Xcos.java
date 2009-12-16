/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos;

import java.io.File;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.XcosPaletteManager;
import org.scilab.modules.xcos.palette.actions.ViewPaletteBrowserAction;
import org.scilab.modules.xcos.utils.ConfigXcosManager;

public final class Xcos {

    /**
     * The current Xcos version and name
     */
    public static final String VERSION = "1.0";
    public static final String TRADENAME = "Xcos";
    
    private static Map<String, SuperBlock> openedSuperBlock = new HashMap<String, SuperBlock>();

    /* Static class */
    private Xcos() {
    }

    /** Palette creation */
    static {
	/* load scicos libraries (macros) */
	InterpreterManagement.requestScilabExec("loadScicosLibs();");
    }

    /**
     * @param args
     */
    public static void main(String[] args) {
	SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		xcos();
	    }
	});
    }

    public static void xcos() {
	SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		XcosPaletteManager.loadPalette();
		createEmptyDiagram();
		ViewPaletteBrowserAction.setPalettesVisible(true);
	    }
	});
    }

    public static void xcos(String fileName) {
	final String filename = fileName;
	SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		ConfigXcosManager.saveToRecentOpenedFiles(filename);
		if (XcosTab.focusOnExistingFile(filename) == false) {
		    XcosDiagram diagram = createEmptyDiagram();
		    diagram.openDiagramFromFile(filename);
		}
	    }
	});
    }

    public static XcosDiagram createEmptyDiagram() {
	XcosDiagram xcosDiagramm = createANotShownDiagram();
	XcosTab.showTabFromDiagram(xcosDiagramm);
	return xcosDiagramm;
    }

    public static XcosDiagram createANotShownDiagram() {
	XcosDiagram xcosDiagramm = new XcosDiagram();
	xcosDiagramm.installListeners();
	XcosTab.createTabFromDiagram(xcosDiagramm);
	return xcosDiagramm;
    }

    public static XcosDiagram createHiddenDiagram() {
	XcosDiagram xcosDiagramm = new XcosDiagram();
	xcosDiagramm.installListeners();
	return xcosDiagramm;
    }

    public static void closeSession() {
	List<XcosDiagram> diagrams = XcosTab.getAllDiagrams();

	while (diagrams.size() > 0) {
	    diagrams.get(0).closeDiagram();
	}
	ViewPaletteBrowserAction.setPalettesVisible(false);
    }

    public static Tab getPalettes() {
	return XcosPaletteManager.getPalettes();
    }

    public static List<XcosDiagram> getDiagrams() {
	return XcosTab.getAllDiagrams();
    }

    /**
     * Look in each diagram to find the block corresponding to the given uid and
     * display a warning message.
     * 
     * @param UID
     *            - A String as UID.
     * @param message
     *            - The message to display.
     */
    public static void warnCellByUID(String UID, String message) {
	// Try to find a block with given index (UID)
	List<XcosDiagram> allDiagrams = Xcos.getDiagrams();
	for (int i = 0; i < allDiagrams.size(); ++i) {
	    allDiagrams.get(i).warnCellByUID(UID, message);
	}
    }

    /**
     * This method is called when the user exits from Scilab
     */
    public static void closeXcosFromScilab() {

	SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		// call close on all diagrams
		while (XcosTab.getAllDiagrams().size() > 0) {
		    XcosTab.getAllDiagrams().get(0).closeDiagram(true);
		}
	    }
	});
    }

    /**
     * This function convert a Xcos diagram to Scilab variable
     */
    public static int xcosDiagramToHDF5(String xcosFile, String h5File,
	    boolean forceOverwrite) {
	final String file = xcosFile;
	final File temp = new File(h5File);
	final boolean overwrite = forceOverwrite;

	if (temp.exists()) {
	    if (overwrite == false) {
		return 1;
	    } else {
		temp.delete();
	    }
	}

	try {
	    SwingUtilities.invokeAndWait(new Runnable() {
		public void run() {
		    XcosDiagram diagram = createHiddenDiagram();
		    diagram.openDiagramFromFile(file);
		    diagram.dumpToHdf5File(temp.getAbsolutePath());
		    diagram.closeDiagram();
		}
	    });
	} catch (InterruptedException e) {
	    e.printStackTrace();
	} catch (InvocationTargetException e) {
	    e.printStackTrace();
	}
	return 0;
    }

    public static void xcosDiagramOpen(String uid, boolean showed) {
	final String UID = uid;
	final boolean show = showed;

	try {
	    SwingUtilities.invokeAndWait(new Runnable() {
		public void run() {
		    BasicBlock block = null;
		    List<XcosDiagram> allDiagrams = Xcos.getDiagrams();
		    for (XcosDiagram diagram : allDiagrams) {
			// exclude SuperBlock from parsing
			if (diagram instanceof SuperBlockDiagram) {
			    continue;
			}

			block = diagram.getChildById(UID);
			if (block != null) {
			    SuperBlock newSP = (SuperBlock) BasicBlock.createBlock("SUPER_f");
			    newSP.setRealParameters(block.getRealParameters());
			    newSP.setParentDiagram(block.getParentDiagram());
			    if (show == true) {
				if(newSP.createChildDiagram() == true) {
				    XcosTab.createTabFromDiagram(newSP.getChild());
				    XcosTab.showTabFromDiagram(newSP.getChild());
				    newSP.getChild().setReadOnly(true);
				}
			    }
			    openedSuperBlock.put(UID, newSP);
			    break;
			}
		    }
		}
	    });
	} catch (InterruptedException e) {
	    e.printStackTrace();
	} catch (InvocationTargetException e) {
	    e.printStackTrace();
	}
    }

    public static void xcosDiagramClose(String uid) {
	final String UID = uid;

	try {
	    SwingUtilities.invokeAndWait(new Runnable() {
		public void run() {
		    SuperBlock SP = openedSuperBlock.get(UID);
		    if (SP != null) {
			openedSuperBlock.remove(UID);
			SP.closeBlockSettings();
			SP = null;
		    }
		}
	    });
	} catch (InterruptedException e) {
	    e.printStackTrace();
	} catch (InvocationTargetException e) {
	    e.printStackTrace();
	}
    }
}
