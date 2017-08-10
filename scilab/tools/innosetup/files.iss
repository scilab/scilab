;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
;
;-------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;-------------------------------------------------------------------------------

#include BinariesSourcePath + "\tools\innosetup\base.iss"
#ifndef SCILAB_X64
#include BinariesSourcePath + "\tools\innosetup\atlas.iss"
#endif
#include BinariesSourcePath + "\tools\innosetup\MKL.iss"
#include BinariesSourcePath + "\tools\innosetup\FFTW_MKL.iss"
#include BinariesSourcePath + "\tools\innosetup\MKL_commons.iss"
#include BinariesSourcePath + "\tools\tools.iss"
#include BinariesSourcePath + "\contrib\toolbox_skeleton.iss"
#include BinariesSourcePath + "\modules\core\core.iss"
#include BinariesSourcePath + "\modules\helptools\helptools.iss"
#include BinariesSourcePath + "\modules\demo_tools\demo_tools.iss"
#include BinariesSourcePath + "\modules\fileio\fileio.iss"
#include BinariesSourcePath + "\modules\output_stream\output_stream.iss"
#include BinariesSourcePath + "\modules\string\string.iss"
#include BinariesSourcePath + "\modules\io\io.iss"
#include BinariesSourcePath + "\modules\integer\integer.iss"
#include BinariesSourcePath + "\modules\overloading\overloading.iss"
#include BinariesSourcePath + "\modules\elementary_functions\elementary_functions.iss"
#include BinariesSourcePath + "\modules\jvm\jvm.iss"
#include BinariesSourcePath + "\modules\commons\commons.iss"
#include BinariesSourcePath + "\modules\time\time.iss"
#include BinariesSourcePath + "\modules\localization\localization.iss"
#include BinariesSourcePath + "\modules\boolean\boolean.iss"
#include BinariesSourcePath + "\modules\data_structures\data_structures.iss"
#include BinariesSourcePath + "\modules\functions\functions.iss"
#include BinariesSourcePath + "\modules\windows_tools\windows_tools.iss"
#include BinariesSourcePath + "\modules\dynamic_link\dynamic_link.iss"
#include BinariesSourcePath + "\modules\gui\gui.iss"
#include BinariesSourcePath + "\modules\completion\completion.iss"
#include BinariesSourcePath + "\modules\history_manager\history_manager.iss"
#include BinariesSourcePath + "\modules\action_binding\action_binding.iss"
#include BinariesSourcePath + "\modules\console\console.iss"
#include BinariesSourcePath + "\modules\special_functions\special_functions.iss"
#include BinariesSourcePath + "\modules\linear_algebra\linear_algebra.iss"
#include BinariesSourcePath + "\modules\sparse\sparse.iss"
#include BinariesSourcePath + "\modules\graphics\graphics.iss"
#include BinariesSourcePath + "\modules\renderer\renderer.iss"
#include BinariesSourcePath + "\modules\graphic_export\graphic_export.iss"
#include BinariesSourcePath + "\modules\external_objects\external_objects.iss"
#include BinariesSourcePath + "\modules\external_objects_java\external_objects_java.iss"
#include BinariesSourcePath + "\modules\sound\sound.iss"
#include BinariesSourcePath + "\modules\tclsci\tclsci.iss"
#include BinariesSourcePath + "\modules\arnoldi\arnoldi.iss"
#include BinariesSourcePath + "\modules\cacsd\cacsd.iss"
#include BinariesSourcePath + "\modules\optimization\optimization.iss"
#include BinariesSourcePath + "\modules\polynomials\polynomials.iss"
#include BinariesSourcePath + "\modules\scicos\scicos.iss"
#include BinariesSourcePath + "\modules\scicos_blocks\scicos_blocks.iss"
#include BinariesSourcePath + "\modules\statistics\statistics.iss"
#include BinariesSourcePath + "\modules\signal_processing\signal_processing.iss"
#include BinariesSourcePath + "\modules\javasci\javasci.iss"
#include BinariesSourcePath + "\modules\mexlib\mexlib.iss"
#include BinariesSourcePath + "\modules\m2sci\m2sci.iss"
#include BinariesSourcePath + "\modules\randlib\randlib.iss"
#include BinariesSourcePath + "\modules\interpolation\interpolation.iss"
#include BinariesSourcePath + "\modules\umfpack\umfpack.iss"
#include BinariesSourcePath + "\modules\fftw\fftw.iss"
#include BinariesSourcePath + "\modules\spreadsheet\spreadsheet.iss"
#include BinariesSourcePath + "\modules\differential_equations\differential_equations.iss"
#include BinariesSourcePath + "\modules\compatibility_functions\compatibility_functions.iss"
#include BinariesSourcePath + "\modules\development_tools\development_tools.iss"
#include BinariesSourcePath + "\modules\genetic_algorithms\genetic_algorithms.iss"
#include BinariesSourcePath + "\modules\simulated_annealing\simulated_annealing.iss"
#include BinariesSourcePath + "\modules\parameters\parameters.iss"
#include BinariesSourcePath + "\modules\matio\matio.iss"
#include BinariesSourcePath + "\modules\call_scilab\call_scilab.iss"
#include BinariesSourcePath + "\modules\api_scilab\api_scilab.iss"
#include BinariesSourcePath + "\modules\atoms\atoms.iss"
#include BinariesSourcePath + "\modules\types\types.iss"
#include BinariesSourcePath + "\modules\hdf5\hdf5.iss"
#include BinariesSourcePath + "\modules\scinotes\scinotes.iss"
#include BinariesSourcePath + "\modules\xcos\xcos.iss"
#include BinariesSourcePath + "\modules\graph\graph.iss"
;#ifndef SCILAB_F2C
;#include BinariesSourcePath + "\modules\parallel\parallel.iss"
;#endif
#include BinariesSourcePath + "\modules\modules_manager\modules_manager.iss"
#include BinariesSourcePath + "\modules\ui_data\ui_data.iss"
#include BinariesSourcePath + "\modules\history_browser\history_browser.iss"
#include BinariesSourcePath + "\modules\threads\threads.iss"
#include BinariesSourcePath + "\modules\xml\xml.iss"
#include BinariesSourcePath + "\modules\preferences\preferences.iss"
#include BinariesSourcePath + "\modules\graphic_objects\graphic_objects.iss"
#include BinariesSourcePath + "\modules\ast\ast.iss"
#include BinariesSourcePath + "\modules\functions_manager\functions_manager.iss"
#include BinariesSourcePath + "\modules\mpi\mpi.iss"
#include BinariesSourcePath + "\modules\slint\slint.iss"
#include BinariesSourcePath + "\modules\coverage\coverage.iss"
;-------------------------------------------------------------------------------
