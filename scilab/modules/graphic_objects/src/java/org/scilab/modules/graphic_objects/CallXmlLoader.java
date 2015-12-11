package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.xmlloader.XMLDomLoader;
import org.scilab.modules.graphic_objects.xmlloader.XmlLoader;
import org.scilab.modules.graphic_objects.xmlloader.XmlSaver;
import org.xml.sax.SAXException;

public class CallXmlLoader {

    public static int Load(String filename) throws SAXException {
        XmlLoader loader = new XmlLoader(filename);
        return loader.parse();
    }

    public static int DomLoad(String filename) throws SAXException {
        XMLDomLoader loader = new XMLDomLoader(filename);
        return loader.parse();
    }

    public static String Save(int figure, String filename, boolean reverseChildren) {
        return XmlSaver.save(figure, filename, reverseChildren);
    }
}
