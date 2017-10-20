/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    %{Cpp:License:FileName}
 * @brief
 *******************************************************************************
 */
#include "MainWindow.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    QPalette pal(Qt::red, Qt::green);
//    pal.setColor(QPalette::Button, Qt::darkYellow);
//    pal.setColor(QPalette::ButtonText, Qt::white);
//    w.setPalette(pal);
    w.show();
    return a.exec();
}
