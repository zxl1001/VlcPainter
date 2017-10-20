/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    MainWindow.h
 * @brief
 *******************************************************************************
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "ComboBoxHeaderView.h"
#include <QSpreadsheetHeaderView.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    Ui::MainWindow *ui;
    QSpreadsheetHeaderView* mHeaderView;
};

#endif // MAINWINDOW_H
