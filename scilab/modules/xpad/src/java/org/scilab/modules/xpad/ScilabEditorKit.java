/*
 */
package org.scilab.modules.xpad;

import java.io.Reader;
import java.io.IOException;

import javax.swing.text.DefaultEditorKit;
import javax.swing.text.Document;
import javax.swing.text.ViewFactory;
import javax.swing.text.BadLocationException;

public class ScilabEditorKit extends DefaultEditorKit {

    ScilabContext preferences;

    private final char[] buffer = new char[8192];	

    public ScilabEditorKit() {
    	super();
    }

    public String getContentType() {
	return "text/scilab";
    }

    public Object clone() {
	ScilabEditorKit kit = new ScilabEditorKit();
	return kit;
    }

    public Document createDefaultDocument() {
    	Document doc = new ScilabDocument();
    	return doc;
    }
    
    public ScilabContext getStylePreferences() {
	if (preferences == null) {
	    preferences = new ScilabContext();
	}
	return preferences;
    }
    
    public void setStylePreferences(ScilabContext prefs) {
	preferences = prefs;
    }

    public final ViewFactory getViewFactory() {
	return getStylePreferences();
    }

    public void read(Reader in, Document doc, int pos) throws IOException, BadLocationException {	
	int nch, st, offs = 0;
	StringBuffer sbuf = new StringBuffer(buffer.length);
	while ((nch = in.read(buffer, 0, buffer.length)) != -1) {
	    sbuf.append(buffer, 0, nch);
	    offs += nch;
	}
	doc.insertString(0, sbuf.toString(), null);
    }
}