/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

package org.scilab.modules.ui_data.newsfeed;

import org.scilab.modules.commons.ScilabConstants;

import java.io.File;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

/**
 * Loads news feed settings like URL or update interval time
 * Settings stored in newsfeed.xml (in directory ui_data/etc or SCIHOME)
 */
public class NewsFeedSettings {

    public static String getStringSetting(String settingName) throws Exception {
        File settingsFile = getSettingsFile();

        DocumentBuilder builder = DocumentBuilderFactory.newInstance().newDocumentBuilder();
        Document doc = builder.parse(settingsFile);
        NodeList newsfeedNodes = doc.getElementsByTagName("newsfeed");
        return getItemValue((Element) (newsfeedNodes.item(0)), settingName);
    }

    public static int getIntSetting(String settingName) throws Exception {
        String strSetting = getStringSetting(settingName);
        return Integer.parseInt(strSetting);
    }

    private static File getSettingsFile() {
        File settingsFile = new File(ScilabConstants.SCIHOME + "/newsfeed.xml");
        if (!settingsFile.exists()) {
            settingsFile = new File(ScilabConstants.SCI + "/modules/ui_data/etc/newsfeed.xml");
        }
        if (settingsFile.exists()) {
            return settingsFile;
        } else {
            return null;
        }
    }

    private static String getItemValue(Element item, String nodeName) {
        NodeList nodeList = item.getElementsByTagName(nodeName);
        if (nodeList.getLength() > 0) {
            return nodeList.item(0).getFirstChild().getNodeValue();
        } else {
            return null;
        }
    }

}
