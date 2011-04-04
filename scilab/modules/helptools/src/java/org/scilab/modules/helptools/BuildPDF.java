/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.helptools;

import java.io.File;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.OutputStream;
import java.io.BufferedOutputStream;
import java.io.FileOutputStream;

import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.Source;
import javax.xml.transform.stream.StreamSource;
import javax.xml.transform.Result;
import javax.xml.transform.sax.SAXResult;

import org.apache.fop.apps.FOPException;
import org.apache.fop.apps.FopFactory;
import org.apache.fop.apps.Fop;
import org.apache.fop.apps.MimeConstants;
import org.apache.fop.apps.FormattingResults;
import org.xml.sax.SAXException;

import org.scilab.forge.jlatexmath.fop.JLaTeXMathElementMapping;
import org.scilab.forge.jlatexmath.fop.JLaTeXMathXMLHandler;

import org.scilab.modules.commons.xml.ScilabTransformerFactory;

/**
 * This class manages the build of the PDF file
 */
public final class BuildPDF {

	/**
	 * Default constructor (must not be used)
	 */
	private BuildPDF() {
		throw new UnsupportedOperationException();
	}
	
	
    /**
     * After the saxon process, create the PDF thanks to fop 
     *
     * @param outputDirectory Where the files are available and 
     * @param language In which language (for the file name)
     * @return The result of the process
	 */
	public static String buildPDF(String outputDirectory, String language, String format) {

		String baseName = Helpers.getBaseName(language);
		/* the following '..' is used because we are in the current working
		   directory with all the tmp stuff in it */
		String fileName = outputDirectory + "/" + baseName; 
		if (format.equalsIgnoreCase("PS")) {
			 fileName+= ".ps";
		}else{
			 fileName+= ".pdf";
		}

		try {
                        FopFactory fopFactory = FopFactory.newInstance();
                        fopFactory.addElementMapping(new JLaTeXMathElementMapping());
                        fopFactory.getXMLHandlerRegistry().addXMLHandler(new JLaTeXMathXMLHandler());
                        fopFactory.setUserConfig(new File(System.getenv("SCI") + "/modules/helptools/etc/fopconf.xml"));
                        
                        // Step 3: Construct fop with desired output format
			OutputStream out = new BufferedOutputStream(new FileOutputStream(fileName));
			Fop fop;
			if (format.equalsIgnoreCase("PS")) {
				fop = fopFactory.newFop(MimeConstants.MIME_POSTSCRIPT, out);
			} else {
				fop = fopFactory.newFop(MimeConstants.MIME_PDF, out);
			}

			// Step 4: Setup JAXP using identity transformer
			String factoryName = ScilabTransformerFactory.useDefaultTransformerFactoryImpl();
			TransformerFactory factory = TransformerFactory.newInstance();
			Transformer transformer = factory.newTransformer(); // identity transformer
			// Step 5: Setup input and output for XSLT transformation 
			// Setup input stream
			Source src = new StreamSource(new File(Helpers.getTemporaryNameFo(outputDirectory)));

			// Resulting SAX events (the generated FO) must be piped through to FOP
			Result res = new SAXResult(fop.getDefaultHandler());
            
			// Step 6: Start XSLT transformation and FOP processing
			transformer.transform(src, res);
			ScilabTransformerFactory.restoreTransformerFactoryImpl(factoryName);

			FormattingResults foResults = fop.getResults();
			System.out.println("Generated " + foResults.getPageCount() + " pages in total.");

			//Clean-up
			out.close();
		} catch (FOPException e) {
			System.out.println(e.getLocalizedMessage());
		} catch (FileNotFoundException e) {
			System.out.println(e.getLocalizedMessage());
		} catch (TransformerConfigurationException e) {
			System.out.println(e.getLocalizedMessage());
		} catch (TransformerException e) {
			System.out.println(e.getLocalizedMessage());
		} catch (IOException e) {
			System.out.println(e.getLocalizedMessage());
		} catch (SAXException e) {
			System.out.println(e.getLocalizedMessage());
		}
			
		return fileName;
	}
}
