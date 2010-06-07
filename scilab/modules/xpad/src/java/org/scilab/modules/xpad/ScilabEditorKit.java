/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad;

import java.io.Reader;
import java.io.IOException;

import javax.swing.text.DefaultEditorKit;
import javax.swing.text.Document;
import javax.swing.text.ViewFactory;
import javax.swing.text.BadLocationException;

/**
 * The class ScilabEditorKit provides the minimal things needed to render
 * a Scilab's document.
 * @author Calixte DENIZET
 */
public class ScilabEditorKit extends DefaultEditorKit {

    /**
     * The mimetype for a scilab code
     */
    public static final String MIMETYPE = "text/scilab";
    
    private static final int BUFFERCAPACITY = 8192;
    private final char[] buffer = new char[BUFFERCAPACITY];	

    private ScilabContext preferences;

    /**
     * Default constructor
     */
    public ScilabEditorKit() {
    	super();
    }

    /**
     * @return the mime type
     */
    public String getContentType() {
	return MIMETYPE;
    }

    /**
     * @return a cloned ScilabEditorKit
     */
    public Object clone() {
	ScilabEditorKit kit = new ScilabEditorKit();
	return kit;
    }

    /**
     * @return a new ScilabDocument
     */
    public Document createDefaultDocument() {
    	return new ScilabDocument();
    }
    
    /**
     * @return the context associated with the ScilabDocument
     */
    public ScilabContext getStylePreferences() {
	if (preferences == null) {
	    preferences = new ScilabContext();
	}
	return preferences;
    }
    
    /**
     * Set the context to render the document
     * @param prefs the context to use
     */
    public void setStylePreferences(ScilabContext prefs) {
	preferences = prefs;
    }

    /**
     * @return the ViewFactory used to create a View to render a ScilabDocument
     */
    public ViewFactory getViewFactory() {
	return getStylePreferences();
    }

    /**
     * The read method is used to read the Reader and to write its contents
     * in the document at position pos
     * @param in the Reader to read
     * @param doc the Document where to write
     * @param pos position where to write
     * @throws IOException if a problem is encountered in reading the stream
     * @throws BadLocationException if the pos is invalid
     */
    public void read(Reader in, Document doc, int pos) throws IOException, BadLocationException {	
	int nch;
	int st;
	int i;
	int prev;
	int offs = 0;
	boolean win = false;
	boolean mac = false;
	StringBuffer sbuf = new StringBuffer(buffer.length);
	while ((nch = in.read(buffer, 0, buffer.length)) != -1) {
	    prev = 0;
	    for (i = 0; i < nch - 1; i++) {
		if (buffer[i] == '\r') {
		    buffer[i] = '\n';
		    sbuf.append(buffer, prev, i - prev + 1);
		    if (buffer[i + 1] == '\n') {
			i++;
			if (!win && !mac) {
			    ((ScilabDocument) doc).setEOL(ScilabDocument.EOLWIN);
			    win = true;
			}
		    } else {
			if (!win && !mac) {
			    ((ScilabDocument) doc).setEOL(ScilabDocument.EOLMAC);
			    mac = true;
			}
		    }
			
		    prev = i + 1;
		}
	    }
	    
	    if (i == nch - 1) {
		if (buffer[i] == '\r') {
		    if (!win && !mac) {
			((ScilabDocument) doc).setEOL(ScilabDocument.EOLMAC);
		    }
		    buffer[i] = '\n';
		}
		sbuf.append(buffer, prev, i - prev + 1);
	    }
	}
	if (!win && !mac) {
	    ((ScilabDocument) doc).setEOL(ScilabDocument.EOLUNIX);
	}
	doc.insertString(pos, sbuf.toString(), null);
    }
}
