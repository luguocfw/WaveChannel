@echo off
set cur_path=%~dp0
set "cur_path=%cur_path:\=/%"
set GYP_TOOLS=%cur_path%tools/GYP-Tools/gyp
set GYP_DEFINES=OS=win PRO_ROOT=%cur_path%

if exist %cur_path%tools/ (
  echo dirent: %cur_path%tools/ exist;
) else (
  md "%cur_path%tools"
)

if exist %cur_path%/tools/GYP-Tools/ (
  echo dirent : %cur_path%tools/GYP-Tools/ already exist
) else (
  cd %cur_path%tools/
  git clone https://github.com/luguocfw/GYP-Tools
  cd %cur_path%
)

%GYP_TOOLS% --depth=. wave_trans.gyp


