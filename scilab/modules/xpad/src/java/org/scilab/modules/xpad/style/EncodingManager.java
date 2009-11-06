package org.scilab.modules.xpad.style;

import java.nio.charset.Charset;

public class EncodingManager {
	private String encoding = Charset.defaultCharset().toString();

	public String getEncoding() {
		return encoding;
	}
	
	public void setEncoding(String encoding) {
		this.encoding = encoding;
	}

}
