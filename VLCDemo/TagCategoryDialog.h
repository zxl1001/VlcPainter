/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    TagCategoryDialog.h
 * @brief
 *******************************************************************************
 */
#ifndef TAGCATAGEORYDIALOG_H
#define TAGCATAGEORYDIALOG_H

#include <QDialog>

namespace Ui {
class TagCategoryDialog;
}
namespace Video {
class TagCategoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagCategoryDialog(QWidget *parent = 0);
    ~TagCategoryDialog();

Q_SIGNALS:
    void expendChange(bool);

private slots:
    void on_extendBtn_clicked(bool checked);

private:
    Ui::TagCategoryDialog *ui;
};
}
#endif // TAGCATAGEORYDIALOG_H
