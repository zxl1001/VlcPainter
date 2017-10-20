/**
 *******************************************************************************
 *                       Continental Confidential
 *                  Copyright (c) Continental AG. 2017
 *
 *      This software is furnished under license and may be used or
 *      copied only in accordance with the terms of such license.
 *******************************************************************************
 * @file    MainWindow.cpp
 * @brief
 *******************************************************************************
 */
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QToolButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mHeaderView(Q_NULLPTR)
{
    ui->setupUi(this);
    mHeaderView = new QSpreadsheetHeaderView(Qt::Horizontal, ui->tableWidget);
    ui->tableWidget->setHorizontalHeader(mHeaderView);
    ui->tableWidget->horizontalHeader();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent* event)
{
    qDebug() << "mouse double click event!";
}
