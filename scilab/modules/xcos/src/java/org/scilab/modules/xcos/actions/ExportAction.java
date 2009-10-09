/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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
import java.awt.event.KeyEvent;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.HashSet;

import javax.imageio.ImageIO;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxCellRenderer;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxGraph;

/**
 * Diagram export management
 * @author Vincent COUVERT
 *
 */
public final class ExportAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/**
	 * Constructor
	 * @param scilabGraph associated Scilab Graph
	 */
	private ExportAction(ScilabGraph scilabGraph) {
		super(XcosMessages.EXPORT, scilabGraph);
	}

	/**
	 * Create export menu
	 * @param scilabGraph associated Scilab Graph
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.EXPORT, null, new ExportAction(scilabGraph),
				KeyStroke.getKeyStroke(KeyEvent.VK_E, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Action !
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {

		mxGraph graph = getGraph(null);
		mxGraphComponent graphComponent = getGraph(null).getAsComponent();
		String filename = null;

		FileChooser fc = ScilabFileChooser.createFileChooser();
		
		// Adds a filter for each supported image format
		Object[] imageFormats = ImageIO.getReaderFormatNames();

		// Finds all distinct extensions
		HashSet formats = new HashSet();

		for (int i = 0; i < imageFormats.length; i++) {
			String ext = imageFormats[i].toString().toLowerCase();
			formats.add(ext);
		}

		imageFormats = formats.toArray();

		String[] mask = new String[imageFormats.length + 3];
		mask[0] = ".svg";
		mask[1] = ".html";
		mask[2] = ".vml";
		String[] maskDesc = new String[imageFormats.length + 3];
		maskDesc[0] = "SVG";
		maskDesc[1] = "HTML";
		maskDesc[1] = "VML";
		
		for (int i = 0; i < imageFormats.length; i++) {
			String ext = imageFormats[i].toString();
			mask[3 + i] = "." + ext;
			maskDesc[3 + i] =  ext.toUpperCase();
		}

		// Adds filter that accepts all supported image formats
		//fc.addChoosableFileFilter(new DefaultFileFilter.ImageFileFilter(mxResources.get("allImages")));
		
		((SwingScilabFileChooser) fc.getAsSimpleFileChooser()).addMask(mask , maskDesc);
		fc.setTitle(XcosMessages.EXPORT);
		fc.displayAndWait();
		
		if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
			return;
		}

		filename = fc.getSelection()[0];

		if (new File(filename).exists()
				&& JOptionPane.showConfirmDialog(graphComponent,
						XcosMessages.OVERWRITE_EXISTING_FILE) != JOptionPane.YES_OPTION) {
			return;
		}

		try {
			String extension = filename.substring(filename.lastIndexOf('.') + 1);

			if (extension.equalsIgnoreCase("svg")) {
				mxUtils.writeFile(mxUtils.getXml(
						mxCellRenderer.createSvgDocument(graph, null, 1, null, null).getDocumentElement()), filename);
			} else if (extension.equalsIgnoreCase("vml")) {
				mxUtils.writeFile(mxUtils.getXml(
						mxCellRenderer.createVmlDocument(graph, null, 1, null, null).getDocumentElement()), filename);
			} else if (extension.equalsIgnoreCase("html")) {
				mxUtils.writeFile(mxUtils.getXml(
						mxCellRenderer.createHtmlDocument(graph, null, 1, null, null).getDocumentElement()), filename);
			} else 	{
				Color bg = null;

				if ((!extension.equalsIgnoreCase("gif") && !extension.equalsIgnoreCase("png"))
						|| JOptionPane.showConfirmDialog(graphComponent, 
								XcosMessages.TRANSPARENT_BACKGROUND) != JOptionPane.YES_OPTION) {
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

