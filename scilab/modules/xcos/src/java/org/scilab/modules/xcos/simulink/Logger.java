package org.scilab.modules.xcos.simulink;

import java.io.*;

import org.scilab.modules.jvm.utils.ScilabConstants;

public class Logger {
	public static void toFile(String logs) throws IOException {
		// FIXME: just for xendo purposes
		final String homePath = ScilabConstants.SCIHOME.toString();
		PrintWriter outputStream = null;
		try {
			boolean append = true;
            outputStream = new PrintWriter(new FileWriter(new File(homePath + '/' + "simimplogs"), append));
            outputStream.println(logs);
        } finally {
            if (outputStream != null) {
                outputStream.close();
            }
        }
	}
}
