package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.xmlloader.XmlLoader;
import org.xml.sax.SAXException;

public class CallXmlLoader {

    public static int Load(String xmlFile) {
        XmlLoader loader = new XmlLoader(xmlFile);
        try {
            return loader.parse();
        } catch (SAXException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        return 0;
    }
}
