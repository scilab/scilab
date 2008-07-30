package org.scilab.modules.helptools;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileNotFoundException;
import java.util.ArrayList;

import com.icl.saxon.StyleSheet; /* saxon */

/**
 * This classes intends to wrap Saxon features in a easy-to-use class.
 */
public class BuildDocObject extends StyleSheet {
	private String outputDirectory;
	private String format;
	private String language;
	private String docbookPath;
	private String styleDoc;
	private ArrayList<String> specificArgs = new ArrayList<String>();

    /**
     * Creator ... creates the BuildDocObject object
	 */
	public BuildDocObject() {
		super();
		this.docbookPath = System.getenv("DOCBOOK_ROOT");
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
		if (format.equalsIgnoreCase("PDF")) {
			specificArgs.add("use.extensions=1");
			specificArgs.add("graphicsize.extension=0");
			specificArgs.add("paper.type=A4");
			specificArgs.add("\"generate.toc=book toc,title,figure,table,example,equation part toc,title reference toc,title\"");
			specificArgs.add("toc.section.depth=3");
			specificArgs.add("section.autolabel=1");
			specificArgs.add("variablelist.as.blocks=1");
			specificArgs.add("shade.verbatim=1");
		} 
		if (format.equalsIgnoreCase("PS")) {
		}
		if (format.equalsIgnoreCase("HTML")) {
			specificArgs.add("use.id.as.filename=1");
			specificArgs.add("html.stylesheet=html.css");
			specificArgs.add("use.extensions=1");
			specificArgs.add("graphicsize.extension=0");
			specificArgs.add("toc.section.depth=3");
			specificArgs.add("section.autolabel=1");
		}
		if (format.equalsIgnoreCase("JH") || format.equalsIgnoreCase("javaHelp")) {
			// JavaHelp
			specificArgs.add("use.extensions=1");
			specificArgs.add("graphicsize.extension=0");
			specificArgs.add("\"generate.toc= \"");
			this.styleDoc = docbookPath + "/javahelp/javahelp.xsl";
		}
		this.format = format;
	}


	private String preProcessMaster(String masterXML, String styleSheet) {

		String filename = (String) new File(masterXML).getName();
		/* Create the output file which will be created by copyconvert.run into the working directory  */
		File masterXMLTransformed = new File(this.outputDirectory + File.separator + filename.substring(0, filename.lastIndexOf(".")) + "-processed.xml");

		this.copyFile(new File(styleSheet), new File(this.outputDirectory + File.separator + (String) new File(styleSheet).getName()));
        CopyConvert copyConvert = new CopyConvert();
        copyConvert.setVerbose(true);
        copyConvert.setPrintFormat(this.format);
		
        try {
            copyConvert.run(new File(masterXML), masterXMLTransformed);
        } catch (Exception e) {
            System.err.println("Cannot copy/convert '" + masterXML + "' to '"
							   + masterXMLTransformed + "': " + Helpers.reason(e));
            System.exit(2);
        }
		return masterXMLTransformed.getAbsolutePath();
	}

    /**
     * Private method which manages the post processing
     *
     */
	private void postProcess() {
		if (this.format.equalsIgnoreCase("JH") || format.equalsIgnoreCase("javaHelp")) {
			BuildJavaHelp.buildJavaHelp(this.outputDirectory, this.language);
		}
	}


	private void copyFile(File in, File out) {
		try {
			FileInputStream fis = new FileInputStream(in);
			FileOutputStream fos = new FileOutputStream(out);
			byte[] buf = new byte[1024];
			int i = 0;
			while ((i = fis.read(buf)) != -1) {
				fos.write(buf, 0, i);
			}
			fis.close();
			fos.close();
		} catch(java.io.FileNotFoundException e) {
			System.err.println("Error while copying " + in + " to " + out + " : " + e.getMessage());			
		} catch (java.io.IOException e) {
			System.err.println("Error while copying " + in + " to " + out + " : " + e.getMessage());			
		}
	}

    /**
     * Launch the whole Saxon process 
     *
     * @param sourceDoc Path to the XML master document
     * @param styleSheet Path to the CSS stylesheet
	 * @throws FileNotFoundException Raises an exception if no file/dir found
	 */
	public void process(String sourceDoc, String styleSheet) throws FileNotFoundException {
		ArrayList<String> args = new ArrayList<String>();

		if (!new File(sourceDoc).isFile()) {
			throw new FileNotFoundException("Could not find master document: " + sourceDoc);
		}

		if (!new File(styleSheet).isFile()) {
			throw new FileNotFoundException("Could not find CSS stylesheet: " + styleSheet);
		}

		if (!new File(this.styleDoc).isFile()) {
			throw new FileNotFoundException("Could not find the style document: " + this.styleDoc);
		}

		if (!new File(this.outputDirectory).isDirectory()) {
			throw new FileNotFoundException("Could not find directory: " + this.outputDirectory);
		}
		String sourceDocProcessed = this.preProcessMaster(sourceDoc, styleSheet);

		args.add(sourceDocProcessed);
		args.add(this.styleDoc);
		args.add("base.dir=" + this.outputDirectory);
		args.add("html.stylesheet=" + styleSheet);
		args.addAll(specificArgs);

		//		doMain(args.toArray(new String [args.size()]), new StyleSheet(), "java com.icl.saxon.StyleSheet");

		/* Delete the master temp file to avoid to be shipped with the rest */
		new File(sourceDocProcessed).delete();

		this.postProcess();

	}
	
    /**
     * The case to see if it working 
     *
     * @param arg Useless arg
	 */
	public static void main(String[] arg) {
		BuildDocObject d = new BuildDocObject();
		d.setOutputDirectory("/tmp/");
		d.setExportFormat("JH");
		d.setDocbookPath("/usr/share/xml/docbook/stylesheet/nwalsh/");
		try {
			d.process("/home/sylvestre/dev/scilab5/modules/helptools/master_en_US_help.xml", "/home/sylvestre/dev/scilab5/modules/helptools/css/javahelp.css");
		} catch (FileNotFoundException e) {
			System.err.println("Exception catched: " + e.getMessage());
		}
	}
}
