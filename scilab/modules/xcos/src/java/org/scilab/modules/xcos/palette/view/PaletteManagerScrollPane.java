/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import java.awt.Color;

import javax.swing.JPanel;
import javax.swing.JScrollPane;

import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * Implement the default scroll panel.
 * 
 * To easily get the associated panel, use {@link #getContent()}. To set it use
 * {@link #setContent(JPanel)}.
 */
public class PaletteManagerScrollPane extends JScrollPane {
	
	private JPanel content;
	
	/** Default constructor */
	public PaletteManagerScrollPane() {
		super();
		
		setBackground(Color.WHITE);
		getVerticalScrollBar().setBlockIncrement(
				XcosConstants.PALETTE_BLOCK_HEIGHT
						+ XcosConstants.PALETTE_VMARGIN);
		getVerticalScrollBar().setUnitIncrement(
				XcosConstants.PALETTE_BLOCK_HEIGHT
						+ XcosConstants.PALETTE_VMARGIN);

		getHorizontalScrollBar().setBlockIncrement(
				XcosConstants.PALETTE_BLOCK_WIDTH
						+ XcosConstants.PALETTE_HMARGIN);
		getHorizontalScrollBar().setUnitIncrement(
				XcosConstants.PALETTE_BLOCK_WIDTH
						+ XcosConstants.PALETTE_HMARGIN);
	}
	
	/**
	 * @return the content
	 */
	public JPanel getContent() {
		return content;
	}
	
	/**
	 * @param content the content to set
	 */
	public void setContent(JPanel content) {
		this.content = content;
		setViewportView(this.content);
	}
}
