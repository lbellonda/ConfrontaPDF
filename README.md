ConfrontaPDF
============

ConfrontaPDF compares two PDF files via GUI or command line.

ConfrontaPDF is based on DiffPDF. The open source DiffPDF was created and maintained by Mark
Summerfield 2008-2013. This is a fork of that project by Luca Bellonda, begun in 2015.

The source for the last release of Mark's open source version is available from:
http://www.qtrac.eu/diffpdf-foss.html. Mark's commercial versions are available from:
http://www.qtrac.eu/diffpdf.html

Command line
------------
ConfrontaPDF can compare two PDF files, writing the result to standard output,
an XML file and generate a PDF document with the differences between the
input documents. It is scriptable, and does not open windows or wait for
user input during the batch operation, making it suitable for automated tests.
ConfrontaPDFc is a version of ConfrontaPDF that always runs in command line mode.
The result is printed on standard output:

- 0 means that the PDFs are identical
- any other code means a difference or an error

Output can be:

- Code only (default)
sample:

      > confrontapdfc -b a.pdf b.pdf
      > 0

- Code and description using `--outType=1` switch
sample:

      > confrontapdfc -b --outType=1 a.pdf b.pdf
      > 0|no differences

- An XML file with a detailed result can be written using `--xmlResult=path`
sample:

      > confrontapdfc -b --xmlResult=result.xml a.pdf b.pdf
      > 0

GUI
---
By default the comparison is of the words on each pair of pages, but
comparing character by character is also supported (e.g., for
logographic languages). And there's also support for comparing the pages
by appearance (for example, if a diagram is changed or if a paragraph is
reformatted, or a font changed). It is also possible to compare
particular pages or page ranges. For example, if there are two versions
of a PDF file, one with pages 1-12 and the other with pages 1-13 because
of an extra page having been added as page 4, they can be compared by
specifying two page ranges, 1-12 for the first and 1-3, 5-13 for the
second. This will make ConfrontaPDF compare pages in the pairs (1, 1), (2,
2), (3, 3), (4, 5), (5, 6), and so on, to (12, 13).

A couple of example PDF files are in the data folder of the sources.

More information is available in the program's tooltips and About box.


Windows Users
=============
Use `ConfrontaPDFc` for command line only operations.

Compiling and Installing ConfrontaPDF
=====================================

Prerequisites:
- C++ compiler
- Qt 4 or Qt 5 libraries
- the Poppler libraries (at least 0.20.1)

Qt4 requires the Poppler's Qt 4 headers.
Qt5 requires the Poppler's Qt 5 headers.

Linux and BSD users should be able to get compiling from sources.
For other systems you'll need to get Qt and Poppler separately.

Download the sources from site: git clone https://github.com/lbellonda/ConfrontaPDF
or download a zip.
1. Change directory to ConfrontaPDF

        $ cd ConfrontaPDF

2. Run lrelease;

        $ lrelease confrontapdf.pro

3. Run qmake on confrontapdf;

        $ qmake confrontapdf.pro

4. Run make

        $ make

5. Run qmake on confrontapdfc

        $ qmake confrontapdfc.pro

6. Run make

        $ make

7. Copy or soft-link the confrontapdf and confrontapdfc executable to somewhere on your PATH
8. Only the executables are needed; all the files that were unpacked or
   generated can be safely deleted.

Windows users can find an installer script to customize in the install folder.

Two helper files are provided for Windows and Unix-like systems:
- compile.sh
- compile.cmd

Running ConfrontaPDF
====================

If you hit a bug, please report it at
https://github.com/lbellonda/ConfrontaPDF/issues.

License
=======

This program was written by Luca Bellonda.
Copyright © 2015 Luca Bellonda. All rights reserved.
Based on DiffPfd by Mark Summerfield.
Copyright © 2008-13 Qtrac Ltd. All rights reserved.

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 2 of the License, or (at your
option), any later version. This program is distributed in the hope that
it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License (in file gpl-2.0.txt) for more details.
