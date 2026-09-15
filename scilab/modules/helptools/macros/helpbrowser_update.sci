// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function helpbrowser_update(startup)
    // Internal called by add_help_chapter(), del_help_chapter() and atomsLoad()

  if or(getscilabmode()==["STD" "NW"])
      jimport org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser
      global %helps

      // Hand the chapter titles (%helps(:,2)) to the browser: helpbrowser()
      // only carries the paths. This is a static registry, so it also works
      // before the browser is created (toolboxes loaded at startup).
      // Columns are joined into newline-separated blobs to avoid ambiguous
      // Scilab->Java string-matrix marshalling.
      if size(%helps, 1) > 0 then
          sep = ascii(10)
          ScilabHelpBrowser.setChapterTitles(strcat(%helps(:,1), sep), strcat(%helps(:,2), sep))
      else
          ScilabHelpBrowser.setChapterTitles("", "")
      end

      hb = ScilabHelpBrowser.getHelpBrowserWithoutCreation()
      if ~isempty(hb)
          if isdef("startup", "l")
              // for scilab.start
              id = xmlGetValues("//Setting/Profile/HelpBrowser","index",SCIHOME+"/configuration.xml")
              // No way to retrieve as well the helpbrowser language, if it's
              // different from the session's one.
              doc(id)
          else
              // for atomsLoad and add_help_chapter: a new help chapter was
              // registered. Refresh the (already open) browser so its TOC
              // picks up the new chapter.
              id = hb.getCurrentID()
              if type(id) <> 10 then
                  // No page opened yet: getCurrentID() returned null ([])
                  id = ""
              end
              // helpbrowser() requires scalar strings for #2 and #3.
              helpbrowser(%helps(:,1), id, getlanguage(), %f)
          end
      end
      jremove hb ScilabHelpBrowser
  end

endfunction
