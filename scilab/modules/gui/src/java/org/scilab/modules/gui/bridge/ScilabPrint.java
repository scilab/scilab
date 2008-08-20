/**
 * 
 */
package org.scilab.modules.gui.bridge;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.awt.print.PageFormat;
import java.awt.print.Paper;
import java.awt.print.Printable;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;

/**
 * This class allow us to print a figure using Java2D
 * @author Sylvestre Koumar
 *
 */
public class ScilabPrint implements Printable {

	private final int PRINT_MARGIN = 3; 
	
	private BufferedImage canvasDump;

	/**
	 * Default Constructor
	 * Calling print job to print the BufferedImage
	 * @param canvasDump BufferedImage
	 */
	public ScilabPrint(BufferedImage canvasDump) {
		this.canvasDump = canvasDump;

		PrinterJob pj = PrinterJob.getPrinterJob();
		PageFormat pf = pj.defaultPage();
		
		//setting the page format
		Paper paper = new Paper();
	    double margin = PRINT_MARGIN;
	    paper.setImageableArea(margin, margin, paper.getWidth() - margin * 2, paper.getHeight()
	        - margin * 2);
	    pf.setPaper(paper);
		
		pj.setPrintable(this, pf);
		if (pj.printDialog()) {
			try {
				pj.print(); 
			} catch (PrinterException pe) {
				System.out.println("Printing Error");
				System.err.println(pe.toString());
			}
		}

	}

	/**
	 * Printing by converting the buffered image to Graphics2D
	 * @param g Graphics
	 * @param pf PageFormat
	 * @param page int
	 * @return Printable.PAGE_EXISTS if the printing instruction exist
	 * @throws PrinterException indicate that an exceptional condition 
	 * has occurred in the print system
	 */
	public int print(Graphics g, PageFormat pf, int page)
	throws PrinterException {
		if (page >= 1) {
			return Printable.NO_SUCH_PAGE;
		}	
		
		AffineTransform at = AffineTransform.getTranslateInstance(0, 0);		
		((Graphics2D) g).drawRenderedImage(canvasDump, at);
		
		return Printable.PAGE_EXISTS;
	}

}
