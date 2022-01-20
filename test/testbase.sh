#!/bin/sh
#include this file in tests
export datadir=./data
export testdir=./test

#equals short code (default)
var=`$diffcmd -b $datadir/doca1pag.pdf $datadir/doca1pag_dup.pdf`
if [ "$var" != "0" ]; then
 echo "****failed 1: $var"
 exit 1
fi

#equals short code
var=`$diffcmd -b --outType=0 $datadir/doca1pag.pdf $datadir/doca1pag_dup.pdf`
if [ "$var" != "0" ]; then
 echo "****failed 2: $var"
 exit 1
fi

#equals long code
var=`$diffcmd -b --outType=1 $datadir/doca1pag.pdf $datadir/doca1pag_dup.pdf`
if [ "$var" != "0|documents are the same" ]; then
 echo "****failed 3: $var"
 exit 1
fi

#non existent file 1
var=`$diffcmd -b $datadir/xdoca1pag.pdf $datadir/doca1pag_dup.pdf`
if [ "$var" != "-3" ]; then
 echo "****failed 4: $var"
 exit 1
fi

#non existent file 2
var=`$diffcmd -b $datadir/doca1pag.pdf $datadir/xdoca1pag_dup.pdf`
if [ "$var" != "-4" ]; then
 echo "****failed 5: $var"
 exit 1
fi

#same file
var=`$diffcmd -b $datadir/doca1pag.pdf $datadir/doca1pag.pdf`
if [ "$var" != "5" ]; then
 echo "****failed 6: $var"
 exit 1
fi

#different number of pages
var=`$diffcmd -b $datadir/doca1pag.pdf $datadir/doca2pag.pdf`
if [ "$var" != "2" ]; then
 echo "****failed 7: $var"
 exit 1
fi

#page variable
var=`$diffcmd -b --pages=1 $datadir/doca1pag.pdf $datadir/doca2pag.pdf`
if [ "$var" != "0" ]; then
 echo "****failed 8: $var"
 exit 1
fi
var=`$diffcmd -b --pages=2 $datadir/doca1pag.pdf $datadir/doca2pag.pdf`
if [ "$var" != "4" ]; then
 echo "****failed 8.1: $var"
 exit 1
fi

#1 page a!=b
var=`$diffcmd -b $datadir/doca1pag.pdf $datadir/docb1pag.pdf`
if [ "$var" != "1" ]; then
 echo "****failed 9: $var"
 exit 1
fi

#2 pages a!=b
var=`$diffcmd -b $datadir/doca2pag.pdf $datadir/docb2pag.pdf`
if [ "$var" != "1" ]; then
 echo "****failed 10: $var"
 exit 1
fi
var=`$diffcmd -b --pages=2 $datadir/doca2pag.pdf $datadir/doca3pag.pdf`
if [ "$var" != "0" ]; then
 echo "****failed 10.1: $var"
 exit 1
fi
var=`$diffcmd -b --pages=2 $datadir/doca2pag.pdf $datadir/docb2pag.pdf`
if [ "$var" != "1" ]; then
 echo "****failed 10.2: $var"
 exit 1
fi
var=`$diffcmd -b $datadir/doca2pag.pdf $datadir/doca3pag.pdf`
if [ "$var" != "2" ]; then
 echo "****failed 10.3: $var"
 exit 1
fi

#no file 2
var=`$diffcmd -b `
if [ "$var" != "-1" ]; then
 echo "****failed 11.1: $var"
 exit 1
fi
var=`$diffcmd -b $datadir/doca2pag.pdf`
if [ "$var" != "-1" ]; then
 echo "****failed 11.2: $var"
 exit 1
fi

#invalid arg
var=`$diffcmd -b $datadir/doca2pag.pdf -sgurble`
if [ "$var" != "-1" ]; then
 echo "****failed 12: $var"
 exit 1
fi

# pdf diff: created
tf=`mktemp`
var=`$diffcmd -b --pdfdiff=$tf $datadir/doca2pag.pdf $datadir/docb2pag.pdf`
if [ "$var" != "1" ]; then
 echo "****failed 11.1: $var"
 exit 1
fi

if [ -s $tf ]
 then
  echo
 else
 echo "****failed 11.2: $var"
 exit 1
fi

# pdf diff: not created
tf=`mktemp`
var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pag.pdf $datadir/doca1pag_dup.pdf`
if [ "$var" != "0" ]; then
 echo "****failed 12.1: $var"
 exit 1
fi

if [ -s $tf ]
 then
    echo "****failed 12.2: $var"
    exit 1
 else
    echo
fi

#do a test run
tf=`mktemp`
tr=`mktemp`
var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pag.pdf $datadir/docb1pag.pdf --xmlResult=$tr --key=abcd`
if [ "$var" != "1" ]; then
 echo "****failed 12.3: $var"
 exit 1
fi
echo Result 12.3 is in $tr


#do a test run with settings
tf=`mktemp`
tr=`mktemp`
var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pag.pdf $datadir/docb1pag.pdf --xmlResult=$tr --key=xxx --settings=$testdir/test.ini`
echo Result 13 is in $tr
if [ "$var" != "0" ]; then
 echo "****failed 13: $var"
 exit 1
fi

#do a test run with settings
tf=`mktemp`
tr=`mktemp`
var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pag.pdf $datadir/docb1pag.pdf --xmlResult=$tr --key=xxx --settings=$testdir/comp.ini`
echo Result 13.1 is in $tr  pdf is in $tf
if [ "$var" != "1" ]; then
 echo "****failed 13.1: $var"
 exit 1
fi

#compare fonts
tr=`mktemp`
var=`$diffcmd -b --pdfdiff=$tf $datadir/doca1pagi1.pdf $datadir/doca1pagi2.pdf --xmlResult=$tr --key=xxx`
echo Result 14.0 is in $tr
if [ "$var" != "0" ]; then
 echo "****failed 14.0: $var"
 exit 1
fi

#compare fonts
tr=`mktemp`
var=`$diffcmd --outType=0 -b --pdfdiff=$tf $datadir/doca1pagi1.pdf $datadir/doca1pagi2.pdf --xmlResult=$tr --key=xxx --compareFonts`
echo Result 14.1 is in $tr
if [ "$var" != "6" ]; then
 echo "****failed 14.1: $var"
 exit 1
fi

tr=`mktemp`
var=`$diffcmd --outType=0 -b --pdfdiff=$tf $datadir/doca1pagi2.pdf $datadir/doca1pagi1.pdf --xmlResult=$tr --key=xxx --compareFonts`
echo Result 14.2 is in $tr
if [ "$var" != "6" ]; then
 echo "****failed 14.2: $var"
 exit 1
fi


#StartPage
var=`$diffcmd -b --pages=1 --startPage1=1 --startPage2=2 $datadir/startpage1.pdf $datadir/startpage2.pdf`
if [ "$var" != "0" ]; then
 echo "****failed 15.1: >$var<"
 exit 1
fi

var=`$diffcmd -b --pages=1 --startPage1=2 --startPage2=1 $datadir/startpage2.pdf $datadir/startpage1.pdf`
if [ "$var" != "0" ]; then
 echo "****failed 15.2: $var"
 exit 1
fi

var=`$diffcmd -b --pages=1 --startPage1=1 --startPage2=1 $datadir/startpage2.pdf $datadir/startpage1.pdf`
if [ "$var" != "1" ]; then
 echo "****failed 15.3: $var"
 exit 1
fi

echo "Test completed successfully"
exit 0
