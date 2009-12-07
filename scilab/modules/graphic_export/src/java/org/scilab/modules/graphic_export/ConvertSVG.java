/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Calixte Denizet
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
package org.scilab.modules.graphic_export;

import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.File;

import org.apache.batik.transcoder.TranscoderOutput;
import org.apache.batik.transcoder.TranscoderInput;
import org.apache.fop.svg.PDFTranscoder;
import org.apache.fop.render.ps.PSTranscoder;
import org.apache.fop.render.ps.EPSTranscoder;
import org.apache.fop.svg.AbstractFOPTranscoder;
import org.apache.avalon.framework.configuration.DefaultConfiguration;

/**
 * Static class to convert a SVG file into a vectorial file using FOP and Batik
 * @author Calixte Denizet
 *
 */
public class ConvertSVG {
    
/**
 * Convert a SVG to 
 *
 * @param in the input file
 * @param out the PDF transformed file
 * @param fileType The type (can be PDF_EXPORT, PS_EXPORT or EPS_EXPORT)
 */
    public static void SVGTo(String in, String out, int fileType) {
	if (GL2PSRenderer.checkWritePermission(new File(out)) == ExportRenderer.SUCCESS) {
	    AbstractFOPTranscoder trans = null;
	    switch (fileType) {
			case ExportRenderer.PDF_EXPORT :
				trans = new PDFTranscoder();
				break;
			case ExportRenderer.PS_EXPORT :
				trans = new PSTranscoder();
				break;
			case ExportRenderer.EPS_EXPORT :
				trans = new EPSTranscoder();
				break;
	    }

	    try {
			TranscoderInput input = new TranscoderInput(new FileInputStream(in));
			FileOutputStream os = new FileOutputStream(out);
			TranscoderOutput output = new TranscoderOutput(os);
			trans.transcode(input, output);
			os.flush();
			os.close();
	    } catch (Exception e) {
			System.err.println("Error while converting from SVG: "+e.getLocalizedMessage());
			ExportRenderer.setErrorNumber(ExportRenderer.INVALID_FILE);
	    }
	} else {
	    ExportRenderer.setErrorNumber(ExportRenderer.INVALID_FILE);
	}
    }
}