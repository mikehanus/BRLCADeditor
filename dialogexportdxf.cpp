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
#include "dialogexportdxf.h"
#include "ui_dialogexportdxf.h"

DialogExportDXF::DialogExportDXF(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExportDXF)
{
    ui->setupUi(this);
}

DialogExportDXF::~DialogExportDXF()
{
    delete ui;
}

void DialogExportDXF::on_buttonBox_accepted()
{
    QString options;

    if(ui->inches->isChecked()) options += "-i ";
    if(ui->verbose->isChecked()) options += "-v ";
    if(ui->polyface->isChecked()) options += "-p ";

    if(ui->rt->value() != 0) options += "-x " + QString::number(ui->rt->value());
    if(ui->nmg->value() != 0) options += "-X " + QString::number(ui->nmg->value());

    if(ui->absolute->value() != 0) 		options += " -a " + QString::number(ui->absolute->value()) + " ";
    if(ui->relative->value() != 0) 		options += " -r " + QString::number(ui->relative->value()) + " ";
    if(ui->normal->value() != 0) 		options += " -n " + QString::number(ui->normal->value()) + " ";
    if(ui->calculational->value() != 0) options += " -D " + QString::number(ui->calculational->value()) + " ";

    emit optionsSet(options);
    this->close();
}

void DialogExportDXF::on_buttonBox_rejected()
{
    emit optionsSet("rejected");
    this->close();
}
