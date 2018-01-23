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
#include "dialogexportstl.h"
#include "ui_dialogexportstl.h"

DialogExportSTL::DialogExportSTL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExportSTL)
{
    ui->setupUi(this);
}

DialogExportSTL::~DialogExportSTL()
{
    delete ui;
}

void DialogExportSTL::on_buttonBox_accepted()
{
    QString options;

    if(ui->binary->isChecked()) options += "-b ";
    if(ui->inches->isChecked()) options += "-i ";
    if(ui->verbose->isChecked()) options += "-v ";
    if(ui->cubes->isChecked()) options += "-8 ";

    if(ui->rt->text() != "") options += "-x " + ui->rt->text();
    if(ui->nmq->text() != "") options += "-X " + ui->nmq->text();

    if(ui->toleranceType->currentIndex() == 0) options += "-a ";
    if(ui->toleranceType->currentIndex() == 1) options += "-n ";
    if(ui->toleranceType->currentIndex() == 2) options += "-r ";
    if(ui->toleranceType->currentIndex() == 3) options += "-D ";

    options += QString::number(ui->tolerance->value()) + " ";

    emit optionsSet(options);
    this->close();
}

void DialogExportSTL::on_buttonBox_rejected()
{
    emit optionsSet("rejected");
    this->close();
}
