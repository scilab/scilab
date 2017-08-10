/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.helptools;

/**
 * Class to convert DocBook to JavaHelp
 * @author Calixte DENIZET
 */
public interface TemplateFiller {

    /**
     * Make a previous item for the current id
     * @param current id
     * @return the corresponding html
     */
    public String makePrevious(String id);

    /**
     * Make a previous item for the current id
     * @param current id
     * @return the corresponding html
     */
    public String makePath(String id);

    /**
     * Make a top item for the current id
     * @param current id
     * @return the corresponding html
     */
    public String makeTop(String id);

    /**
     * Make a next item for the current id
     * @param current id
     * @return the corresponding html
     */
    public String makeNext(String id);

    /**
     * Make a list of the tocs
     * @param current id
     * @return the corresponding html
     */
    public String makeTocList(String id);

    /**
     * Make the lastModified html
     * @param current id
     * @return the corresponding html
     */
    public String makeLastModified(String id);

    /**
     * Make the subtitle
     * @param current id
     * @return the corresponding html
     */
    public String makeSubtitle(String id);

    /**
     * Make the title
     * @param current id
     * @return the corresponding html
     */
    public String makeTitle(String id);

    /**
     * Make the generation date
     * @param current id
     * @return the corresponding html
     */
    public String makeGenerationDate(String id);

    /**
     * Make the version
     * @param current id
     * @return the corresponding html
     */
    public String makeVersion(String id);

    /**
     * Make the start tag
     * @param current id
     * @return the corresponding html
     */
    public String makeStart(String id);
}