/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
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
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.swing.SwingUtilities;

import org.scilab.modules.graph.utils.ScilabInterpreterManagement;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.XcosPaletteManager;
import org.scilab.modules.xcos.palette.actions.ViewPaletteBrowserAction;
import org.scilab.modules.xcos.utils.ConfigXcosManager;

/**
 * Xcos entry point class 
 */
public final class Xcos {

    /**
     * The current Xcos version and name
     */
    public static final String VERSION = "1.0";
    public static final String TRADENAME = "Xcos";
    
    private static Map<String, SuperBlock> openedSuperBlock = new HashMap<String, SuperBlock>();

    /** This class is a static singleton, thus it must not be instantiated */
    private Xcos() { }

    /**
     * Debug main function
     * @param args command line args (Not used)
     */
    public static void main(String[] args) {
    	/* load scicos libraries (macros) */
		ScilabInterpreterManagement.requestScilabExec("loadScicosLibs();");
    
	SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		xcos();
	    }
	});
    }

    /**
     * Entry point without filename
     */
    public static void xcos() {
    	/* load scicos libraries (macros) */
		ScilabInterpreterManagement.requestScilabExec("loadScicosLibs();");
    
	SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		XcosPaletteManager.loadPalette();
		createEmptyDiagram();
		ViewPaletteBrowserAction.setPalettesVisible(true);
	    }
	});
    }

    /**
     * Entry point with filename
     * @param fileName The filename
     */
    public static void xcos(String fileName) {
    	/* load scicos libraries (macros) */
		ScilabInterpreterManagement.requestScilabExec("loadScicosLibs();");
		
	final String filename = fileName;
	SwingUtilities.invokeLater(new Runnable() {
	    public void run() {
		ConfigXcosManager.saveToRecentOpenedFiles(filename);
		if (!XcosTab.focusOnExistingFile(filename)) {
		    XcosDiagram diagram = createEmptyDiagram();
		    diagram.openDiagramFromFile(filename);
		}
	    }
	});
    }

    /**
     * Create a diagram model with a visible window
     * @return The new empty model
     */
    public static XcosDiagram createEmptyDiagram() {
	XcosDiagram xcosDiagramm = createANotShownDiagram();
	XcosTab.showTabFromDiagram(xcosDiagramm);
	return xcosDiagramm;
    }

    /**
     * Create a diagram model
     * @return The new empty model
     */
    public static XcosDiagram createANotShownDiagram() {
	XcosDiagram xcosDiagramm = new XcosDiagram();
	xcosDiagramm.installListeners();
	XcosTab.createTabFromDiagram(xcosDiagramm);
	return xcosDiagramm;
    }

    /**
     * Create an xcos fake diagram
     * @return The diagram model
     */
    public static XcosDiagram createHiddenDiagram() {
	XcosDiagram xcosDiagramm = new XcosDiagram();
	xcosDiagramm.installListeners();
	return xcosDiagramm;
    }

    /**
     * Close the current xcos session
     */
    public static void closeSession() {
	List<XcosDiagram> diagrams = XcosTab.getAllDiagrams();
	
	/*
	 * Using an iterator because the collection is modified during the
	 * iteration.
	 */
	for (Iterator<XcosDiagram> iterator = diagrams.iterator(); iterator.hasNext();) {
		XcosDiagram xcosDiagram = iterator.next();
		
		/*
		 * We need to close children before closing main diagram
		 */
		xcosDiagram.closeChildren();
		xcosDiagram.closeDiagram();
	}

	ViewPaletteBrowserAction.setPalettesVisible(false);
    }

    /**
     * Get the session palette.
     * @return The Tab palette
     */
    public static Tab getPalettes() {
	return XcosPaletteManager.getPalettes();
    }

    /**
     * @return All the opened (hidden or shown) diagrams
     */
    public static List<XcosDiagram> getDiagrams() {
	return XcosTab.getAllDiagrams();
    }

    /**
     * Look in each diagram to find the block corresponding to the given uid and
     * display a warning message.
     * 
     * @param uid
     *            A String as UID.
     * @param message
     *            The message to display.
     */
    public static void warnCellByUID(String uid, String message) {
	// Try to find a block with given index (UID)
	List<XcosDiagram> allDiagrams = Xcos.getDiagrams();
	for (int i = 0; i < allDiagrams.size(); ++i) {
	    allDiagrams.get(i).warnCellByUID(uid, message);
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
     * @param xcosFile The xcos diagram file
     * @param h5File The target file
     * @param forceOverwrite Does the file will be overwritten ?
     * @return Not used (compatibility) 
     */
    public static int xcosDiagramToHDF5(String xcosFile, String h5File,
	    boolean forceOverwrite) {
	final String file = xcosFile;
	final File temp = new File(h5File);
	final boolean overwrite = forceOverwrite;

	if (temp.exists()) {
	    if (!overwrite) {
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

	/**
	 * Open a diagram by uid.
	 * 
	 * @param uid
	 *            UID diagram to open.
	 * @param showed
	 *            True if you want the diagram to be shown, false otherwise.
	 */
    public static void xcosDiagramOpen(String uid, boolean showed) {
	final String id = uid;
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

			block = diagram.getChildById(id);
			if (block != null) {
			    SuperBlock newSP = (SuperBlock) BlockFactory.createBlock("SUPER_f");
			    newSP.setRealParameters(block.getRealParameters());
			    newSP.setParentDiagram(block.getParentDiagram());
			    if (show) {
				if (newSP.createChildDiagram()) {
				    XcosTab.createTabFromDiagram(newSP.getChild());
				    XcosTab.showTabFromDiagram(newSP.getChild());
				    newSP.getChild().setReadOnly(true);
				}
			    }
			    openedSuperBlock.put(id, newSP);
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

    /**
     * Close a diagram by uid.
     * @param uid The diagram id
     */
    public static void xcosDiagramClose(String uid) {
	final String id = uid;

	try {
	    SwingUtilities.invokeAndWait(new Runnable() {
		public void run() {
		    SuperBlock sp = openedSuperBlock.get(id);
		    if (sp != null) {
			openedSuperBlock.remove(id);
			sp.closeBlockSettings();
			sp = null;
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
