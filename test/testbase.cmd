rem include this file in tests
rem set datadir=.\data
rem set testdir=.\test

set filediff=%temp%\test_mydiff.pdf
set fileres=%temp%\test_res.xml

echo test 1
set resFile=%temp%\res.out
del %resFile%
%diffcmd% -b %datadir%\doca1pag.pdf %datadir%\doca1pag_dup.pdf >%resfile%
set /p var= <%resFile%
if NOT "%var%" == "0"  goto error1

rem equals short code
echo test 2
del %resFile%
%diffcmd% -b --outType=0  %datadir%\doca1pag.pdf %datadir%\doca1pag_dup.pdf >%resfile%
rem var=`$diffcmd -b --outType=0 $datadir/doca1pag.pdf $datadir/doca1pag_dup.pdf`
set /p var= <%resFile%
if NOT "%var%" == "0"  goto error2


echo equals long code
del %resFile%
%diffcmd% -b --outType=1 %datadir%\doca1pag.pdf %datadir%\doca1pag_dup.pdf >%resfile%
rem var=`$diffcmd -b --outType=1 $datadir/doca1pag.pdf $datadir/doca1pag_dup.pdf`
set /p var= <%resFile%
if NOT "%var%" == "0|documents are the same"  goto error3


rem #non existent file 1
del %resFile%
%diffcmd% -b %datadir%\xdoca1pag.pdf %datadir%\doca1pag_dup.pdf >%resfile%
rem var=`$diffcmd -b $datadir/xdoca1pag.pdf $datadir/doca1pag_dup.pdf`
set /p var= <%resFile%
if NOT "%var%" == "-3"  goto error4


rem #non existent file 2
del %resFile%
%diffcmd% -b %datadir%\doca1pag.pdf %datadir%\xdoca1pag_dup.pdf >%resfile%
rem var=`$diffcmd -b $datadir/doca1pag.pdf $datadir/xdoca1pag_dup.pdf`
set /p var= <%resFile%
if NOT "%var%" == "-4"  goto error5

rem #same file
del %resFile%
%diffcmd% -b %datadir%\doca1pag.pdf %datadir%\doca1pag.pdf >%resfile%
rem var=`$diffcmd -b $datadir/doca1pag.pdf $datadir/doca1pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "5"  goto error6

rem #different number of pages
del %resFile%
%diffcmd% -b %datadir%\doca1pag.pdf %datadir%\doca2pag.pdf >%resfile%
rem var=`$diffcmd -b $datadir/doca1pag.pdf $datadir/doca2pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "2"  goto error7


rem #page variable
del %resFile%
%diffcmd% -b --pages=1 %datadir%\doca1pag.pdf %datadir%\doca2pag.pdf >%resfile%
rem var=`$diffcmd -b --pages=1 $datadir/doca1pag.pdf $datadir/doca2pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "0"  goto error8

del %resFile%
%diffcmd% -b --pages=2 %datadir%\doca1pag.pdf %datadir%\doca2pag.pdf >%resfile%
var=`$diffcmd -b --pages=2 $datadir/doca1pag.pdf $datadir/doca2pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "4"  goto error8_1

rem #1 page a!=b
del %resFile%
%diffcmd% -b %datadir%\doca1pag.pdf %datadir%\docb1pag.pdf >%resfile%
rem var=`$diffcmd -b $datadir/doca1pag.pdf $datadir/docb1pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "1"  goto error9

rem #2 pages a!=b
del %resFile%
%diffcmd% -b %datadir%\doca2pag.pdf %datadir%\docb2pag.pdf >%resfile%
rem var=`$diffcmd -b $datadir/doca2pag.pdf $datadir/docb2pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "1"  goto error10

del %resFile%
%diffcmd% -b --pages=2 %datadir%\doca2pag.pdf %datadir%\doca3pag.pdf >%resfile%
rem var=`$diffcmd -b --pages=2 $datadir/doca2pag.pdf $datadir/doca3pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "0"  goto error10_1

del %resFile%
%diffcmd% -b --pages=2 %datadir%\doca2pag.pdf %datadir%\docb2pag.pdf >%resfile%
rem var=`$diffcmd -b --pages=2 $datadir/doca2pag.pdf $datadir/docb2pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "1"  goto error10_2

del %resFile%
%diffcmd% -b %datadir%\doca2pag.pdf %datadir%\doca3pag.pdf >%resfile%
rem var=`$diffcmd -b $datadir/doca2pag.pdf $datadir/doca3pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "2"  goto error10_3

rem #no file 2
del %resFile%
%diffcmd% -b  >%resfile%
rem var=`$diffcmd -b `
set /p var= <%resFile%
if NOT "%var%" == "-1"  goto error11_1

del %resFile%
%diffcmd% -b %datadir%\doca2pag.pdf  >%resfile%
rem var=`$diffcmd -b $datadir/doca2pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "-1"  goto error11_2

rem #invalid arg
del %resFile%
%diffcmd% -b %datadir%\doca2pag.pdf -sgurble  >%resfile%
rem var=`$diffcmd -b $datadir/doca2pag.pdf -sgurble`
set /p var= <%resFile%
if NOT "%var%" == "-1"  goto error12

del %resFile%
del %filediff%
del %fileres%

rem # pdf diff: created
set tf=%filediff%
%diffcmd% -b --pdfdiff=%tf% %datadir%\doca2pag.pdf %datadir%\docb2pag.pdf >%resfile%
rem var=`$diffcmd -b --pdfdiff=$tf $datadir/doca2pag.pdf $datadir/docb2pag.pdf`
set /p var= <%resFile%
if NOT "%var%" == "1"  goto error12_1
if NOT EXIST %tf% goto error12_2

rem # pdf diff: not created
del %resFile%
del %filediff%
del %fileres%
set tf=%filediff%
%diffcmd% -b --pdfdiff=%tf% %datadir%\doca1pag.pdf %datadir%\doca1pag_dup.pdf >%resfile%
rem var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pag.pdf $datadir/doca1pag_dup.pdf`
set /p var= <%resFile%
if NOT "%var%" == "0"  goto error12_3
if  EXIST %tf% goto error12_4

rem #do a test run
del %resFile%
del %filediff%
del %fileres%
set tf=%filediff%
set tr=%fileres%
%diffcmd% -b --pdfdiff=%tf% %datadir%\doca1pag.pdf %datadir%\docb1pag.pdf --xmlResult=%tr% --key=abcd >%resfile%
rem var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pag.pdf $datadir/docb1pag.pdf --xmlResult=$tr --key=abcd`
set /p var= <%resFile%
if NOT "%var%" == "1"  goto error12_5

rem #do a test run with settings
del %resFile%
del %filediff%
del %fileres%
set tf=%filediff%
set tr=%fileres%
%diffcmd% -b --pdfdiff=%tf% %datadir%\doca1pag.pdf %datadir%\docb1pag.pdf --xmlResult=%tr% --key=xxx --settings=%testdir%\test.ini >%resfile%
rem var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pag.pdf $datadir/docb1pag.pdf --xmlResult=$tr --key=xxx --settings=$testdir/test.ini`
set /p var= <%resFile%
if NOT "%var%" == "0"  goto error13

rem #do a test run with settings
del %resFile%
del %filediff%
del %fileres%
set tf=%filediff%
set tr=%fileres%
%diffcmd% -b --pdfdiff=%tf% %datadir%\doca1pag.pdf %datadir%\docb1pag.pdf --xmlResult=%tr% --key=xxx --settings=%testdir%\comp.ini >%resfile%
rem var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pag.pdf $datadir/docb1pag.pdf --xmlResult=$tr --key=xxx --settings=$testdir/comp.ini`
set /p var= <%resFile%
if NOT "%var%" == "1"  goto error13_1

rem #compare fonts
del %resFile%
del %filediff%
del %fileres%
set tf=%filediff%
set tr=%fileres%
%diffcmd% -b --pdfdiff=$tf %datadir%\doca1pagi1.pdf %datadir%\doca1pagi2.pdf --xmlResult=%tr% --key=xxx  >%resfile%
rem var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pagi1.pdf $datadir/doca1pagi2.pdf --xmlResult=$tr --key=xxx`
set /p var= <%resFile%
if NOT "%var%" == "0"  goto error14

rem #compare fonts
del %resFile%
del %filediff%
del %fileres%
set tf=%filediff%
set tr=%fileres%
%diffcmd% --outType=0 -b --pdfdiff=%tf% %datadir%\doca1pagi1.pdf %datadir%\doca1pagi2.pdf --xmlResult=%tr% --key=xxx --compareFonts  >%resfile%
rem var=`$diffcmd --outType=0 -b --pdfdiff=$tf $datadir/doca1pagi1.pdf $datadir/doca1pagi2.pdf --xmlResult=$tr --key=xxx --compareFonts`
set /p var= <%resFile%
if NOT "%var%" == "6"  goto error14_1

rem #compare fonts
del %resFile%
del %filediff%
del %fileres%
set tf=%filediff%
set tr=%fileres%
%diffcmd% --outType=0 -b --pdfdiff=%tf% %datadir%\doca1pagi2.pdf %datadir%\doca1pagi1.pdf --xmlResult=%tr% --key=xxx --compareFonts >%resfile%
rem var=`$diffcmd --outType=0 -b --pdfdiff=$tf $datadir/doca1pagi2.pdf $datadir/doca1pagi1.pdf --xmlResult=$tr --key=xxx --compareFonts`
set /p var= <%resFile%
if NOT "%var%" == "6"  goto error14_2


echo test ok
pause 0
exit 0

:error1
echo "****failed 1: %var%"
pause 0 
exit 1


:error2
echo "****failed 2: %var%"
 pause 0 
 exit 1

:error3
echo "****failed 3: %var%"
pause 0 
exit 1


:error4
echo "****failed 4: %var%"
 pause 0 
 exit 1


:error5
echo "****failed 5: %var%"
 pause 0 
 exit 1
 
:error6
echo "****failed 6: %var%"
 pause 0 
 exit 1

:error7
echo "****failed 7: %var%"
 pause 0 
 exit 1

 
:error8
echo "****failed 8: %var%"
pause 0 
exit 1

:error8_1
echo "****failed 8.1: %var%"
pause 0 
exit 1

:error9
echo "****failed 9: %var%"
 pause 0 
 exit 1

:error10
echo "****failed 9: %var%"
pause 0 
exit 1

:error10
echo "****failed 10: %var%"
pause 0 
exit 1

:error10_1
echo "****failed 10.1: %var%"
pause 0 
exit 1

:error10_2
echo "****failed 10.2: %var%"
 pause 0 
 exit 1

:error10_3
echo "****failed 10.3: %var%"
pause 0 
exit 1

:error11_1
echo "****failed 11.1: %var%"
pause 0 
 exit 1
 
:error11_2
echo "****failed 11.2: %var%"
pause 0 
 exit 1

:error12
echo "****failed 12:%var%"
pause 0 
exit 1

:error12_1
echo "****failed 12.1: %var%"
pause 0 
exit 1

:error12_2
echo "****failed 12.3: %var%"
pause 0 
exit 1

:error12_3
echo "****failed 12.3: %var%"
pause 0 
exit 1

:error12_4
echo "****failed 12.4: %var%"
pause 0 
exit 1
aaa

:error13
echo "****failed 13: %var%"
pause 0 
exit 1

:error13_1
echo "****failed 13.1: %var%"
pause 0 
exit 1

:error14
echo "****failed 14: %var%"
pause 0 
exit 1

:error14_1
echo "****failed 14.1: %var%"
pause 0 
exit 1

:error14_2
echo "****failed 14.2: %var%"
pause 0 
exit 1

:error14_3
echo "****failed 14.3: %var%"
pause 0 
exit 1
