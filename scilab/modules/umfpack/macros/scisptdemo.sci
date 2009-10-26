//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project 
//   <bruno.pincon@iecn.u-nancy.fr>
// 
// This set of scilab 's macros provide a few sparse utilities.
// 
// This software is governed by the CeCILL license under French law and
// abiding by the rules of distribution of free software.  You can  use, 
// modify and/ or redistribute the software under the terms of the CeCILL
// license as circulated by CEA, CNRS and INRIA at the following URL
// "http://www.cecill.info". 
// 
// As a counterpart to the access to the source code and  rights to copy,
// modify and redistribute granted by the license, users are provided only
// with a limited warranty  and the software's author,  the holder of the
// economic rights,  and the successive licensors  have only  limited
// liability. 
// 
// In this respect, the user's attention is drawn to the risks associated
// with loading,  using,  modifying and/or developing or reproducing the
// software by the user in light of its specific status of free software,
// that may mean  that it is complicated to manipulate,  and  that  also
// therefore means  that it is reserved for developers  and  experienced
// professionals having in-depth computer knowledge. Users are therefore
// encouraged to load and test the software's suitability as regards their
// requirements in conditions enabling the security of their systems and/or 
// data to be ensured and,  more generally, to use and operate it in the 
// same conditions as regards security. 
// 
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.

function [] = scisptdemo()
   // demo pour l'interface umf
   mode(-1)
   st = stacksize();
   if st(1) < 3000000 then
      messagebox([gettext(" For this demo the current stack size is not enought ");
		 gettext(" enter the following at the scilab prompt :          ");
		 gettext("                                                     ");
		 gettext("              stacksize(3000000);                    ");
		 gettext("                                                     ");
    gettext("           then re enter scisptdemo()                ");
		gettext("                                                     ");
		gettext("               CLICK TO  CONTINUE                    ")]);
      return
   end

   oldln = lines();
   lines(0)

   deff('[]=demoex(num)','exec(scisptdemolist(num,2),-1)')
   a = gda();
   a.title.font_size = 3;
   a;title.font_style = 6;
   

   scisptdemolist = [gettext("how to use this stuff")           , SCI+"/modules/umfpack/examples/scisptdem1.dem";
                     gettext("display a speed comparison test") , SCI+"/modules/umfpack/examples/scisptdem2.dem";
	             gettext("small tests for condestsp")       , SCI+"/modules/umfpack/examples/scisptdem3.dem"];
   while %t
      num=x_choose(scisptdemolist(:,1), gettext("Click to choose a demo"));
      if num==0 then 
	 lines(oldln(1)) , break
      else
	 demoex(num)
      end
   end
   
   sda()
endfunction
