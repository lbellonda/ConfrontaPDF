#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
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
#include <QMainWindow>
#include <QPen>
#include "startupparameters.h"
#include "status.h"
#include "compareresults.h"
#include "batchcompare.h"

class AboutForm;
class HelpForm;
class Label;
class LineEdit;
class QBoxLayout;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
class QRadioButton;
class QScrollArea;
class QSpinBox;
class QSplitter;


class MainWindow : public QMainWindow, CompareNotifier
{
    Q_OBJECT

public:
    MainWindow(const Debug debug,
            const InitialComparisonMode comparisonMode,
            const QString &filename1, const QString &filename2,
            const QString &language, StartupParameters *startupParameters,
            Status *status, QWidget *parent=0);
    void setOverrideCursor();
    void setRestoreCursor();
    void processEvents();
    void setStatusLabel(const QString &text);
    void messageBox(const QString &text);

protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *object, QEvent *event);
    QString finalFileName(const QString &filename);
    DocInfo *docInfo(PdfDocument pdf, const QString &fileName);
    //void initCompareParams(BatchCompare &compare);

private slots:
    void setFile1(QString filename=QString());
    void setFile2(QString filename=QString());
    void setFiles1(const QStringList &filenames);
    void setFiles2(const QStringList &filenames);
    void compare();
    void options();
    void save();
    void about();
    void help();
    void initialize(const QString &filename1, const QString &filename2);
    void updateUi();
    void updateViews(int index=-1);
    void controlDockLocationChanged(Qt::DockWidgetArea area);
    void actionDockLocationChanged(Qt::DockWidgetArea area);
    void zoningDockLocationChanged(Qt::DockWidgetArea area);
    void marginsDockLocationChanged(Qt::DockWidgetArea area);
    void controlTopLevelChanged(bool floating);
    void actionTopLevelChanged(bool floating);
    void zoningTopLevelChanged(bool floating);
    void marginsTopLevelChanged(bool floating);
    void logTopLevelChanged(bool floating);
    void previousPages();
    void nextPages();
    void showZones();
    void showMargins();
    void setAMargin(const QPoint &pos);

private:
    enum Difference {NoDifference, TextualDifference, VisualDifference};

    void createWidgets(const QString &filename1, const QString &filename2);
    void createCentralArea();
    void createDockWidgets();
    void createConnections();
    const QPair<int, int> comparePages(const QString &filename1,
            const PdfDocument &pdf1, const QString &filename2,
            const PdfDocument &pdf2);
    void comparePrepareUi();
    void compareUpdateUi(const QPair<int, int> &pair, const int millisec);
    int writeFileInfo(const QString &filename);
    void writeLine(const QString &text);
    void writeError(const QString &text);
    PdfDocument getPdf(const QString &filename);
    QList<int> getPageList(int which, PdfDocument pdf);
    Difference getTheDifference(PdfPage page1, PdfPage page2);
    void paintOnImage(const QPainterPath &path, QImage *image);
    const QPair<QPixmap, QPixmap> populatePixmaps(const PdfDocument &pdf1,
            const PdfPage &page1, const PdfDocument &pdf2,
            const PdfPage &page2, bool hasVisualDifference,
            const QString &key1, const QString &key2);
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

    QPushButton *setFile1Button;
    LineEdit *filename1LineEdit;
    QLabel *comparePages1Label;
    QLineEdit *pages1LineEdit;
    Label *page1Label;
    QScrollArea *area1;
    QPushButton *setFile2Button;
    LineEdit *filename2LineEdit;
    QLabel *comparePages2Label;
    QLineEdit *pages2LineEdit;
    Label *page2Label;
    QScrollArea *area2;
    QComboBox *compareComboBox;
    QLabel *compareLabel;
    QLabel *viewDiffLabel;
    QPushButton *compareButton;
    QComboBox *viewDiffComboBox;
    QPushButton *previousButton;
    QPushButton *nextButton;
    QLabel *statusLabel;
    QLabel *zoomLabel;
    QSpinBox *zoomSpinBox;
    QLabel *showLabel;
    QComboBox *showComboBox;
    QPushButton *optionsButton;
    QPushButton *saveButton;
    QPushButton *aboutButton;
    QPushButton *helpButton;
    QPushButton *quitButton;
    QPlainTextEdit *logEdit;
    QSplitter *splitter;
    QBoxLayout *controlLayout;
    QDockWidget *controlDockWidget;
    QBoxLayout *actionLayout;
    QDockWidget *actionDockWidget;
    QDockWidget *logDockWidget;
    QBoxLayout *compareLayout;
    QGroupBox *zoningGroupBox;
    QLabel *columnsLabel;
    QSpinBox *columnsSpinBox;
    QLabel *toleranceRLabel;
    QSpinBox *toleranceRSpinBox;
    QLabel *toleranceYLabel;
    QSpinBox *toleranceYSpinBox;
    QCheckBox *showZonesCheckBox;
    QBoxLayout *zoningLayout;
    QDockWidget *zoningDockWidget;
    QGroupBox *marginsGroupBox;
    QLabel *topMarginLabel;
    QSpinBox *topMarginSpinBox;
    QLabel *bottomMarginLabel;
    QSpinBox *bottomMarginSpinBox;
    QLabel *leftMarginLabel;
    QSpinBox *leftMarginSpinBox;
    QLabel *rightMarginLabel;
    QSpinBox *rightMarginSpinBox;
    QBoxLayout *marginsLayout;
    QDockWidget *marginsDockWidget;

    QBrush brush;
    QPen pen;
    QString currentPath;
    Qt::DockWidgetArea controlDockArea;
    Qt::DockWidgetArea actionDockArea;
    Qt::DockWidgetArea marginsDockArea;
    Qt::DockWidgetArea zoningDockArea;
    Qt::DockWidgetArea logDockArea;
    bool cancel;
    bool showToolTips;
    bool combineTextHighlighting;
    QString saveFilename;
    bool saveAll;
    SavePages savePages;
    const QString language;
    Debug debug;
    AboutForm *aboutForm;
    HelpForm *helpForm;
    StartupParameters *_startupParameters;
    Status *_status;
    int currentCompareIndex;
    int currentShowCompareIndex;
};

#endif // MAINWINDOW_HPP

