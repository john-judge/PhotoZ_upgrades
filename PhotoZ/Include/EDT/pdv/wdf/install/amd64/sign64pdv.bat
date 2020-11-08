
%WINDDK%\bin\amd64\signtool sign /v /ac MSCV-VSClass3.cer /s my /n "ENGINEERING DESIGN TEAM" /t http://timestamp.verisign.com/scripts/timstamp.dll pdvwdf.sys
%WINDDK%\bin\amd64\makecat -v pdvwdf.cdf
%WINDDK%\bin\amd64\signtool sign /v /ac MSCV-VSClass3.cer /s my /n "ENGINEERING DESIGN TEAM" /t http://timestamp.verisign.com/scripts/timstamp.dll pdvwdf.cat


