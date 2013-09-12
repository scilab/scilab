package org.scilab.modules.helptools;

import java.io.IOException;

import org.xml.sax.SAXException;

public interface Converter {

    /**
     * List all supported Converters backends for doc generation
     */
    public static enum Backend {
        JAVAHELP,
        HTML,
        WEB,
        CHM,
        JAR_ONLY
    }

    /**
     * Register all the converter related external xml handlers
     */
    void registerAllExternalXMLHandlers();

    /**
     * Convert the document
     */
    void convert() throws SAXException, IOException;

    /**
     * Install converted files and other data to the target directory
     */
    void install() throws IOException;
}
