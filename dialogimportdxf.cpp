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
#include "dialogimportdxf.h"
#include "ui_dialogimportdxf.h"
#include <QMessageBox>

DialogImportDXF::DialogImportDXF(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImportDXF)
{
    ui->setupUi(this);
    options = "";
}

DialogImportDXF::~DialogImportDXF()
{
    if(options.isEmpty()) emit optionsSet("rejected");
    delete ui;
}

void DialogImportDXF::on_buttonBox_accepted()
{
    if(ui->color->isChecked()) options += "-c ";
    if(ui->verbose->isChecked()) options += "-v ";
    if(ui->debug->isChecked()) options += "-d ";

    if(ui->tolerance->value() != 0) options += " -t " + QString::number(ui->tolerance->value()) + " ";
    if(ui->scale->value() != 0)		options += " -s " + QString::number(ui->scale->value()) + " ";

    if(ui->extrude->value() != 0)
    {
        if(!ui->name->text().isEmpty()) options += ";" + QString::number(ui->extrude->value()) + ";" + ui->name->text();
        else QMessageBox::warning(this, "No name", "You do not have any name entered\nWithout name you can not extrude");
    }

    QMessageBox::about(this, "options", options);

    emit optionsSet(options);
    this->close();
}

void DialogImportDXF::on_buttonBox_rejected()
{
    emit optionsSet("rejected");
    options = "foo"; //does not send "rejeced" when exiting (see destructor)
    this->close();
}
