/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
import javax.swing.text.BadLocationException;
import javax.swing.text.Segment;

public class ScilabDocumentReader extends Reader {
    
    private int end;
    private int pos;
    private ScilabDocument doc;
    
    public ScilabDocumentReader(ScilabDocument doc, int start, int end) {
	this.doc = doc;
	this.end = Math.min(doc.getLength(), end);
	this.pos = start;
    }
    
    public int read(char[] cbuf, int pos, int len) throws IOException {
	if (doc == null) {
	    throw new IOException("Reader closed");
	}
	
	if (this.pos < end) {
	    if (len > end - this.pos) {
		len = end - this.pos;
	    }
	    Segment segment = new Segment();
	    try {
		doc.getText(this.pos, len, segment);
		System.arraycopy(segment.array, segment.offset, cbuf, pos, len);
		this.pos += len;
		return len;
	    } catch (BadLocationException e) {
		throw new IOException("Bad location");
	    }
	}
	return -1;
    }
    
    public void close() throws IOException {
	doc = null;
    }
}   