/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.helptools;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;

import org.xml.sax.SAXException;

import com.icl.saxon.StyleSheet; /* saxon */

/**
 * This classes intends to wrap Saxon features in a easy-to-use class.
 */
public class BuildDocObject extends StyleSheet {
	
	/**
	 * Windows version ?
	 */
	public static final boolean IS_WINDOWS = (File.pathSeparatorChar == ';');

	private static final String SCI = System.getenv("SCI");
	private static final String ERROR_WHILE_COPYING = "Error while copying ";
	private static final String CANNOT_COPY_CONVERT = "Cannot copy/convert '";
	private static final String COULD_NOT_FIND_STYLE_DOC = "Could not find the style document: ";
	private static final String TO_WITH_QUOTES = "' to '";
	private static final String TO = " to ";
	private static final String COLON_WITH_QUOTES = "': ";
	private static final String COLON = " : ";
	private static final String PDF_FORMAT = "PDF";
	private static final String POSTSCRIPT_FORMAT = "PS";
	private static final String JH_FORMAT = "JH";
	private static final String JAVAHELP_FORMAT = "JAVAHELP";
	private static final String USE_EXTENSIONS_1 = "use.extensions=1";
	private static final String GRAPHICSIZE_EXTENSION_0 = "graphicsize.extension=0";
	private static final String TOC_SECTION_DEPTH_3 = "toc.section.depth=3";
	private static final String SECTION_AUTOLABEL_1 = "section.autolabel=1";
	
	private String outputDirectory;
	private String format;
	private String language;
	private String docbookPath;
	private String styleDoc;
	private ArrayList<String> specificArgs = new ArrayList<String>();

    /**
     * Creator ... creates the BuildDocObject object
     * 
     * @throws FileNotFoundException if Docbook is not found 
	 */
	public BuildDocObject() throws FileNotFoundException {
		super();
		this.docbookPath = System.getenv("DOCBOOK_ROOT");
		
		if (IS_WINDOWS) {
			if (this.docbookPath == null) {
				this.docbookPath = SCI + "/thirdparty/docbook";
			}
		} else {
			if (this.docbookPath == null) {
				throw new FileNotFoundException("Could not find variable DOCBOOK_ROOT defining Docbook root directory");
			}
		}
	}

    /**
     * Set the path to the DocBook XSL application 
     * See: http://wiki.docbook.org/topic/DocBookXslStylesheets
     * @param docbookPath The absolute path to the base directory
	 */
	public void setDocbookPath(String docbookPath) {
		this.docbookPath = docbookPath;
	}

    /**
     * Set the directory where files must be exported
     * Note that directory is created 
	 * 
     * @param outputDirectory The path to the directory
     * @return If the directory exists
	 */
	public boolean setOutputDirectory(String outputDirectory) {
		File directory = new File(outputDirectory);
		if (!directory.isDirectory()) {
			if (!directory.mkdirs()) {
				return false;
			}
		}
		this.outputDirectory = outputDirectory;
		return true;
	}



    /**
     * Defines the language
     *
     * @param language the language (xx_XX ex: en_US, fr_FR)
	 */
	public void setWorkingLanguage(String language) {
		this.language = language;
	}

	
    /**
     * Defines the export format
     *
     * @param format the format (among the list CHM, HTML, PDF, JH, PS)
	 */
	public void setExportFormat(String format) {
		// Need to work with a String instead of a enum since it needs
		// to be called from C/C++ and GIWS doesn't manage this type.
		// Can be CHM, HTML, PDF, JavaHelp, Postscript
		if (format.equalsIgnoreCase(PDF_FORMAT) || format.equalsIgnoreCase(POSTSCRIPT_FORMAT)) {
			specificArgs.add(USE_EXTENSIONS_1);
			specificArgs.add(GRAPHICSIZE_EXTENSION_0);
			specificArgs.add("paper.type=A4");
			specificArgs.add("\"generate.toc=book toc,title,figure,table,example,equation part toc,title reference toc,title\"");
			specificArgs.add(TOC_SECTION_DEPTH_3);
			specificArgs.add(SECTION_AUTOLABEL_1);
			specificArgs.add("variablelist.as.blocks=1");
			specificArgs.add("shade.verbatim=1");
			this.styleDoc = docbookPath + "/fo/docbook.xsl";

		} 

		/* HTML Format */
		if (format.equalsIgnoreCase("HTML")) {
			specificArgs.add("use.id.as.filename=1");
			specificArgs.add("html.stylesheet=html.css");
			specificArgs.add(USE_EXTENSIONS_1);
			specificArgs.add(GRAPHICSIZE_EXTENSION_0);
			specificArgs.add(TOC_SECTION_DEPTH_3);
			specificArgs.add(SECTION_AUTOLABEL_1);
			this.styleDoc = docbookPath + "/html/chunk.xsl";

			/* Copy the css file for thr HTML pages */
			String cssFile=new String(SCI+"/modules/helptools/css/html.css");
			try {
				Helpers.copyFile(new File(cssFile), new File(outputDirectory+"/html.css"));
			} catch (java.io.FileNotFoundException e) {
				System.err.println(ERROR_WHILE_COPYING + cssFile + TO + outputDirectory + COLON + e.getMessage());			
			} catch (java.io.IOException e) {
				System.err.println(ERROR_WHILE_COPYING + cssFile + TO + outputDirectory + COLON + e.getMessage());			
			}
			
		}

		/* Java Help */
		if (format.equalsIgnoreCase(JH_FORMAT) || format.equalsIgnoreCase(JAVAHELP_FORMAT)) {
			// JavaHelp
			specificArgs.add(USE_EXTENSIONS_1);
			specificArgs.add(GRAPHICSIZE_EXTENSION_0);
			specificArgs.add("\"generate.toc= \"");
			this.styleDoc = docbookPath + "/javahelp/javahelp.xsl";
		}
		this.format = format;
	}


	/**
	 * Replace links by the contents of the XML files in the master
	 * @param masterXML name of the master file
	 * @param styleSheet CSS to be used
	 * @return the absolute path the the new master file
	 */
	private String preProcessMaster(String masterXML, String styleSheet) {

		String filename = (String) new File(masterXML).getName();
		/* Create the output file which will be created by copyconvert.run into the working directory  */
		File masterXMLTransformed = new File(this.outputDirectory 
				+ File.separator + filename.substring(0, filename.lastIndexOf(".")) + "-processed.xml");
		/* Where it will be stored */
		String out = this.outputDirectory + File.separator + (String) new File(styleSheet).getName();

		try {
			Helpers.copyFile(new File(styleSheet), new File(out));
		} catch (java.io.FileNotFoundException e) {
			System.err.println(ERROR_WHILE_COPYING + styleSheet + TO + out + COLON + e.getMessage());			
		} catch (java.io.IOException e) {
			System.err.println(ERROR_WHILE_COPYING + styleSheet + TO + out + COLON + e.getMessage());			
		}

        CopyConvert copyConvert = new CopyConvert();
        copyConvert.setVerbose(true);
        copyConvert.setPrintFormat(this.format);
		
        try {
            copyConvert.run(new File(masterXML), masterXMLTransformed);
        } catch (SAXException e) {
            System.err.println(CANNOT_COPY_CONVERT + masterXML + TO_WITH_QUOTES
					   + masterXMLTransformed + COLON_WITH_QUOTES + Helpers.reason(e));
            return null;
        } catch (IOException e) {
           System.err.println(CANNOT_COPY_CONVERT + masterXML + TO_WITH_QUOTES
        		   + masterXMLTransformed + COLON_WITH_QUOTES + Helpers.reason(e));
            return null;
        }
		return masterXMLTransformed.getAbsolutePath();
	}

    /**
     * Private method which manages the post processing
     * @return The path to the file/directory created.
     */
	private String postProcess() {
		if (this.format.equalsIgnoreCase(JH_FORMAT) || format.equalsIgnoreCase(JAVAHELP_FORMAT)) {
			return BuildJavaHelp.buildJavaHelp(this.outputDirectory, this.language);
		}
		if (format.equalsIgnoreCase(PDF_FORMAT) || format.equalsIgnoreCase(POSTSCRIPT_FORMAT)) {
			return BuildPDF.buildPDF(this.outputDirectory, this.language, format);
		}
		
		return this.outputDirectory;
	}


    /**
     * Launch the whole Saxon process 
     *
     * @param sourceDoc Path to the XML master document
     * @param styleSheet Path to the CSS stylesheet
	 * @return The path to the file/directory created.
	 * @throws FileNotFoundException Raises an exception if no file/dir found
	 */
	public String process(String sourceDoc, String styleSheet) throws FileNotFoundException {
		ArrayList<String> args = new ArrayList<String>();

		if (!new File(sourceDoc).isFile()) {
			throw new FileNotFoundException("Could not find master document: " + sourceDoc);
		}

		if (!new File(styleSheet).isFile()) {
			throw new FileNotFoundException("Could not find CSS stylesheet: " + styleSheet);
		}

		if (!new File(this.styleDoc).isFile()) {
			throw new FileNotFoundException(COULD_NOT_FIND_STYLE_DOC + this.styleDoc);
		}

		if (!new File(this.outputDirectory).isDirectory()) {
			throw new FileNotFoundException("Could not find directory: " + this.outputDirectory);
		}
		
		String sourceDocProcessed = this.preProcessMaster(sourceDoc, styleSheet);
		
		if (sourceDocProcessed == null) {
		    throw new FileNotFoundException("Unable to parse generated master file.");
		}
		
		if (format.equalsIgnoreCase(PDF_FORMAT) || format.equalsIgnoreCase(POSTSCRIPT_FORMAT)) {
			/* PDF & postscript take other args */
			args.add("-o");
			args.add(Helpers.getTemporaryNameFo(outputDirectory));
		}

		args.add(sourceDocProcessed);
		args.add(this.styleDoc);
		args.add("base.dir=" + this.outputDirectory);
		args.add("html.stylesheet=" + new File(styleSheet).getName());
		args.addAll(specificArgs);
		/**
		 * We are calling directly the method as we were using a command line
		 * program because it is much easier ... 
		 * However, this should be rewritted using the API 
		 */
		doMain(args.toArray(new String [args.size()]), new StyleSheet(), "java com.icl.saxon.StyleSheet");

		if (new File(sourceDocProcessed).isDirectory()) {
			/* Delete the master temp file to avoid to be shipped with the rest */
			new File(sourceDocProcessed).delete();
		}

		return this.postProcess();

	}
	
    /**
     * The case to see if it working 
     *
     * @param arg Useless arg
	 */
	public static void main(String[] arg) {
		try {
			BuildDocObject d = new BuildDocObject();
			d.setOutputDirectory("/tmp/");
			d.setExportFormat(PDF_FORMAT);
			d.setDocbookPath("/usr/share/xml/docbook/stylesheet/nwalsh/");
			d.process("/home/sylvestre/dev/scilab5/modules/helptools/master_en_US_help.xml",
					"/home/sylvestre/dev/scilab5/modules/helptools/css/javahelp.css");
		} catch (FileNotFoundException e) {
			System.err.println("Exception catched: " + e.getMessage());
		}
	}
}
