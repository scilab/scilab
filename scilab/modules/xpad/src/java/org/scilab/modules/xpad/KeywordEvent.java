package org.scilab.modules.xpad;

import java.util.EventObject;

public class KeywordEvent extends EventObject {

    private int start;
    private int length;
    private int type;
    
    public KeywordEvent(Object source, int type, int start, int length) {
	super(source);
	this.start = start;
	this.length = length;
	this.type = type;
    }
  
    public int getStart() {
	return start;
    }

    public int getLength() {
	return length;
    }

    public int getType() {
	return type;
    }
}