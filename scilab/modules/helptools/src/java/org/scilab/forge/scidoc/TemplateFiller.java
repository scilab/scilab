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

package org.scilab.forge.scidoc;

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