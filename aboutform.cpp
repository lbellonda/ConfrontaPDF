/*
    Copyright © 2011-13 Qtrac Ltd. All rights reserved.
    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 2 of
    the License, or (at your option) any later version. This program is
    distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.
*/

#include "aboutform.hpp"
#include <poppler-version.h>
#include <QApplication>
#include <QHBoxLayout>
#include <QSettings>
#include <QShortcut>
#include <QTabWidget>
#include <QTextBrowser>

const QString AboutForm::Version("1.0.0");
#ifdef COMPARA_IS_CONSOLE
const QString AboutForm::ProgramName("ConfrontaPDFc");
#else
const QString AboutForm::ProgramName("ConfrontaPDF");
#endif

AboutForm::AboutForm(QWidget *parent) : QDialog(parent)
{
    QTextBrowser *aboutBrowser = new QTextBrowser;
    aboutBrowser->setReadOnly(true);
    aboutBrowser->setOpenExternalLinks(true);
    aboutBrowser->setHtml(tr(
    "<table border=0>"
    "<tr><td width=90%><b>%1</a> %2</b> by Luca Bellonda</td><td rowspan=5><img align=right src=\":/icon.png\"></td></tr>"
    "<tr><td>&lt;lbellonda@gmail.com&gt;.</td></tr>"
    "<tr><td>Site: <a href='https://github.com/lbellonda/ConfrontaPDF'>github.com/lbellonda/ConfrontaPDF</a></td></tr>"
    "<tr><td>Report issues at: <a href='https://github.com/lbellonda/ConfrontaPDF/issues'>github.com/lbellonda/ConfrontaPDF/issues</a></td></tr>"

    "<tr><td>Based on DiffPDF by by Mark Summerfield.</td></tr>"

    "<tr><td colspan=2>Built with Qt %3 and Poppler %4.</td></tr>"
    "</table><hr>"
    "<p>This program compares the text or the visual appearance of "
    "each page in two PDF files and supports batch operations."
    "<hr>"
    "<p>The open source DiffPDF was created and maintained by Mark Summerfield 2008-2013. This is a fork of that project by Luca Bellonda, begun in 2015."
    "<br/>The source for the last release of Mark's open source version is available from: <a href='http://www.qtrac.eu/diffpdf-foss.html'>http://www.qtrac.eu/diffpdf-foss.html</a>."
    "<br/>Mark's commercial versions are available from: <a href='http://www.qtrac.eu/diffpdf.html'>http://www.qtrac.eu/diffpdf.html</a>"
    "<br/><br/>Consider also Mark books:"
    "<li><a href=\"http://www.qtrac.eu/gobook.html\">Programming in Go</a></li>"
    "<li><a href=\"http://www.qtrac.eu/aqpbook.html\">Advanced Qt Programming</a></li>"
    "<li><a href=\"http://www.qtrac.eu/py3book.html\">Programming in Python 3</a></li>"
    "<li><a href=\"http://www.qtrac.eu/pyqtbook.html\">Rapid GUI Programming with Python and Qt</a></li>"
    "</ul>").arg(qApp->applicationName())
            .arg(Version).arg(qVersion()).arg(POPPLER_VERSION));
    QTextBrowser *contributorsBrowser = new QTextBrowser;
    contributorsBrowser->setReadOnly(true);
    contributorsBrowser->setHtml(tr("<table>"
    "<tr><td>&bull;</td><td><b>Mark Summerfield</b> &mdash; "
    "wrote the original DiffPDF</td></tr>"
    "<tr><td>&bull;</td><td bgcolor=lightyellow><i>Anonymous Company</i> "
    "&mdash; funded the addition of the margin exclusion "
    "functionality</td></tr>"
    "<tr><td>&bull;</td><td><b>David Paleino</b> &mdash; "
    "Debian packager</td></tr>"
    "<tr><td>&bull;</td><td><b>Dirk Loss</b> &mdash; creating "
    "Mac binaries</td></tr>"
    "<tr><td>&bull;</td><td>Florian Heiderich &mdash; suggested "
    "using composition modes for showing subtle differences</td></tr>"
    "<tr><td>&bull;</td><td><b>Jasmin Blanchette</b> &mdash; "
    "the original idea and subsequent suggestions</td></tr>"
    "<tr><td>&bull;</td><td>Liviu Andronic &mdash; suggested adding "
    "drag and drop</td></tr>"
    "<tr><td>&bull;</td><td>Paul Howarth &mdash; suggestions "
    "resulting in Characters mode</td></tr>"
    "<tr><td>&bull;</td><td bgcolor=\"#F0F0F0\"><i>Pavel Fric</i> &mdash; "
    "Czech translation</td></tr>"
    "<tr><td>&bull;</td><td bgcolor=\"#F0F0F0\"><i>Pierre-Alain "
    "Bandinelli</i>&mdash; French translation</td></tr>"
    "<tr><td>&bull;</td><td bgcolor=\"#F0F0F0\"><i>Rainer Krachten</i> "
    "&mdash; German translation and various suggestions</td></tr>"
    "<tr><td>&bull;</td><td>Rory Gordon &mdash; suggested adding "
    "drag and drop</td></tr>"
    "<tr><td>&bull;</td><td>Bryan Huh &mdash; subtle bug fix"
    "</td></tr>"
    "<tr><td>&bull;</td><td><b>Steven Lee</b> &mdash; creating "
    "Windows binaries</td></tr>"
    "</table>"));
    QTextBrowser *licenceBrowser = new QTextBrowser;
    licenceBrowser->setReadOnly(true);
    licenceBrowser->setHtml(tr(
    "This program is free software: you can redistribute it "
    "and/or modify it under the terms of the GNU General Public License "
    "as published by the Free Software Foundation, either version 2 of "
    "the License, or (at your option), any "
    "later version. This program is distributed in the hope that it will "
    "be useful, but WITHOUT ANY WARRANTY; without even the implied "
    "warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. "
    "See the GNU General Public License (in file <tt>gpl-2.0.txt</tt>) "
    "for more details."));
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->addTab(aboutBrowser, tr("&About"));
    tabWidget->addTab(contributorsBrowser, tr("&Contributors"));
    tabWidget->addTab(licenceBrowser, tr("&License"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tabWidget);
    setLayout(layout);
    resize(520, 486);
    setWindowTitle(tr("%1 — About").arg(AboutForm::ProgramName));
}
