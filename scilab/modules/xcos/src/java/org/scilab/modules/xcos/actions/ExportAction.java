/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.Color;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.Collection;
import java.util.Set;
import java.util.TreeSet;

import javax.imageio.ImageIO;
import javax.swing.JOptionPane;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxCellRenderer;
import com.mxgraph.util.mxUtils;

/**
 * Diagram export management
 */
public final class ExportAction extends DefaultAction {
	/** Name of the action */
	public static final String NAME = XcosMessages.EXPORT;
	/** Icon name of the action */
	public static final String SMALL_ICON = "";
	/** Mnemonic key of the action */
	public static final int MNEMONIC_KEY = KeyEvent.VK_E;
	/** Accelerator key for the action */
	public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
	
	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	public ExportAction(ScilabGraph scilabGraph) {
		super(scilabGraph);
	}

	/**
	 * Create export menu
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(scilabGraph, ExportAction.class);
	}

	/**
	 * @param e parameter
	 * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
	 */
	@Override
	public void actionPerformed(ActionEvent e) {

	    XcosDiagram graph = (XcosDiagram) getGraph(null);
		mxGraphComponent graphComponent = graph.getAsComponent();
		String filename = null;

		FileChooser fc = ScilabFileChooser.createFileChooser();
		
		// Adds a filter for each supported image format
		Collection<String> imageFormats = Arrays.asList(ImageIO.getWriterFormatNames());

		// The mask and mask description ordered collection
		Set<String> mask = new TreeSet<String>();
		Set<String> maskDesc = new TreeSet<String>();

		/* FIXME : why hardcoded ? */
		mask.add(".svg");
		mask.add(".html");
		mask.add(".vml");

		maskDesc.add("SVG");
		maskDesc.add("HTML");
		maskDesc.add("VML");
		
		// Add all distinct extensions
		for (String string : imageFormats) {
		    String ext = string.toLowerCase();
		    mask.add("." + ext);
		    maskDesc.add(ext.toUpperCase());
		}

		// Adds filter that accepts all supported image formats
		//fc.addChoosableFileFilter(new DefaultFileFilter.ImageFileFilter(mxResources.get("allImages")));
				
		((SwingScilabFileChooser) fc.getAsSimpleFileChooser()).addMask(mask.toArray(new String[mask.size()]) , 
			maskDesc.toArray(new String[maskDesc.size()]));
		fc.setTitle(XcosMessages.EXPORT);
		fc.displayAndWait();
		
		if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
			return;
		}

		filename = fc.getSelection()[0];

		if (new File(filename).exists()
			&& ScilabModalDialog.show(graph.getParentTab(), XcosMessages.OVERWRITE_EXISTING_FILE, XcosMessages.XCOS,
				IconType.QUESTION_ICON, ButtonType.YES_NO) != AnswerOption.YES_OPTION) {
			return;
		}

		try {
			String extension = filename.substring(filename.lastIndexOf('.') + 1);

			if (extension.equalsIgnoreCase("svg")) {
			    Document doc = mxCellRenderer.createSvgDocument(graph, null, 1, null, null);
			    if (doc != null) {
				mxUtils.writeFile(mxUtils.getXml(doc.getDocumentElement()), filename);
			    }
			} else if (extension.equalsIgnoreCase("vml")) {
			    Document doc = mxCellRenderer.createVmlDocument(graph, null, 1, null, null);
			    if (doc != null) {
				mxUtils.writeFile(mxUtils.getXml(doc.getDocumentElement()), filename);
			    }
			} else if (extension.equalsIgnoreCase("html")) {
			    Document doc = mxCellRenderer.createHtmlDocument(graph, null, 1, null, null);
			    if (doc != null) {
				mxUtils.writeFile(mxUtils.getXml(doc.getDocumentElement()), filename);
			    }
			} else 	{
				Color bg = null;

				if ((!extension.equalsIgnoreCase("gif") && !extension.equalsIgnoreCase("png"))
					|| ScilabModalDialog.show(graph.getParentTab(), XcosMessages.TRANSPARENT_BACKGROUND, XcosMessages.XCOS, 
						IconType.QUESTION_ICON, ButtonType.YES_NO) != AnswerOption.YES_OPTION) {
					bg = graphComponent.getBackground();
				}

				BufferedImage image = mxCellRenderer.createBufferedImage(
						graph, null, 1, bg, graphComponent.isAntiAlias(), null, graphComponent.getCanvas());

				if (image != null) {
					ImageIO.write(image, extension, new File(filename));
				} else  {
					JOptionPane.showMessageDialog(graphComponent, XcosMessages.NO_IMAGE_DATA);
				}
			}
		} catch (IOException e1) {
			e1.printStackTrace();
		}
	}
}

