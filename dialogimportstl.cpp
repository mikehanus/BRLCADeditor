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
#include "dialogimportstl.h"
#include "ui_dialogimportstl.h"

DialogImportSTL::DialogImportSTL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImportSTL)
{
    ui->setupUi(this);
}

DialogImportSTL::~DialogImportSTL()
{
    delete ui;
}

void DialogImportSTL::on_buttonBox_accepted()
{
    QString options;

    if(ui->binary->isChecked()) options += " -b";
    if(ui->verbose->isChecked()) options += " -d";

    if(ui->rt->value() != 0) options += " -x " + QString::number(ui->rt->value());

    options += " -t " + QString::number(ui->tolerance->value()) ;
    if(!ui->name->text().isEmpty()) options += " -N " + ui->name->text();

    if(ui->region->value() != 0)
    {
        if(ui->ToAllRegions->isChecked()) options += " -I ";
        else options += " -i ";
        options += QString::number(ui->region->value());
    }

    if(ui->material->value() != 0) options += " -m " + QString::number(ui->material->value());
    if(!ui->units->text().isEmpty()) options += " -c " + ui->units->text();
    if(ui->rt->value() != 0) options += " -x " + QString::number(ui->rt->value());

    emit optionsSet(options);
    this->close();
}

void DialogImportSTL::on_buttonBox_rejected()
{
    emit optionsSet("rejected");
    this->close();
}
