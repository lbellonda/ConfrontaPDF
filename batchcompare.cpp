/*
    Copyright © 2015 Luca Bellonda. All rights reserved.
    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 2 of
    the License, or (at your option) any later version. This program is
    distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
    for more details.
*/


#include "batchcompare.h"

#include "generic.hpp"
#include "mainwindow.hpp"
#include "sequence_matcher.hpp"
#include "textitem.hpp"
#ifdef DEBUG
#include <QtDebug>
#endif
#include <QDir>
#include <QEvent>
#include <QLabel>
#include <QPainter>
#include <QPixmapCache>
#include <QPlainTextEdit>
#include <QPrinter>
#include <QSettings>
#include <QUrl>
#include <QXmlStreamWriter>
#include <QBuffer>
#include "aboutform.hpp"

BatchCompare::BatchCompare(const Debug debug,
        const InitialComparisonMode comparisonMode,
        const QString &filename1, const QString &filename2,
        StartupParameters *startupParameters, Status *status, QWidget *parent)
    : QObject(parent),
      savePages(SaveBothPages),
      debug(debug)
{
    pages1LineEdit = filename1;
    pages2LineEdit = filename2;
    _startupParameters = startupParameters ;
    currentCompareIndex = comparisonMode ;
    _status = status ;
    cacheSizeMB = 100 ;

    initValues();
    QPixmapCache::setCacheLimit(1000 * qBound(1, cacheSizeMB, 100));
    //writeToSettings("test.ini");
}


void BatchCompare::readFromSettings()
{
    if( _startupParameters->settingsFile().isEmpty()) {
        return ;
    }
    QSettings settings(_startupParameters->settingsFile(), QSettings::IniFormat);
    combineTextHighlighting = settings.value("CombineTextHighlighting",true).toBool();
    showHighlight = settings.value("ShowHighlight", -1).toInt();
    pen = settings.value("Outline", pen).value<QPen>();
    brush.setColor(pen.color());
    brush.setStyle(Qt::SolidPattern);
    brush = settings.value("Fill", brush).value<QBrush>();
    zoningGroupBoxChecked = settings.value("Zoning/Enable", false).toBool();

    zoomSpinBoxValue = settings.value("Zoom", 100).toInt();
    columnsSpinBoxValue = settings.value("Columns", 1).toInt();
    toleranceRSpinBoxValue = settings.value("Tolerance/R", 8).toInt();
    toleranceYSpinBoxValue = settings.value("Tolerance/Y", 10).toInt();
    marginsGroupBoxChecked = settings.value("Margins/Exclude", false).toBool();
    leftMarginSpinBoxValue = settings.value("Margins/Left", 0).toInt();
    rightMarginSpinBoxValue = settings.value("Margins/Right", 0).toInt();
    topMarginSpinBoxValue = settings.value("Margins/Top", 0).toInt();
    bottomMarginSpinBoxValue = settings.value("Margins/Bottom", 0).toInt();
    cacheSizeMB = settings.value("CacheSizeMB", 25).toInt();
    compositionMode = static_cast<QPainter::CompositionMode>(settings.value("compositionMode", -1).toInt()) ;
    squareSize = settings.value("SquareSize", squareSize).toInt();
    ruleWidth = settings.value("RuleWidth", ruleWidth).toDouble();
    overlap = settings.value("Overlap", overlap).toInt();
    combineTextHighlighting = settings.value("CombineTextHighlighting", combineTextHighlighting).toBool();
    opacity = settings.value("Opacity", opacity).toInt();
}

void BatchCompare::writeToSettings(const QString &filePath)
{
    //WARNING: this function still to be completed.
    QSettings settings(filePath, QSettings::IniFormat );
    settings.setValue("CombineTextHighlighting", combineTextHighlighting);
    settings.setValue("ShowHighlight", showHighlight);
    settings.setValue("Outline", pen);
    settings.setValue("Fill", brush);
    settings.setValue("Zoning/Enable", zoningGroupBoxChecked);

    settings.setValue("Zoom", zoomSpinBoxValue);
    settings.setValue("Columns", columnsSpinBoxValue);
    settings.setValue("Tolerance/R", toleranceRSpinBoxValue);
    settings.setValue("Tolerance/Y", toleranceYSpinBoxValue);
    settings.setValue("Margins/Exclude", marginsGroupBoxChecked);
    /*
    leftMarginSpinBoxValue = settings.value("Margins/Left", 0).toInt();
    rightMarginSpinBoxValue = settings.value("Margins/Right", 0).toInt();
    topMarginSpinBoxValue = settings.value("Margins/Top", 0).toInt();
    bottomMarginSpinBoxValue = settings.value("Margins/Bottom", 0).toInt();
    cacheSizeMB = settings.value("CacheSizeMB", 25).toInt();
    compositionMode = static_cast<QPainter::CompositionMode>(settings.value("compositionMode", -1).toInt()) ;
*/
    settings.setValue("SquareSize", squareSize);
    settings.setValue("RuleWidth", ruleWidth);

    settings.setValue("Overlap", overlap);
    settings.setValue("CombineTextHighlighting", combineTextHighlighting);
    settings.setValue("Opacity", opacity);

    settings.sync();
}

void BatchCompare::initValues()
{
    combineTextHighlighting = true ;
    pen.setStyle(Qt::NoPen);
    pen.setColor(Qt::red);
    brush.setColor(pen.color());
    brush.setStyle(Qt::SolidPattern);

    marginsGroupBoxChecked = false;
    zoningGroupBoxChecked = false ;
    zoomSpinBoxValue = 100 ;
    showHighlight = -1 ;
    squareSize= 10;
    ruleWidth= 1.5;
    zoningGroupBoxChecked = false;


    columnsSpinBoxValue = 1;
    toleranceRSpinBoxValue = 8 ;
    toleranceYSpinBoxValue = 10 ;
    marginsGroupBoxChecked = false;
    leftMarginSpinBoxValue = 0;
    rightMarginSpinBoxValue = 0;
    topMarginSpinBoxValue = 0;
    bottomMarginSpinBoxValue = 0;
    cacheSizeMB = 25;
    compositionMode = static_cast<QPainter::CompositionMode>(-1) ;
    overlap = 5 ;
    combineTextHighlighting = true ;
    opacity = 13 ;
}

const QPair<QString, QString> BatchCompare::cacheKeys(const int index,
        const PagePair &pair) const
{
    int comparisonMode1;
    int comparisonMode2;
    // TODO: refactor this key, it is very bad.
    if (currentCompareIndex == CompareAppearance) {
        // this key will be always positive, even if the value is -1
        comparisonMode1 = 1;
        comparisonMode2 = static_cast<int>(compositionMode);
    } else {
        // this will be always negative, given that App is 0.
        comparisonMode1 = 0 ;
        comparisonMode2 = currentCompareIndex;
    }
    QString zoning;
    if (zoningGroupBoxChecked)
        zoning = QString("%1:%2:%3").arg(columnsSpinBoxValue)
                .arg(toleranceRSpinBoxValue)
                .arg(toleranceYSpinBoxValue);
    QString margins;
    if (marginsGroupBoxChecked)
        margins = QString("%1:%2:%3:%4").arg(topMarginSpinBoxValue)
                .arg(bottomMarginSpinBoxValue)
                .arg(leftMarginSpinBoxValue)
                .arg(rightMarginSpinBoxValue);
    const QString key = QString("%1:%2:%3:%4:%5:%6").arg(index)
            .arg(zoomSpinBoxValue).arg(comparisonMode1).arg(comparisonMode2).arg(zoning)
            .arg(margins);
    const QString key1 = QString("1:%1:%2:%3").arg(key).arg(pair.left)
            .arg(filename1);
    const QString key2 = QString("2:%1:%2:%3").arg(key).arg(pair.right)
            .arg(filename2);
    return qMakePair(key1, key2);
}

void BatchCompare::computeTextHighlights(QPainterPath *highlighted1,
        QPainterPath *highlighted2, const PdfPage &page1,
        const PdfPage &page2, const int DPI)
{
    const bool ComparingWords = currentCompareIndex == CompareWords;
    QRectF rect1;
    QRectF rect2;
    const int OVERLAP = overlap;
    const bool COMBINE = combineTextHighlighting;
    QRectF rect;
    if (marginsGroupBoxChecked)
        rect = pointRectForMargins(page1->pageSize());
    const TextBoxList list1 = getTextBoxes(page1, rect);
    const TextBoxList list2 = getTextBoxes(page2, rect);
    TextItems items1 = ComparingWords ? getWords(list1)
                                      : getCharacters(list1);
    TextItems items2 = ComparingWords ? getWords(list2)
                                      : getCharacters(list2);
    const int ToleranceY = toleranceYSpinBoxValue;
    if (zoningGroupBoxChecked) {
        const int ToleranceR = toleranceRSpinBoxValue;
        const int Columns = columnsSpinBoxValue;
        items1.columnZoneYxOrder(page1->pageSize().width(), ToleranceR,
                                 ToleranceY, Columns);
        items2.columnZoneYxOrder(page2->pageSize().width(), ToleranceR,
                                 ToleranceY, Columns);
    }

    if (debug >= DebugShowTexts) {
        const bool Yx = debug == DebugShowTextsAndYX;
        items1.debug(1, ToleranceY, ComparingWords, Yx);
        items2.debug(2, ToleranceY, ComparingWords, Yx);
    }

    SequenceMatcher matcher(items1.texts(), items2.texts());
    RangesPair rangesPair = computeRanges(&matcher);
    rangesPair = invertRanges(rangesPair.first, items1.count(),
                              rangesPair.second, items2.count());

    foreach (int index, rangesPair.first)
        addHighlighting(&rect1, highlighted1, items1.at(index).rect,
                        OVERLAP, DPI, COMBINE);
    if (!rect1.isNull() && !rangesPair.first.isEmpty())
        highlighted1->addRect(rect1);
    foreach (int index, rangesPair.second)
        addHighlighting(&rect2, highlighted2, items2.at(index).rect,
                        OVERLAP, DPI, COMBINE);
    if (!rect2.isNull() && !rangesPair.second.isEmpty())
        highlighted2->addRect(rect2);
}


void BatchCompare::addHighlighting(QRectF *bigRect,
        QPainterPath *highlighted, const QRectF wordOrCharRect,
        const int OVERLAP, const int DPI, const bool COMBINE)
{
    QRectF rect = wordOrCharRect;
    scaleRect(DPI, &rect);
    if (COMBINE && rect.adjusted(-OVERLAP, -OVERLAP, OVERLAP, OVERLAP)
        .intersects(*bigRect))
        *bigRect = bigRect->united(rect);
    else {
        highlighted->addRect(*bigRect);
        *bigRect = rect;
    }
}

void BatchCompare::computeVisualHighlights(QPainterPath *highlighted1,
        QPainterPath *highlighted2, const QImage &plainImage1,
        const QImage &plainImage2)
{
    const int SQUARE_SIZE = squareSize;
    QRect box;
    if (marginsGroupBoxChecked)
        box = pixelRectForMargins(plainImage1.size());
    QRect target;
    for (int x = 0; x < plainImage1.width(); x += SQUARE_SIZE) {
        for (int y = 0; y < plainImage1.height(); y += SQUARE_SIZE) {
            const QRect rect(x, y, SQUARE_SIZE, SQUARE_SIZE);
            if (!box.isEmpty() && !box.contains(rect))
                continue;
            QImage temp1 = plainImage1.copy(rect);
            QImage temp2 = plainImage2.copy(rect);
            if (temp1 != temp2) {
                if (rect.adjusted(-1, -1, 1, 1).intersects(target))
                    target = target.united(rect);
                else {
                    highlighted1->addRect(target);
                    highlighted2->addRect(target);
                    target = rect;
                }
            }
        }
    }
    if (!target.isNull()) {
        highlighted1->addRect(target);
        highlighted2->addRect(target);
    }
}

QRect BatchCompare::pixelRectForMargins(const QSize &size)
{
    const int DPI = static_cast<int>(POINTS_PER_INCH *
                (zoomSpinBoxValue / 100.0));
    int top = pixelOffsetForPointValue(DPI, topMarginSpinBoxValue);
    int left = pixelOffsetForPointValue(DPI, leftMarginSpinBoxValue);
    int right = pixelOffsetForPointValue(DPI, rightMarginSpinBoxValue);
    int bottom = pixelOffsetForPointValue(DPI,
            bottomMarginSpinBoxValue);
    return QRect(QPoint(left, top),
                 QPoint(size.width() - right, size.height() - bottom));
}

void BatchCompare::paintOnImage(const QPainterPath &path, QImage *image)
{
    QPen pen_(pen);
    QBrush brush_(brush);
    QColor color = pen.color();
    const qreal Alpha = opacity / 100.0;
    color.setAlphaF(Alpha);
    pen_.setColor(color);
    brush_.setColor(color);

    QPainter painter(image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen_);
    painter.setBrush(brush_);

    const int SQUARE_SIZE = squareSize;
    const double RULE_WIDTH = ruleWidth;
    QRectF rect = path.boundingRect();
    if (rect.width() < SQUARE_SIZE && rect.height() < SQUARE_SIZE) {
        rect.setHeight(SQUARE_SIZE);
        rect.setWidth(SQUARE_SIZE);
        painter.drawRect(rect);
        if (!qFuzzyCompare(RULE_WIDTH, 0.0)) {
            painter.setPen(QPen(pen.color()));
            painter.drawRect(0, rect.y(), RULE_WIDTH, rect.height());
        }
    }
    else {
        QPainterPath path_(path);
        path_.setFillRule(Qt::WindingFill);
        painter.drawPath(path_);
        if (!qFuzzyCompare(RULE_WIDTH, 0.0)) {
            painter.setPen(QPen(pen.color()));
            QList<QPolygonF> polygons = path_.toFillPolygons();
            foreach (const QPolygonF &polygon, polygons) {
                const QRectF rect = polygon.boundingRect();
                painter.drawRect(0, rect.y(), RULE_WIDTH, rect.height());
            }
        }
    }
    painter.end();
}

PdfDocument BatchCompare::getPdf(const QString &filename)
{
    PdfDocument pdf(Poppler::Document::load(filename));
    if (!pdf) {
        _status->setStatusWithDescription(ErrorUnableToLoadFile,
                                          tr("Cannot load '%1'.").arg(filename));
        _notifier->messageBox(tr("Cannot load '%1'.").arg(filename));
    } else if (pdf->isLocked()) {
        _status->setStatusWithDescription(ErrorUnableToLoadFile,
                                          tr("Cannot read a locked PDF ('%1').").arg(filename));
        _notifier->messageBox(tr("Cannot read a locked PDF ('%1').").arg(filename));
#if QT_VERSION >= 0x040600
        pdf.clear();
#else
        pdf.reset();
#endif
    }
    return pdf;
}

BatchCompare::Difference BatchCompare::getTheDifference(PdfPage page1,
                                                    PdfPage page2)
{
    QRectF rect;
    if (marginsGroupBoxChecked)
        rect = pointRectForMargins(page1->pageSize());
    const TextBoxList list1 = getTextBoxes(page1, rect);
    const TextBoxList list2 = getTextBoxes(page2, rect);
    if (list1.count() != list2.count())
        return TextualDifference;
    for (int i = 0; i < list1.count(); ++i)
        if (list1[i]->text() != list2[i]->text())
            return TextualDifference;
//refactor prima di lanciare
    if (currentCompareIndex == CompareAppearance) {
        int x = -1;
        int y = -1;
        int width = -1;
        int height = -1;
        if (marginsGroupBoxChecked)
            computeImageOffsets(page1->pageSize(), &x, &y, &width,
                    &height);
        QImage image1 = page1->renderToImage(POINTS_PER_INCH,
                POINTS_PER_INCH, x, y, width, height);
        QImage image2 = page2->renderToImage(POINTS_PER_INCH,
                POINTS_PER_INCH, x, y, width, height);
        if (image1 != image2)
            return VisualDifference;
    }
    return NoDifference;
}


QRectF BatchCompare::pointRectForMargins(const QSize &size)
{
    return rectForMargins(size.width(), size.height(),
            topMarginSpinBoxValue, bottomMarginSpinBoxValue,
            leftMarginSpinBoxValue, rightMarginSpinBoxValue);
}

void BatchCompare::computeImageOffsets(const QSize &size, int *x, int *y,
        int *width, int *height)
{
    const int DPI = static_cast<int>(POINTS_PER_INCH *
                (zoomSpinBoxValue / 100.0));
    *y = pixelOffsetForPointValue(DPI, topMarginSpinBoxValue);
    *x = pixelOffsetForPointValue(DPI, leftMarginSpinBoxValue);
    *width = pixelOffsetForPointValue(DPI, size.width() -
            (leftMarginSpinBoxValue + rightMarginSpinBoxValue));
    *height = pixelOffsetForPointValue(DPI, size.height() -
            (topMarginSpinBoxValue + bottomMarginSpinBoxValue));
}

void BatchCompare::batchOperation()
{
    PdfDocument pdf1 = getPdf(_startupParameters->file1());
    if (!pdf1) {
        _status->setStatus(ErrorUnableToLoadFile1);
        return;
    }
    PdfDocument pdf2 = getPdf(_startupParameters->file2());
    if (!pdf2) {
        _status->setStatus(ErrorUnableToLoadFile2);
        return;
    }
    _status->setDoc1Info(docInfo(pdf1, _startupParameters->file1()));
    _status->setDoc2Info(docInfo(pdf2,_startupParameters->file2() ));

    CompareResults results;
    comparePagesBatch( _status, results,
                       _startupParameters->file1(), pdf1,
                      _startupParameters->file2(), pdf2);
    if(!_status->isError() && _startupParameters->isCompareFonts() ) {
        compareFonts(_status->doc1Info(), _status->doc2Info());
    }
    if( _status->isError() && _status->isErrorComparing() ) {
        _status->setPagesNotEqualCount(results.count());
        if( _startupParameters->enablePDFDiff() ) {
            saveResultsBatch(_status, results, pdf1, pdf2 ) ;
        }
    }
}

QList<int> BatchCompare::getPageListBatch( const int which, PdfDocument pdf, const int startPageSet)
{
    // Poppler has 0-based page numbers; the UI has 1-based page numbers
    QList<int> pages;

    int startPage = startPageSet-1;
    if( startPage >= pdf->numPages() ) {
        _status->setStatusWithDescription(ErrorInitialPage, tr("file: %1, start page greater than available pages").arg(which) );
        return pages ;
    }
    int count = _startupParameters->pages();
    if( count == StartupParameters::AllPages ) {
        count = pdf->numPages() - startPage ;
    }
    if( (startPage + count ) > pdf->numPages()) {
        _status->setStatusWithDescription(ErrorFinalPage, tr("file: %1, final page is not existing").arg(which) );
        return pages ;
    }
    // 0 -based pages
    for( int index = 0 ; index < count ; index ++ ) {
        pages.append(startPage + index );
    }
    return pages;
}

void BatchCompare::comparePagesBatch(
        Status *status,
        CompareResults &results,
        const QString &filename1, const PdfDocument &pdf1,
        const QString &filename2, const PdfDocument &pdf2)
{
    QList<int> pages1 = getPageListBatch(1, pdf1, _startupParameters->startPage1() );
    if( _status->isError() ) {
        return ;
    }
    QList<int> pages2 = getPageListBatch(2, pdf2,  _startupParameters->startPage1() );
    if( _status->isError() ) {
        return ;
    }
    // check for the same number of pages
    if( pages1.count() != pages2.count() ) {
        _status->setStatusWithDescription( ErrorPagesDiffer, tr("the number of pages is not the same on both the documents, doc1:%1, doc2:%2").arg(pages1.count()).arg(pages2.count()));
    }
    currentCompareIndex = _startupParameters->comparisonMode();
    results.setTotal(qMin(pages1.count(), pages2.count()));
    while (!pages1.isEmpty() && !pages2.isEmpty()) {
        int p1 = pages1.takeFirst();
        PdfPage page1(pdf1->page(p1));
        if (!page1) {
            _status->setStatusWithDescription( ErrorLoadingPage, tr("Failed to read page %1 from '%2'.").arg(p1 + 1).arg(filename1));
            continue;
        }
        int p2 = pages2.takeFirst();
        PdfPage page2(pdf2->page(p2));
        if (!page2) {
            _status->setStatusWithDescription( ErrorLoadingPage, tr("Failed to read page %1 from '%2'.").arg(p2 + 1).arg(filename2));
            continue;
        }
        Difference difference = getTheDifference(page1, page2);
        if (difference != NoDifference) {
            results.differences().append(PagePair(p1, p2, difference == VisualDifference));
            results.incCount();
            status->setStatusWithDescription( ErrorDocDiffer, tr("documents differ at page: %1").arg(p1+1));
        }
    }
}

void BatchCompare::saveResultsBatch(Status *status, CompareResults &results, const PdfDocument &pdf1, const PdfDocument &pdf2)
{
    int start = 0;
    int end = results.count();
    QString header;
    const QChar bullet(0x2022);
    header = tr("%5 %1 %2 vs. %3 %1 %4").arg(bullet)
        .arg(_startupParameters->file1()).arg(_startupParameters->file2())
        .arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(AboutForm::ProgramName);
    saveAsPdfBatch( status, results, _startupParameters->pdfDiffFilePath(), start, end, pdf1, pdf2, header);
}

void BatchCompare::saveAsPdfBatch(Status *status, CompareResults &results,
        const QString &outputFile, const int start, const int end,
        const PdfDocument &pdf1, const PdfDocument &pdf2,
        const QString &header)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(outputFile);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setColorMode(QPrinter::Color);
    printer.setCreator(AboutForm::ProgramName);
    printer.setOrientation(savePages == SaveBothPages
            ? QPrinter::Landscape : QPrinter::Portrait);
    QPainter painter(&printer);
    painter.setRenderHints(QPainter::Antialiasing|
            QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform);
    painter.setFont(QFont("Helvetica", 11));
    painter.setPen(Qt::darkCyan);
    const QRect rect(0, 0, painter.viewport().width(),
                        painter.fontMetrics().height());
    const int y = painter.fontMetrics().lineSpacing();
    const int height = painter.viewport().height() - y;
    const int gap = 30;
    int width = (painter.viewport().width() / 2) - gap;
    if (savePages != SaveBothPages) {
        width = painter.viewport().width();
    }
    const QRect leftRect(0, y, width, height);
    const QRect rightRect(width + gap, y, width, height);
    for (int index = start; index < end; ++index) {
        if (!paintSaveAsBatch(&painter, results, index, pdf1, pdf2, header, rect,
                    leftRect, rightRect)) {
            status->setStatusWithDescriptionUncond(ErrorWritingPDFDiffFile, tr("error while writing differences file"));
            continue;
        }
        if (index + 1 < end)
            printer.newPage();
    }
    if( printer.printerState() == QPrinter::Error ) {
        status->setStatusWithDescriptionUncond(ErrorWritingPDFDiffFile, tr("error while writing differences file"));
    }
}


bool BatchCompare::paintSaveAsBatch(QPainter *painter, CompareResults &results, const int index,
        const PdfDocument &pdf1, const PdfDocument &pdf2,
        const QString &header, const QRect &rect, const QRectF &leftRect,
        const QRectF &rightRect)
{
    PagePair pair = results.differences().at(index);
    if (pair.isNull())
        return false;
    PdfPage page1(pdf1->page(pair.left));
    if (!page1)
        return false;
    PdfPage page2(pdf2->page(pair.right));
    if (!page2)
        return false;
    const QPair<QString, QString> keys = cacheKeys(index, pair);
    const QPair<QPixmap, QPixmap> pixmaps = populatePixmaps(pdf1,
            page1, pdf2, page2, pair.hasVisualDifference,
            keys.first, keys.second);
    if(!header.isEmpty()) {
        painter->drawText(rect, header, QTextOption(Qt::AlignHCenter| Qt::AlignTop));
    }
    if (savePages == SaveBothPages) {
        QRectF rect = resizeRect(leftRect, pixmaps.first.size());
        painter->drawPixmap(rect.toAlignedRect(), pixmaps.first);
        rect = resizeRect(rightRect, pixmaps.second.size());
        painter->drawPixmap(rect.toAlignedRect(), pixmaps.second);
        painter->drawRect(rightRect.adjusted(2.5, 2.5, 2.5, 2.5));
    } else if (savePages == SaveLeftPages) {
        QRectF rect = resizeRect(leftRect, pixmaps.first.size());
        painter->drawPixmap(rect.toAlignedRect(), pixmaps.first);
    } else { // (savePages == SaveRightPages)
        QRectF rect = resizeRect(leftRect, pixmaps.second.size());
        painter->drawPixmap(rect.toAlignedRect(), pixmaps.second);
    }
    painter->drawRect(leftRect.adjusted(2.5, 2.5, 2.5, 2.5));
    return true;
}

const QPair<QPixmap, QPixmap> BatchCompare::populatePixmaps(
        const PdfDocument &pdf1, const PdfPage &page1,
        const PdfDocument &pdf2, const PdfPage &page2,
        bool hasVisualDifference, const QString &key1,
        const QString &key2)
{
    QPixmap pixmap1;
    QPixmap pixmap2;
#if QT_VERSION >= 0x040600
    if (!QPixmapCache::find(key1, &pixmap1) ||
        !QPixmapCache::find(key2, &pixmap2)) {
#else
    if (!QPixmapCache::find(key1, pixmap1) ||
        !QPixmapCache::find(key2, pixmap2)) {
#endif
        _notifier->setOverrideCursor();
        const int DPI = static_cast<int>(POINTS_PER_INCH *
                (zoomSpinBoxValue / 100.0));
        const bool compareText = currentCompareIndex != CompareAppearance;
        QImage plainImage1;
        QImage plainImage2;
        if (hasVisualDifference || !compareText) {
            plainImage1 = page1->renderToImage(DPI, DPI);
            plainImage2 = page2->renderToImage(DPI, DPI);
        }
        pdf1->setRenderHint(Poppler::Document::Antialiasing);
        pdf1->setRenderHint(Poppler::Document::TextAntialiasing);
        pdf2->setRenderHint(Poppler::Document::Antialiasing);
        pdf2->setRenderHint(Poppler::Document::TextAntialiasing);
        QImage image1 = page1->renderToImage(DPI, DPI);
        QImage image2 = page2->renderToImage(DPI, DPI);

        if (compareText || (-1 == static_cast<int>(compositionMode) ) ) {
            QPainterPath highlighted1;
            QPainterPath highlighted2;
            if (hasVisualDifference || !compareText)
                computeVisualHighlights(&highlighted1, &highlighted2,
                        plainImage1, plainImage2);
            else
                computeTextHighlights(&highlighted1, &highlighted2, page1,
                        page2, DPI);
            if (!highlighted1.isEmpty())
                paintOnImage(highlighted1, &image1);
            if (!highlighted2.isEmpty())
                paintOnImage(highlighted2, &image2);
            if (highlighted1.isEmpty() && highlighted2.isEmpty()) {
                QFont font("Helvetica", 14);
                font.setOverline(true);
                font.setUnderline(true);
                highlighted1.addText(DPI / 4, DPI / 4, font,
                    tr("%1: False Positive").arg(AboutForm::ProgramName));
                paintOnImage(highlighted1, &image1);
            }
            pixmap1 = QPixmap::fromImage(image1);
            pixmap2 = QPixmap::fromImage(image2);
        } else {
            pixmap1 = QPixmap::fromImage(image1);
            QImage composed(image1.size(), image1.format());
            QPainter painter(&composed);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillRect(composed.rect(), Qt::transparent);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter.drawImage(0, 0, image1);
            painter.setCompositionMode(compositionMode);
            painter.drawImage(0, 0, image2);
            painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
            painter.fillRect(composed.rect(), Qt::white);
            painter.end();
            pixmap2 = QPixmap::fromImage(composed);
        }
        QPixmapCache::insert(key1, pixmap1);
        QPixmapCache::insert(key2, pixmap2);
        _notifier->setRestoreCursor();
    }
    return qMakePair(pixmap1, pixmap2);
}

DocInfo *BatchCompare::docInfo(PdfDocument pdf, const QString &fileName)
{
    DocInfo *info = new DocInfo();
    info->fileName = QFileInfo(fileName).canonicalPath();
    foreach (const QString &key, pdf->infoKeys()) {
        if (key == "CreationDate" || key == "ModDate")
            continue;
        info->infos.append(qMakePair( key, pdf->info(key)));
    }
    QDateTime created = pdf->date("CreationDate");
    QDateTime modified = pdf->date("ModDate");
    info->modDate = modified.toString();
    info->creationDate = created.toString();
    info->pageCount = pdf->numPages();
    if (info->pageCount > 0) {
        const double PointToMM = 0.3527777777;
        PdfPage page1(pdf->page(0));
        QSize size = page1->pageSize();
        info->pageSize = QString("%1pt x %2pt (%3mm x %4mm)")
                  .arg(size.width()).arg(size.height())
                  .arg(qRound(size.width() * PointToMM))
                  .arg(qRound(size.height() * PointToMM));
    }
    pdf->getPdfVersion(&info->pdfVersionMajor, &info->pdfVersionMinor);
    // get fonts info
    QList<Poppler::FontInfo> fonts = pdf->fonts();
    foreach(Poppler::FontInfo fi, fonts ) {
        LFontInfo *fil = new LFontInfo();
        fil->name = fi.name();
        fil->embedded = fi.isEmbedded();
        fil->subset = fi.isSubset();
        fil->typeName = fi.typeName();
        info->fonts.append(fil);
    }
    return info;
}

void BatchCompare::setNotifier(CompareNotifier *newNotifier)
{
    _notifier = newNotifier;
}


void BatchCompare::writeParam(QXmlStreamWriter & writer, const QString &name, const QString &value)
{
    writer.writeStartElement("item");
    writer.writeAttribute("name", name);
    writer.writeAttribute("value", value);
    writer.writeEndElement();
}

void BatchCompare::writeParam(QXmlStreamWriter & writer, const QString &name, const QVariant &value)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    QDataStream dataStream(&buffer);
    buffer.open(QFile::WriteOnly);
    dataStream << value ;
    buffer.close();
    QString realValue ;
    for( int i = 0 ; i < byteArray.size() ; i ++ ) {
        QString s = QString::number(0x00FF&byteArray.at(i));
        realValue.append(s);
    }
    writer.writeStartElement("item");
    writer.writeAttribute("name", name);
    writer.writeAttribute("value", realValue.toCaseFolded());
    writer.writeEndElement();
}

static QString toBoolString(const bool value)
{
    if(value) {
        return "true";
    }
    return "false";
}

void BatchCompare::writeParameters(QXmlStreamWriter & writer)
{
    writeParam(writer, "CombineTextHighlighting", toBoolString(combineTextHighlighting) );
    writeParam(writer, "ShowHighlight", QString::number(showHighlight) );
    QVariant penVariant(pen);
    writeParam(writer, "Outline", penVariant );
    QVariant fillVariant(brush);
    writeParam(writer, "Fill", fillVariant );
    writeParam(writer, "Zoom", QString::number(zoomSpinBoxValue) );
    writeParam(writer, "Columns", QString::number(columnsSpinBoxValue)  );
    writeParam(writer, "Tolerance/R", QString::number(toleranceRSpinBoxValue) );
    writeParam(writer, "Tolerance/Y", QString::number(toleranceYSpinBoxValue) );
    writeParam(writer, "Zoning/Enable", toBoolString(zoningGroupBoxChecked) );
    writeParam(writer, "Margins/Exclude", toBoolString(marginsGroupBoxChecked) );
    writeParam(writer, "Margins/Left", QString::number(leftMarginSpinBoxValue) );
    writeParam(writer, "Margins/Right", QString::number(rightMarginSpinBoxValue) );
    writeParam(writer, "Margins/Top", QString::number(topMarginSpinBoxValue) );
    writeParam(writer, "Margins/Bottom", QString::number(bottomMarginSpinBoxValue) );
    writeParam(writer, "CacheSizeMB", QString::number(cacheSizeMB) );
    writeParam(writer, "compositionMode", QString::number(compositionMode) );

    writeParam(writer, "SquareSize", QString::number(squareSize) );
    writeParam(writer, "RuleWidth", QString::number(ruleWidth) );

    writeParam(writer, "Overlap", QString::number(overlap) );
    writeParam(writer, "CombineTextHighlighting", toBoolString(combineTextHighlighting));
    writeParam(writer, "Opacity", QString::number(opacity) );
}

QString BatchCompare::makeFontKey(LFontInfo *l)
{
    QString key = QString("%1:%2:%3:%4").arg(l->name).arg(l->typeName).arg(l->embedded).arg(l->subset);
    return key;
}

QString BatchCompare::makeFontInfoString(LFontInfo *l)
{
    QString key = QString("name:%1, type:%2, embedded:%3, subset:%4").arg(l->name).arg(l->typeName).arg(l->embedded).arg(l->subset);
    return key;
}

bool BatchCompare::compareFonts(DocInfo *doc1, DocInfo *doc2)
{
    bool error = false ;
    QMultiHash<QString,LFontInfo*> fontsDoc2;
    foreach( LFontInfo* f2, doc2->fonts ) {
        fontsDoc2.insertMulti(makeFontKey(f2), f2);
    }

    foreach( LFontInfo* f1, doc1->fonts ) {
        QString keyFont1 = makeFontKey(f1);
        if( fontsDoc2.contains(keyFont1)) {
            fontsDoc2.remove(keyFont1, fontsDoc2.values(keyFont1).first());
        } else {
            _status->setStatusWithDescription(ErrorFontsDiffer, QString("Doc 2 is missing font:%1").arg(makeFontInfoString(f1)));
            error = true ;
        }
    }
    // check for
    if( fontsDoc2.keys().size()>0 ) {
        LFontInfo *missing = fontsDoc2.values().first();
        _status->setStatusWithDescription(ErrorFontsDiffer, QString("Doc 1 is missing font:%1").arg(makeFontInfoString(missing)));
        error = true ;
    }
    return !error;
}
