/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.bridge.filechooser;

import java.io.File;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.border.TitledBorder;

import org.scilab.modules.action_binding.InterpreterManagement;

/**
 * This is the son of the usual Scilab file chooser,
 * it have been customized for the graphic export
 * @author Sylvestre Koumar
 *
 */
public class SwingScilabExportFileChooser extends SwingScilabFileChooser {
	
	private final String bmpDesc = "Windows BMP image";
	private final String gifDesc = "GIF image";
	private final String jpgDesc = "JPEG image";
	private final String pngDesc = "PNG image";
	private final String ppmDesc = "PPM image";
	private final String emfDesc = "Enhanced Metafile image";
	private final String epsDesc = "Encapsulated PostScript image";
	private final String figDesc = "FIG image";
	private final String pdfDesc = "PDF image";
	private final String svgDesc = "SVG image";
	private final String byExtensionDesc = "By extension";
	
	private final String bmp = "bmp";
	private final String gif = "gif";
	private final String jpg = "jpg";
	private final String png = "png";
	private final String ppm = "ppm";
	private final String emf = "emf";
	private final String eps = "eps";
	private final String fig = "fig";
	private final String pdf = "pdf";
	private final String svg = "svg";
	private final String byExtension = "*";
	
	private String exportName;	
	private String extensionSelected;
	private int figureId;
	
	//Format description & format name	
	private	String [][] data = {
			{bmpDesc, bmp},
			{gifDesc, gif},
			{jpgDesc, jpg},
			{pngDesc, png},
			{ppmDesc, ppm},
			{emfDesc, emf},
			{epsDesc, eps},
			{figDesc, fig},
			{pdfDesc, pdf},
			{svgDesc, svg},
			{byExtensionDesc, byExtension} // should always be at the last position
	};
	
	/**
	 * Default constructor
	 */
	public SwingScilabExportFileChooser(int figureId) {
		super();
		this.figureId = figureId;
		exportCustomFileChooser(figureId);
	}
	
	/**
	 * We customize the file chooser for the graphic export 
	 * by adding format selection
	 * @param figureId exported figure number
	 */
	public void exportCustomFileChooser(int figureId) {		

		super.setDialogTitle("Export");		
		super.setApproveButtonText("Export");
		final String fileName = "Untitled-export";
		File exportFile = new File(fileName);
		super.setSelectedFile(exportFile);
		super.setAcceptAllFileFilterUsed(false);		
		
		//image filter
		FileMasque[] fm = new FileMasque[data.length];
		
		this.figureId = figureId;
		
		for (int i = 0; i < data.length; i++) {
			fm[i] = new FileMasque(data[i][1].toString(), data[i][0].toString());
		}
		
		for (int i = 0; i < fm.length; i++) {
			super.addChoosableFileFilter(fm[i]);
		}				
				
		fm[fm.length-1].clearExtensions();		
			

        //Preview panel 
		JPanel panelPreview = new JPanel();
		
        //Title for preview panel
        TitledBorder titlePreview;
        titlePreview = BorderFactory.createTitledBorder(" Preview ");        
        panelPreview.setBorder(titlePreview);
        
        //add preview image
		panelPreview.add(new ImagePreview(this));		
		
		//Main panel contains extensionTable panel & preview panel
		JPanel accessoryPanel = new JPanel();
		
		//accessoryPanel.add(scrollPane);		
		accessoryPanel.add(panelPreview);
		accessoryPanel.setVisible(true);	
		super.setAccessory(accessoryPanel);
		
		
		JFrame frame = new JFrame();	
		frame.setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());
		int selection = super.showOpenDialog(frame);		
		
		if (selection == JFileChooser.APPROVE_OPTION) {		
			javax.swing.filechooser.FileFilter ft = super.getFileFilter();
			exportName = super.getSelectedFile().getAbsolutePath();
			
			//Test if there is a file with the same name
			File existFile = new File(exportName);
			if (existFile.exists()) {
				int actionDialog = JOptionPane.showConfirmDialog(this, "Replace existing file?", "File already exist", JOptionPane.YES_NO_OPTION);

				if(actionDialog == JOptionPane.YES_OPTION) {

				} else {
					return;
				}
			}
			
			
			extensionSelected = ft.getDescription();
			
			//get the extension from the description of the combobox
			String extensionCombo = extensionSelected.subSequence(extensionSelected.indexOf(".") + 1, extensionSelected.length() - 1).toString().toLowerCase();
			
			if (extensionCombo.equals(byExtension)) {				
				exportManager();	
				
			} else if(extensionCombo.equals(emf) || extensionCombo.equals(eps) || extensionCombo.equals(fig) 
					|| extensionCombo.equals(pdf) || extensionCombo.equals(svg)){		
				vectorialExport(extensionCombo);
				
			} else {				
				bitmapExport(extensionCombo);				
			}
		} else ; // no file chosen		
		
	}
	
	
	/**
	 * Return the file extension
	 * @param fileName Name of the file
	 * @return the extension
	 */
	public String getExtension(String fileName) {
		if(fileName != null) {
			int i = fileName.lastIndexOf('.');
			if(i > 0 && i < fileName.length() - 1) {
				return fileName.substring(i + 1).toLowerCase();
			};
		}
		return null;
	}
	
	/**
	 * Manage the export (bitmap/vectorial format) and export errors 
	 */
	public void exportManager() {
		
		//get the extension of the fileName chosen by the user (can be 'null')
		String userExtension = getExtension(exportName);			
		
		if (userExtension == null) {
			//fileName without extension + "by extension (.*)" selected
			JOptionPane.showMessageDialog(this, "Please specify a file format", "Error on export", JOptionPane.ERROR_MESSAGE);
			return;
		} else if (userExtension.equals(bmp)) {
			bitmapExport(userExtension);

		} else if (userExtension.equals(gif)) {			
			bitmapExport(userExtension);			
			
		} else if (userExtension.equals(jpg)) {
			bitmapExport(userExtension);
			
		} else if (userExtension.equals(png)) {
			bitmapExport(userExtension);
			
		} else if (userExtension.equals(ppm)) {
			bitmapExport(userExtension);
		
		} else if (userExtension.equals(emf)) {
			vectorialExport(userExtension);
			
		} else if (userExtension.equals(eps)) {
			vectorialExport(userExtension);
				
		} else if (userExtension.equals(fig)) {
			vectorialExport(userExtension);
		
		} else if (userExtension.equals(pdf)) {
			vectorialExport(userExtension);
		
		} else if (userExtension.equals(svg)) {
			vectorialExport(userExtension);
			
		} else {
			//fileName with a wrong extension + "by extension (.*)" selected
			JOptionPane.showMessageDialog(this, "Unrecognized extension '" + userExtension + "'.\n Please specify a valid file format.", "Error on export", JOptionPane.ERROR_MESSAGE);
			return;
		}		
	}
	
	/**
	 * Export an bitmap file
	 * @param userExtension extension caught by the user
	 */
	public void bitmapExport(String userExtension) {
		ExportData exportData;
		String exportcmd;
		
		exportData = new ExportData(figureId, exportName, userExtension, null);
		
		//the export instruction for the selected format
		exportcmd = "xs2" + exportData.getExportExtension() 
		+ "(" + figureId + ", '" + exportData.getExportName() + "');";
		InterpreterManagement.putCommandInScilabQueue(exportcmd);		
	}
	
	/**
	 * Export an bitmap file
	 * @param userExtension extension caught by the user
	 */
	public void vectorialExport(String userExtension) {
		ExportData exportData;
		ExportOptionWindow exportOptionWindow;
		
		exportData = new ExportData(figureId, exportName, userExtension, null);
        exportOptionWindow = new ExportOptionWindow(exportData);		
	}

}
