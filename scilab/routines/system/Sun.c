/* Copyright INRIA/ENPC */

#include "../machine.h"

/** only used to force the linker to load all calelm functions **/

Sun_contents(x) 
     int x;
{
  if ( x== 1) 
    {
      C2F(isanan)();
      BuildName();
      CallDynInterf();
      C2F(getenvc)();
      GetDynFunc();
      LinkStatus();
      SciLink();
      SciLinkInit();
      SearchInDynLinks();
      ShowDynLinks();
      C2F(iislink)();
      C2F(iscilink)();
      C2F(isciulink)();
      C2F(systemc)();
      C2F(setprlev)();
      C2F(zzledt)();
      controlC_handler();
      C2F(csignal)();
      C2F(getpidc)();
      C2F(stimer)();
      C2F(timer)();
      get_echo_mode();
      get_is_reading();
      set_echo_mode();
      set_is_reading();
      ScilabC2MStr2();
      ScilabCM2MStr();
      ScilabMStr2C();
      ScilabMStr2CM();
      ScilabStr2C();
      strwidth();
      RemoveInterf();
      SciLibLoad();
      ShowInterf();
      C2F(addinter)();
      C2F(userlk)();
      HelpActivate();
      HelpGetPath();
      Help_Init();
      SciCallHelp();
      Sci_Apropos();
      Sci_Help();
      SetAproposTopics();
      initHelpDatas();
      setHelpTopicInfo();
      C2F(settmpdir)();
      C2F(tmpdirc)();
      C2F(cluni0)();
      C2F(inffic)();
      C2F(infficl)();
      C2F(bashos)();
      C2F(basin)();
      C2F(myback)();
      C2F(basou1)();
      C2F(basout)();
      C2F(clunit)();
      C2F(ctrlc)();
      C2F(dbasin)();
      C2F(nextv)();
      C2F(s2int)();
      C2F(s2val)();
      C2F(fgetarg)();
      C2F(getpro)();
      C2F(inibrk)();
      C2F(sunieee)();
      C2F(sigbas)();
    }
}
