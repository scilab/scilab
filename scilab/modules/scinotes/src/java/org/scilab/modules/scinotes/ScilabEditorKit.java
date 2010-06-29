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

package org.scilab.modules.scinotes;

import java.nio.CharBuffer;
import java.nio.charset.Charset;
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
    private boolean plain;

    /**
     * Default constructor
     */
    public ScilabEditorKit() {
        super();
    }

    /**
     * Constructor
     * @param plain is true to use a plain view (no line-wrapping)
     */
    public ScilabEditorKit(boolean plain) {
        super();
        this.plain = plain;
    }

    /**
     * @return the mime type
     */
    public String getContentType() {
        return MIMETYPE;
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
            preferences = new ScilabContext(plain);
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
        int inc = 0;
        boolean win = false;
        boolean mac = false;
        boolean first = true;
        boolean binary = false;
        StringBuffer sbuf = new StringBuffer(buffer.length);
        while ((nch = in.read(buffer, 0, buffer.length)) != -1) {
            if (first) {
                /* We try to know if we have a binary file
                   The rule is : two \0 in the first 8ko : it's binary ! */
                CharBuffer cb = CharBuffer.wrap(buffer);
                byte[] bytes = Charset.defaultCharset().encode(cb).array();
                for (i = 0; i < nch; i++) {
                    if (bytes[i] == 0 ) {
                        inc++;
                        if (inc == 2) {
                            binary = true;
                            break;
                        }
                    }
                }
                first = false;
            }

            prev = 0;
            if (!binary) {
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
            } else {
                sbuf.append(buffer, 0, nch);
            }
        }
        if (!win && !mac) {
            ((ScilabDocument) doc).setEOL(ScilabDocument.EOLUNIX);
        }

        ((ScilabDocument) doc).setBinary(inc == 2);

        doc.insertString(pos, sbuf.toString(), null);
    }
}
