/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    TagCatageoryDialog.cpp
 * @brief
 *******************************************************************************
 */
#include "TagCategoryDialog.h"
#include "ui_TagCategoryDialog.h"
namespace Video {
TagCategoryDialog::TagCategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagCategoryDialog)
{
    ui->setupUi(this);
}

TagCategoryDialog::~TagCategoryDialog()
{
    delete ui;
}

void TagCategoryDialog::on_extendBtn_clicked(bool checked)
{
    emit expendChange(checked);
}
}//namespace end


