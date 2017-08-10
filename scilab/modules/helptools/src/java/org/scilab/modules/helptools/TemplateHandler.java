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

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.BufferedReader;
import java.io.FileReader;
import java.nio.charset.Charset;

/**
 * Class to handle a template
 * @author Calixte DENIZET
 */
public class TemplateHandler {

    private String[] array;
    private TemplateFiller filler;
    private String language;

    public TemplateHandler(TemplateFiller filler, File f, String language) {
        this.filler = filler;
        this.language = language;
        parseFile(f);
    }

    public void generateFileFromTemplate(String fileName, String id, String contents) {
        try {
            OutputStream out = new FileOutputStream(fileName);
            OutputStreamWriter writer = new OutputStreamWriter(out, Charset.forName("UTF-8"));

            for (int i = 0; i < array.length; i += 2) {
                writer.append(array[i]);
                String str = "";
                if (i + 1 < array.length) {
                    if (array[i + 1].equals("content")) {
                        str = contents;
                    } else if (array[i + 1].equals("top")) {
                        str = filler.makeTop(id);
                    } else if (array[i + 1].equals("previous")) {
                        str = filler.makePrevious(id);
                    } else if (array[i + 1].equals("next")) {
                        str = filler.makeNext(id);
                    } else if (array[i + 1].equals("path")) {
                        str = filler.makePath(id);
                    } else if (array[i + 1].equals("title")) {
                        str = filler.makeTitle(id);
                    } else if (array[i + 1].equals("subtitle")) {
                        str = filler.makeSubtitle(id);
                    } else if (array[i + 1].equals("toclist")) {
                        str = filler.makeTocList(id);
                    } else if (array[i + 1].equals("lastmodified")) {
                        str = filler.makeLastModified(id);
                    } else if (array[i + 1].equals("generationdate")) {
                        str = filler.makeGenerationDate(id);
                    } else if (array[i + 1].equals("version")) {
                        str = filler.makeVersion(id);;
                    } else if (array[i + 1].equals("start")) {
                        str = filler.makeStart(id);
                    } else if (array[i + 1].startsWith("translate=")) {
                        String toTranslate = array[i + 1].substring("translate=".length());
                        str = TemplateLocalization.getLocalized(language, toTranslate);
                    }
                    writer.append(str);
                }
            }

            writer.flush();
            writer.close();
            out.flush();
            out.close();
        } catch (IOException e) {
            System.err.println(e);
        }
    }

    private void parseFile(File f) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader(f));
            String line = null;
            StringBuilder stringBuilder = new StringBuilder();
            while ((line = reader.readLine()) != null ) {
                stringBuilder.append(line);
                stringBuilder.append("\n");
            }
            reader.close();

            array = stringBuilder.toString().split("(<!--<)|(>-->)");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}