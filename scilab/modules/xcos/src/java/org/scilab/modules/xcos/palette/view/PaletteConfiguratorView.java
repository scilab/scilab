/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
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
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;

import javax.swing.BorderFactory;
import javax.swing.JCheckBox;
import javax.swing.JPanel;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import org.scilab.modules.xcos.palette.PaletteConfigurator;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement the default view for any palette configuration
 */
public class PaletteConfiguratorView extends JPanel {
	private static final Border BORDER = BorderFactory
		.createCompoundBorder(
				BorderFactory.createLineBorder(Color.DARK_GRAY),
				BorderFactory.createEmptyBorder(10, 10, 10, 10)
			);
	
	private final PaletteConfigurator controller;
	
	private JCheckBox enabling;
	private TitledBorder border;
	
	/**
	 * Default constructor
	 * @param controller the associated controller
	 */
	public PaletteConfiguratorView(PaletteConfigurator controller) {
		super();
		setBackground(Color.WHITE);
		this.controller = controller;
		
		initComponents();
	}

	/**
	 * Instantiate all the components
	 */
	private void initComponents() {
		enabling = new JCheckBox(XcosMessages.ACTIVATE_PAL_TITLE);
		enabling.setBackground(Color.WHITE);
		enabling.setSelected(true);
		enabling.addItemListener(new ItemListener() {
			public void itemStateChanged(ItemEvent e) {
				controller.setEnable(e.getStateChange() == ItemEvent.SELECTED);
			}
		});
		add(enabling);
		
		border = new TitledBorder(BORDER, controller.getPalette().toString());
		setBorder(border);
	}
	
	/**
	 * Set the status of the checkbox
	 * @param enabled true if enabled, false otherwise
	 */
	public void setChecked(boolean enabled) {
		enabling.setSelected(enabled);
	}
}
