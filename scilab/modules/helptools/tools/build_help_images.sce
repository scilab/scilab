// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// CI / packaging driver around generate_help_images.sce:
// - checks that the versioned pre-generated help images are up to date
//   with the help sources (a regeneration here means a developer changed
//   an example without committing the regenerated images);
// - packs the images and their manifest into help_images.zip, shipped as
//   a single file by the installer (helptools.iss).
//
// Exit status: 0 ok, 1 generation failure/crash, 2 images were out of date.

GEN_ZIP = SCI + "/modules/helptools/help_images.zip";
exec(SCI + "/modules/helptools/tools/generate_help_images.sce");

if nFail + nCrash > 0 then
    mprintf("build_help_images: %d snippet(s) failed or crashed, see log above\n", nFail + nCrash);
    exit(1);
end
if nGen > 0 then
    mprintf("build_help_images: %d image(s) were out of date and have been regenerated.\n", nGen);
    mprintf("Run modules/helptools/tools/generate_help_images.sce and commit the images.\n");
    exit(2);
end
exit(0);
