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
#include "editor.h"
#include "ui_editor.h"

Editor::Editor(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Editor)
{
    ui->setupUi(this);

    setupMenu();

    setupEditor();
    setupErrorLog();

    setWindowTitle(tr("BRL-CAD editor"));
    ui->tabRt->hide();
    ui->dockLogs->hide();
    ui->dockProperties->hide();

#ifdef _WIN32
    BRLCADbinPath = "./";
#else
    BRLCADbinPath = "/usr/brlcad/bin/";
#endif

    CppAvailable = false;


    connect(ui->dockRaytrace, SIGNAL(allowedAreasChanged(Qt::DockWidgetAreas)), this, SLOT(refreshRtImage()));

    if(CppAvailable) {
        ui->EditorsTabs->installEventFilter(this);
        ui->editorCpp->installEventFilter(this); }
    ui->editorAscii->installEventFilter(this);
    ui->lbImage->installEventFilter(this);

    //sliders and mouse event

    connect(ui->sliderRtAzimut, SIGNAL(valueChanged(int)), this, SLOT(setRtAzimut(int)));
    connect(ui->sliderRtElevation, SIGNAL(valueChanged(int)), this, SLOT(setRtElevation(int)));

    this->showMaximized();
}

bool Editor::eventFilter(QObject *watched, QEvent *event)
{
    if (watched != 0 && event->type() == QEvent::Resize) refreshRtImage();
    return false;
}

void Editor::refreshRtImage()
{
    int maxImageSize = qMin(qMin(ui->lbImage->width(), ui->lbImage->height()), qMin(this->centralWidget()->height(), this->width())) - 5;
    rtImageScaled = new QPixmap(rtImage->scaled(maxImageSize, maxImageSize));

    ui->lbImage->setPixmap(*rtImageScaled);
}

//********************************************************************************************************************************************************************************************************
//************************************************************************************************ Setups ************************************************************************************************
//********************************************************************************************************************************************************************************************************

void Editor::setupMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), this, SLOT(newFile()), QKeySequence::New);
    fileMenu->addAction(tr("&Open"), this, SLOT(openFile()), QKeySequence::Open);
    fileMenu->addAction(tr("&Save"), this, SLOT(saveFile()), QKeySequence::Save);
    fileMenu->addAction(tr("&Save As"), this, SLOT(saveFileAs()), QKeySequence::SaveAs);
    fileMenu->addAction(tr("&Export"), this, SLOT(exportAs()), QKeySequence(tr("Ctrl+E")));
    fileMenu->addAction(tr("&Import"), this, SLOT(importAs()), QKeySequence(tr("Ctrl+I")));
    fileMenu->addAction(tr("&Ratetrace"), this, SLOT(raterace()), QKeySequence(tr("F5")));

    QMenu *optionsMenu = new QMenu(tr("&Options"), this);
    menuBar()->addMenu(optionsMenu);

    optionsMenu->addAction(tr("&Properties"), this, SLOT(about()));
    optionsMenu->addAction(tr("&Shortcuts"), this, SLOT(about()));

    QMenu *windowMenu = new QMenu(tr("&Window"), this);
    menuBar()->addMenu(windowMenu);

    windowMenu->addAction(tr("Show &Logs && Errors"), ui->dockLogs, SLOT(show()));
    windowMenu->addAction(tr("Show &Properties"), ui->dockProperties, SLOT(show()));
    windowMenu->addAction(tr("Show &Raytrace"), ui->dockRaytrace, SLOT(show()));

    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, SLOT(about()));
    helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
}

void Editor::setupRtImage()
{
    rtImage = new QPixmap(currentPathPixmap);
    refreshRtImage();
}

void Editor::setupEditor()
{
    QFont font;
    font.setFamily("Courier New");
    font.setFixedPitch(true);
    font.setPointSize(12);

    ui->editorCpp->setFont(font);
    ui->editorAscii->setFont(font);

    highlightercpp = new HighlighterCpp(ui->editorCpp->document());
    highlighterascii = new HighlighterAscii(ui->editorAscii->document());
}

void Editor::setupErrorLog()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->errorLog->setFont(font);
}

//***************************************************************************************************-*****************************************************************************************************
//******************************************************************************************** File operations ********************************************************************************************
//*********************************************************************************************************************************************************************************************************

void Editor::newFile()
{
    if(ui->editorCpp->document()->toPlainText() != "") saveFile();
    currentPath = "";
    currentFileName = "";
    currentFileNameExportedAsc = "";
    currentPathPixmap = "";
    ui->editorCpp->clear();
}

void Editor::openFile(QString path)
{
    if(CppAvailable) { if(ui->editorCpp->document()->toPlainText() != "") saveFile(); }
    else { if(ui->editorAscii->document()->toPlainText() != "") saveFile(); }

    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C++ Files (*.cpp *.h);;ASCII Files (*.asc);;G database (*.g)");

    if (!fileName.isEmpty())
    {
        currentPath = fileName;
        QFileInfo info(fileName);

        if (info.suffix() != "cpp") openFileAscOrG();
        else
        {
            CppAvailable = true;
            setCentralWidget(ui->EditorsTabs);

            QFile file(fileName);

            if (file.open(QFile::ReadOnly | QFile::Text))
                ui->editorCpp->setPlainText(file.readAll());
            ui->EditorsTabs->setTabText(0, file.fileName());
            file.close();
        }
    }
}

void Editor::openFileAscOrG()
{
    CppAvailable = false;
    QFileInfo info(currentPath);
    if(info.suffix() == "g") system( (BRLCADbinPath + "g2asc " + currentPath + " " + info.absoluteDir().absolutePath() + "/" + info.baseName() + ".asc").toStdString().c_str() );
    if(info.suffix() == "asc") system( (BRLCADbinPath + "asc2g " + currentPath + " " + info.absoluteDir().absolutePath() + "/" + info.baseName() + ".g").toStdString().c_str() );

    currentPath = info.absoluteDir().absolutePath() + "/" + info.baseName() + ".asc";

    QFile ascFile(currentPath);
    ascFile.open(QFile::ReadOnly | QFile::Text);
    ui->editorAscii->setPlainText(ascFile.readAll());
    ascFile.close();

    setCentralWidget(ui->editorAscii);
}

void Editor::saveFile()
{
    bool newfile = false;
    if(currentPath.isEmpty()) { currentPath = QFileDialog::getSaveFileName(this, tr("Open File"), "", "C++ Files (*.cpp *.h);;ASCII Files (*.asc);;G files (*.g)"); newfile = true; }
    if(currentPath.isEmpty()) return;

    QFile file(currentPath);
    file.open(QIODevice::ReadWrite);
    file.resize(0);
    QTextStream fs(&file);

    if(newfile) CppAvailable = (QFileInfo(currentFileName).suffix() == "cpp");

    if(CppAvailable) fs << ui->editorCpp->document()->toPlainText();
    else fs << ui->editorAscii->document()->toPlainText();
    file.close();

    if(!CppAvailable && newfile) openFileAscOrG();

}

void Editor::saveFileAs()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Open File"), "", "C++ Files (*.cpp *.h)");

    QFile file(path);
    file.open(QIODevice::ReadWrite);
    QTextStream fs(&file);

    if(CppAvailable) fs << ui->editorCpp->document()->toPlainText();
    else fs << ui->editorAscii->document()->toPlainText();

    file.close();
}

void Editor::setRtAzimut(int i)
{
    justRatetrace();
}

void Editor::setRtElevation(int i)
{
    justRatetrace();
}

void Editor::justRatetrace()
{
    QFileInfo info(currentPath);
    QString FileName = info.absoluteDir().absolutePath() + "/" + info.baseName(); // name without extend

    system( (BRLCADbinPath + "rt -e " + QString::number(ui->sliderRtElevation->value()) + " -a " + QString::number(ui->sliderRtAzimut->value()) + " " + ui->leOptions->text() + " -o " + FileName + ".pix " + FileName + ".g " + ui->leObjects->text()).toStdString().c_str() );
    system( ("chmod +rw " + FileName + ".pix").toStdString().c_str() ); //just for linux
    system( (BRLCADbinPath + "pix-png " + FileName + ".pix > " + FileName + ".png").toStdString().c_str() );

    currentPathPixmap = FileName + ".png";

    setupRtImage();
}

void Editor::raterace()
{
    saveFile();
    if(currentPath.isNull()) return;
    std::cout << "rt" << CppAvailable << std::endl << std::endl;

    //spliting extension
    QFileInfo info(currentPath);
    QString FileName = info.absoluteDir().absolutePath() + "/" + info.baseName(); // name without extend
    QString FileNameExportedAsc = info.baseName() + ".asc"; // name without extend

    if(!CppAvailable)
    {
        system( (BRLCADbinPath + "asc2g " + FileName + ".asc " + FileName + ".g").toStdString().c_str() );

        currentPathPixmap = FileName + ".png";
        justRatetrace();
        return;
    }


    //compiling and running cpp
    if(CppAvailable)
    {
        if(compiler == "") compiler = "g++";
        system( (compiler + " -o " + FileName + " " + currentPath + " " + additionalFiles).toStdString().c_str() );
        system( (FileName).toStdString().c_str() );
        system(("mv " + FileNameExportedAsc + " " + FileName + ".asc").toStdString().c_str());
    }

    //convering asc to g to pix to png

    system( (BRLCADbinPath + "asc2g " + FileName + ".asc " + FileName + ".g").toStdString().c_str()); // /usr/brlcad/bin/asc2g

//    currentPathPixmap = FileName + ".png";

    justRatetrace();

    QFile ascFile(FileName + ".asc");
    ascFile.open(QFile::ReadOnly | QFile::Text);
    ui->editorAscii->setPlainText(ascFile.readAll());
    ascFile.close();
}

//***************************************************************************************************-*****************************************************************************************************
//********************************************************************************************* Export Import *********************************************************************************************
//*********************************************************************************************************************************************************************************************************

void Editor::exportAs(QString fileName)
{
    QFileInfo currentInfo(currentPath);
    QString currentFileName = currentInfo.absoluteDir().absolutePath() + "/" + currentInfo.baseName() + ".g"; // name without extend


    QString exportFileName;
    if(fileName != "") exportFileName = fileName;
    else exportFileName = QFileDialog::getSaveFileName(this, tr("Export File"), "",					//"asc (*.asc)"
                                                                                                    //";;acad (*.acad)"
                                                                                                    //";;dot (*.dot)"
                                                                                  "dxf (*.dxf)"
                                                                                                    //";;egg (*.egg)"
                                                                                                    //";;euclid (*.euclid)"
                                                                                                    //";;euclid1 (*.euclid1)"
                                                                                  //";;iges (*.iges)"
                                                                                                    //";;jack (*.jack)"
                                                                                                    //";;nff (*.nff)"
                                                                                                    //";;nmg (*.nmg)"
                                                                                                    //";;obj (*.obj)"
                                                                                                    //";;off (*.off)"
                                                                                                    //";;ply (*.ply)"
                                                                                                    //";;raw (*.raw)"
                                                                                                    //";;shellrect (*.ShellRect)"
                                                                                  ";;step (*.step)"
                                                                                  ";;stl (*.stl)"
                                                                                                    //";;tankill (*.tankill)"
                                                                                                    //";;var (*.var)"
                                                                                                    //";;voxel (*.voxel)"
                                                                                                    //";;vrml (*.vrml)"
                                                                                                    //";;x3d (*.x3d)"
                                                       );

    QFileInfo exportInfo(exportFileName);
    QString fileType = exportInfo.suffix().toLower();
    QString command;

    if(fileType == "step")
        command = BRLCADbinPath + "g-step -o " + exportFileName + " " + currentFileName;
    else if(fileType == "stl")
    {
        DialogExportSTL dialog;
        dialog.setModal(true);

        exportImportOptions = "";
        connect(&dialog, SIGNAL(optionsSet(QString)), this, SLOT(newExportImportOptions(QString)));

        dialog.exec();

        while(exportImportOptions == "");
        if(exportImportOptions == "rejected") return;

        command = BRLCADbinPath + "g-stl " + exportImportOptions + " -o " + exportFileName + " " + currentFileName + " " + ui->leObjects->text() + " &";
        exportImportOptions = "";
    }
    else if(fileType == "dxf")
    {
        DialogExportDXF dialog;
        dialog.setModal(true);

        exportImportOptions = "";
        connect(&dialog, SIGNAL(optionsSet(QString)), this, SLOT(newExportImportOptions(QString)));

        dialog.exec();

        while(exportImportOptions == "");
        if(exportImportOptions == "rejected") return;

        command = BRLCADbinPath + "g-dxf " + exportImportOptions + " -o " + exportFileName + " " + currentFileName + " " + ui->leObjects->text() + " &";
        exportImportOptions = "";
    }

    std::cout << "\n\n\n" << command.toStdString() << "\n\n\n" << std::flush;

    system(command.toStdString().c_str());

    //global option to edit specific options just in line (this can be used for not so requently used formats)
    //system(QInputDialog::getText(this, "Export Options", "", QLineEdit::Normal, (BRLCADbinPath + exportInfo.suffix().toLower() + "-g -o" + exportFileName + " " + currentFileName + ".g " + ui->leObjects->text())).toStdString().c_str());
}

void Editor::importAs(QString fileName)
{
    QFileInfo currentInfo(currentPath);
    QString currentDir = currentInfo.absoluteDir().absolutePath() + "/"; // name without extend

    QString importFileName;
    if(!fileName.isEmpty()) importFileName = fileName;
    else importFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                                                                         //";;3dm (*.3dm)"
                                                                                                         //";;comgeom (*.comgeom)"
                                                                                                         //";;vg (*.vg)"
                                                                                                         //";;cy (*.cy)"
                                                                                                         //";;dem (*.dem)"
                                                                                     "dxf (*.dxf)"
                                                                                                         //";;enf (*.enf)"
                                                                                                         //";;euclid (*.euclid)"
                                                                                                         //";;fast4 (*.fast4)"
                                                                                     //";;iges (*.iges)"
                                                                                                         //";;jack (*.jack)"
                                                                                                         //";;k (*.k)"
                                                                                                         //";;nastran (*.nastran)"
                                                                                                         //";;obj (*.obj)"
                                                                                                         //";;off (*.off)"
                                                                                                         //";;patch (*.patch)"
                                                                                                         //";;ply (*.ply)"
                                                                                                         //";;proe (*.proe)"
                                                                                                         //";;raw (*.raw)"
                                                                                                         //";;shp (*.shp)"
                                                                                     ";;step (*.step)"
                                                                                     ";;stl (*.stl)"
                                                                                                         //";;tankill (*.tankill)"
                                                                                                         //";;tgf (*.tgf)"
                                                                                                         //";;viewpoint (*.viewpoint)"
                                                          );

    QFileInfo importInfo(importFileName);
    QString fileType = importInfo.suffix();
    QString command;
    QStringList splitOptions; //just used for 2D imports


    if(fileType == "step")
        command = BRLCADbinPath + "step-g -o " + importFileName + " tmp.g";
    else if(fileType == "stl")
    {
        DialogImportSTL dialog;
        dialog.setModal(true);

        exportImportOptions = "";
        connect(&dialog, SIGNAL(optionsSet(QString)), this, SLOT(newExportImportOptions(QString)));

        dialog.exec();

        while(exportImportOptions == "");
        if(exportImportOptions == "rejected") return;

        command = BRLCADbinPath + "stl-g " + exportImportOptions + " " + importFileName + " " + currentDir + "tmp.g";
        QMessageBox::information(this, "info", command);
    }
    else if(fileType == "dxf")
    {
        DialogImportDXF dialog;
        dialog.setModal(true);

        exportImportOptions = "";
        connect(&dialog, SIGNAL(optionsSet(QString)), this, SLOT(newExportImportOptions(QString)));

        dialog.exec();

        while(exportImportOptions == "");
        if(exportImportOptions == "rejected") return;

        command = BRLCADbinPath + "dxf-g " + exportImportOptions.split(';')[0] + " " + importFileName + " " + currentDir + "tmp.g";
        splitOptions = exportImportOptions.split(';');
        exportImportOptions = "";
    }
    else system((BRLCADbinPath + importInfo.suffix().toLower() + "-g " + importFileName + " -o " + currentDir + "tmp.g").toStdString().c_str() );

    system(("rm " + currentDir + "tmp.g").toStdString().c_str() );
    system(("rm " + currentDir + "tmp.asc").toStdString().c_str() );

    system(command.toStdString().c_str() );

    system((BRLCADbinPath + "g2asc " + currentDir + "tmp.g " + currentDir + "tmp.asc").toStdString().c_str() );

    QFile ascImport(currentDir + "tmp.asc");
    ascImport.open(QFile::ReadOnly);

    QString ascii(ascImport.readAll()), combination;

    if(fileType == "dxf" && splitOptions.length() == 3)
    {
        for(int i = 1; ascii.indexOf("sketch." + QString::number(i)) != -1; i++)
        {
            ascii += "\n put {" + splitOptions[2] + "part" + QString::number(i) + ".s} extrude V {0 0 0} H {0 0 " + splitOptions[1] + "} A {1 0 0} B {0 1 0} S sketch." + QString::number(i);
            if(i == 1) combination = " { l " + splitOptions[2] + "part" + QString::number(i) + ".s }";
            else combination = "{u " + combination + " { l " + splitOptions[2] + "part" + QString::number(i) + ".s }}";
        }
        ascii += "\nput {" + splitOptions[2] + ".r} comb region no tree " + combination;
    }

    if(CppAvailable) ui->editorCpp->setPlainText(ui->editorCpp->document()->toPlainText() + "\n Manual(\"" + ascii.replace("\n", "\"\n\"") + "\");"); /*Please place this text to some function*/
    else                   ui->editorAscii->setPlainText(ui->editorAscii->document()->toPlainText() + "\n" + ascii);
    ascImport.close();

    system("rm tmp.g");
    system("rm tmp.asc");

}
















