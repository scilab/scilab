/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.gui.bridge.filechooser;

import java.util.Vector;

/**
 * This Class brought together all the information about a graphical export
 * (file name, extension, export options)
 * @author Sylvestre Koumar
 *
 */
public class ExportData {

    private Integer figureUID;
    private String exportName;
    private String exportExtension;
    private Vector<String> exportProperties;

    /**
     * Default constructor
     * @param figureId id of the exported figure
     * @param exportName name of the exported file
     * @param exportExtension extension of the exported file
     * @param exportProperties options of the export
     *
     */
    public ExportData(Integer figureUID, String exportName, String exportExtension, Vector<String> exportProperties) {
        this.exportName = exportName;
        this.exportExtension = exportExtension;
        this.exportProperties = exportProperties;
        this.figureUID = figureUID;
    }

    /**
     * Constructor
     */
    public ExportData() {

    }

    /**
     * figureUID getter
     * @return the figureUID
     */
    public Integer getFigureId() {
        return figureUID;
    }

    /**
     * figureUID setter
     * @param figureUID the figureUID to set
     */
    public void setFigureId(Integer figureUID) {
        this.figureUID = figureUID;
    }

    /**
     * exportName getter
     * @return the exportName
     */
    public String getExportName() {
        return exportName;
    }

    /**
     * exportName setter
     * @param exportName the exportName to set
     */
    public void setExportName(String exportName) {
        this.exportName = exportName;
    }

    /**
     * exportExtension getter
     * @return the exportExtension
     */
    public String getExportExtension() {
        return exportExtension;
    }

    /***
     * exportExtension setter
     * @param exportExtension the exportExtension to set
     */
    public void setExportExtension(String exportExtension) {
        this.exportExtension = exportExtension;
    }

    /**
     * exportProperties getter
     * @return the exportProperties
     */
    public Vector<String> getExportProperties() {
        return exportProperties;
    }

    /**
     * exportProperties setter
     * @param exportProperties the exportProperties to set
     */
    public void setExportProperties(Vector<String> exportProperties) {
        this.exportProperties = exportProperties;
    }

}
