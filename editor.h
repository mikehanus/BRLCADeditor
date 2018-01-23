/* Copyright 2018 Michal Hanus
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef BRLCAD_EDITOR_H
#define BRLCAD_EDITOR_H

#include "highlightercpp.h"
#include "highlighterascii.h"
#include "dialogexportstl.h"
#include "dialogexportdxf.h"
#include "dialogimportdxf.h"
#include "dialogimportstl.h"

#include <QMainWindow>
#include <QtWidgets>
#include <iostream>
#include <stdio.h>

namespace Ui {
class Editor;
}

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    Editor(QWidget *parent = 0);
    void setBRLCADbinPath(QString path) { BRLCADbinPath = path; }

public slots:
    void about() { QMessageBox::about(this, tr("About BRL-CAD editor"), tr("<p>The <b>BRL-CAD editr</b> is OpenSCAD like alfa version of C++ script editor</p>")); }
    void newFile();
    void saveFile();
    void saveFileAs();
    void justRatetrace();
    void raterace();
    void openFile(QString path = QString());
    void openFileAscOrG();
    void refreshRtImage();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void exportAs(QString fileName = "");
    void importAs(QString fileName = "");

    void newExportImportOptions(QString newOptions) { exportImportOptions = newOptions; }
    void setRtAzimut(int i);
    void setRtElevation(int i);

private:
    Ui::Editor * ui;
    void setupEditor();
    void setupMenu();
    void setupRtImage();
    void setupErrorLog();

    //QTextEdit *editor;

    bool CppAvailable;

    HighlighterCpp *highlightercpp;
    HighlighterAscii *highlighterascii;

    QSignalMapper importMapper;
    QSignalMapper exportMapper;

    QPixmap * rtImage;
    QPixmap * rtImageScaled;
    QAction *fullScreenImage;
    int i;
    QString currentPath;
    QString currentPathPixmap;
    QString currentFileName; // name without extend
    QString currentFileNameExportedAsc; // name without extend

    QString compiler;
    QString additionalFiles;
    QString BRLCADbinPath;

    QString exportImportOptions;

    int rtAzimut, rtElevation;
};

#endif // EDITOR_H
