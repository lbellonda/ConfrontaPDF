/*
    Copyright © 2008-13 Qtrac Ltd. All rights reserved.
    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 2 of
    the License, or (at your option) any later version. This program is
    distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.
*/

#include "mainwindow.hpp"
#include <QApplication>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>
#include <QSettings>
#include <QTextCodec>
#include <QTextStream>
#include <QTranslator>


int main(int argc, char *argv[])
{
    StartupParameters startupParameters;
    QApplication app(argc, argv);
#ifdef Q_WS_MAC
    app.setCursorFlashTime(0);
#endif
    app.setOrganizationName("Qtrac Ltd.");
    app.setOrganizationDomain("qtrac.eu");
    app.setApplicationName("DiffPDF");
    app.setWindowIcon(QIcon(":/icon.png"));
#if !defined(USE_QT5)
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif
    QTextStream out(stdout);
    QStringList args = app.arguments().mid(1);
    QSettings settings;
    InitialComparisonMode comparisonMode = static_cast<
            InitialComparisonMode>(settings.value("InitialComparisonMode",
                        CompareWords).toInt());
    const QString LanguageOption = "--language=";
    QString filename1;
    QString filename2;
    QString language = QLocale::system().name();
    bool optionsOK = true;
    Debug debug = DebugOff;
    Status status ;
    bool seenCompareType = false;
    QStringList errors;
    foreach (const QString arg, args) {
        if (optionsOK && (arg == "--appearance" || arg == "-a")) {
            comparisonMode = CompareAppearance;
            seenCompareType = true ;
        } else if (optionsOK && (arg == "--characters" || arg == "-c")) {
            comparisonMode = CompareCharacters;
            seenCompareType = true ;
        } else if (optionsOK && (arg == "--words" || arg == "-w")) {
            comparisonMode = CompareWords;
            seenCompareType = true ;
        } else if (optionsOK && arg.startsWith(LanguageOption))
            language = arg.mid(LanguageOption.length());
        else if (optionsOK && (arg == "--help" || arg == "-h")) {
            out << "usage: diffpdf [options] [file1.pdf [file2.pdf]]\n\n"
                "A GUI program that compares two PDF files and shows "
                "their differences.\n"
                "\nThe files are optional and are normally set "
                "through the user interface.\n\n"
                "options:\n"
                "--help        -h   show this usage text and terminate "
                "(run the program without this option and press F1 for "
                "online help)\n"
                "--appearance  -a   set the initial comparison mode to "
                "Appearance\n"
                "--characters  -c   set the initial comparison mode to "
                "Characters\n"
                "--words       -w   set the initial comparison mode to "
                "Words\n"
                "--language=xx      set the program to use the given "
                "translation language, e.g., en for English, cz for "
                "Czech; English will be used if there is no translation "
                "available\n"
                "--debug=2          write the text fed to the sequence "
                "matcher into temporary files (e.g., /tmp/page1.txt "
                "etc.)\n"
                "--debug=3          as --debug=3 but also includes "
                "coordinates in y, x order\n"
                "--batch -b activates the batch mode\n"
                "result information returned in batch mode:\n"
                "  --outType=0 prints only the return code\n"
                "  --outType=1 prints the return code and a description\n"
                "--pages=nn the number of pages to compare (default all)\n"
                "--startPage1=nn the stating page number for file 1\n"
                "--startPage2=nn the stating page number for file 2\n"
                "--pdfdiff=path generates a pdf with differences \n"
                "\nRun the program without the --help option and click "
                "About to see copyright and license details\n"
                ;
            return 0;
        }
        else if (optionsOK && (arg == "--debug" || arg == "--debug=1" ||
                               arg == "--debug1"))
            ; // basic debug mode currently does nothing (did show zones)
        else if (optionsOK && (arg == "--debug=2" || arg == "--debug2"))
            debug = DebugShowTexts;
        else if (optionsOK && (arg == "--debug=3" || arg == "--debug3"))
            debug = DebugShowTextsAndYX;
        else if (optionsOK && arg == "--") {
            optionsOK = false;
        } else if (optionsOK && startupParameters.parseArgument(arg, &status) ) {
            ; // empty statement
        } else if (filename1.isEmpty() && arg.toLower().endsWith(".pdf")) {
            filename1 = arg;
            startupParameters.setFile1(filename1);
        } else if (filename2.isEmpty() && arg.toLower().endsWith(".pdf")) {
            filename2 = arg;
            startupParameters.setFile2(filename2);
        } else {
            errors << arg ;
        }
    }
    if(!startupParameters.isBatch()) {
        QTranslator qtTranslator;
        qtTranslator.load("qt_" + language,
            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        app.installTranslator(&qtTranslator);
        QTranslator appTranslator;
        appTranslator.load("diffpdf_" + language, ":/");
        app.installTranslator(&appTranslator);
    }

    if( errors.count() > 0 ) {
        if( startupParameters.isBatch() ) {
            status.setParamError( true, errors.first());
            return status.returnOp(startupParameters.returnType());
        } else {
            foreach( QString str, errors ) {
                out << QObject::tr("unrecognized argument '") << str << "'\n";
            }
        }
    }
    if( startupParameters.isBatch() && !seenCompareType ) {
        startupParameters.setComparisonMode(CompareAppearance);
    }
    startupParameters.setComparisonMode(comparisonMode);
    if( status.isError() ) {
        return status.returnOp(startupParameters.returnType());
    }
    if( !startupParameters.validate(&status) ) {
        return status.returnOp(startupParameters.returnType());
    }

    MainWindow window(debug, comparisonMode, filename1, filename2,
            language.left(2), &startupParameters, &status ); // We want de not de_DE etc.
    if( startupParameters.isBatch() ) {
        window.batchOperation();
    } else {
        window.show();
        app.exec();
    }
    return status.returnOp(startupParameters.returnType());
}

