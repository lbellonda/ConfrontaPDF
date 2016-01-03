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


#ifndef BATCHCOMPARE_H
#define BATCHCOMPARE_H

#include "generic.hpp"
#include "saveform.hpp"
#if QT_VERSION >= 0x040600
#include <QSharedPointer>
#else
#include <tr1/memory>
#endif
#if defined(USE_QT5)
   #include <poppler-qt5.h>
#else
    #include <poppler-qt4.h>
#endif
#include <QBrush>
#include <QList>
#include <QPen>
#include <QPainter>
#include "startupparameters.h"
#include "status.h"
#include "compareresults.h"


class CompareNotifier {
public:
    virtual void writeError(const QString &text)= 0 ;
    virtual void writeLine(const QString &text)=0;
    virtual void setOverrideCursor()=0;
    virtual void setRestoreCursor()=0;
    virtual void processEvents()=0;
    virtual void setStatusLabel(const QString &text)=0;
    virtual void messageBox(const QString &text)=0;
};

class BatchCompare : public QObject
{
    Q_OBJECT
public:
    BatchCompare(const Debug debug,
            const InitialComparisonMode comparisonMode,
            const QString &filename1, const QString &filename2,
                 StartupParameters *startupParameters,
            Status *status, QWidget *parent=0);
    void batchOperation();
    void readFromSettings();
    void writeToSettings(const QString &filePath);
protected:
    QString finalFileName(const QString &filename);
    DocInfo *docInfo(PdfDocument pdf, const QString &fileName);
    void initValues();

private:
    enum Difference {NoDifference, TextualDifference, VisualDifference};

    PdfDocument getPdf(const QString &filename);
    Difference getTheDifference(PdfPage page1, PdfPage page2);
    void paintOnImage(const QPainterPath &path, QImage *image);
    void computeTextHighlights(QPainterPath *highlighted1,
            QPainterPath *highlighted2, const PdfPage &page1,
            const PdfPage &page2, const int DPI);
    void computeVisualHighlights(QPainterPath *highlighted1,
        QPainterPath *highlighted2, const QImage &plainImage1,
        const QImage &plainImage2);
    void addHighlighting(QRectF *bigRect, QPainterPath *highlighted,
            const QRectF wordOrCharRect, const int OVERLAP, const int DPI,
            const bool COMBINE=true);
    const QPair<QString, QString> cacheKeys(const int index,
            const PagePair &pair) const;
    const TextBoxList zoneYxOrdered(const TextBoxList &list);
    void showZones(const int Width, const TextBoxList &list,
            QLabel *label);
    void showMargins(QLabel *label);
    void saveAsPdf(const int start, const int end, const PdfDocument &pdf1,
            const PdfDocument &pdf2, const QString &header);
    bool paintSaveAs(QPainter *painter, const int index,
            const PdfDocument &pdf1, const PdfDocument &pdf2,
            const QString &header, const QRectF &rect,
            const QRectF &leftRect, const QRectF &rightRect);
    void saveAsImages(const int start, const int end,
            const PdfDocument &pdf1, const PdfDocument &pdf2,
            const QString &header);
    void computeImageOffsets(const QSize &size, int *x, int *y,
            int *width, int *height);
    QRectF pointRectForMargins(const QSize &size);
    QRect pixelRectForMargins(const QSize &size);

    // batch operations
    QList<int> getPageListBatch( const int which, PdfDocument pdf, const int startPageSet);
    void comparePagesBatch(
            Status *status,
            CompareResults &results,
            const QString &filename1, const PdfDocument &pdf1,
            const QString &filename2, const PdfDocument &pdf2);
    void saveResultsBatch(Status *status, CompareResults &results, const PdfDocument &pdf1, const PdfDocument &pdf2);
    void saveAsPdfBatch(Status *status, CompareResults &results, const QString &outputFile, const int start, const int end,
            const PdfDocument &pdf1, const PdfDocument &pdf2,
            const QString &header);
    const QPair<QString, QString> cacheKeysBatch(const int index, const PagePair &pair) const;
    const QPair<QPixmap, QPixmap> populatePixmaps(
            const PdfDocument &pdf1, const PdfPage &page1,
            const PdfDocument &pdf2, const PdfPage &page2,
            bool hasVisualDifference, const QString &key1,
            const QString &key2);
    bool paintSaveAsBatch(QPainter *painter, CompareResults &results, const int index,
            const PdfDocument &pdf1, const PdfDocument &pdf2,
            const QString &header, const QRect &rect, const QRectF &leftRect,
            const QRectF &rightRect);
    void writeParam(QXmlStreamWriter &writer, const QString &name, const QString &value);
    void writeParam(QXmlStreamWriter & writer, const QString &name, const QVariant &value);


    QBrush brush;
    QPen pen;
    SavePages savePages;
    Debug debug;
    StartupParameters *_startupParameters;
    Status *_status;
    CompareNotifier *_notifier;

    //---------------
public:
    int currentCompareIndex;
    bool zoningGroupBoxChecked;
    int columnsSpinBoxValue;
    int toleranceRSpinBoxValue;
    int toleranceYSpinBoxValue;

    bool marginsGroupBoxChecked;
    int topMarginSpinBoxValue;
    int bottomMarginSpinBoxValue;
    int leftMarginSpinBoxValue;
    int rightMarginSpinBoxValue;
    int cacheSizeMB;

    int zoomSpinBoxValue;
    QString filename1;
    QString filename2;
    int showHighlight;
    QString pages1LineEdit, pages2LineEdit;
    // -1 for highlight, any other value: composition
    QPainter::CompositionMode compositionMode;
    int squareSize;
    double ruleWidth;
    int overlap;
    bool combineTextHighlighting;
    int opacity;

    void setNotifier(CompareNotifier *newNotifier);
    void writeParameters(QXmlStreamWriter &writer);
};

#endif // BATCHCOMPARE_H
