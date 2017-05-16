#include "mainwindow.h"
#include "ui_mainwindow.h"

static void *lock(void *op, void **plane);
static void unlock(void *op, void *pic,void* const * plane);
static void display(void *op, void *pic);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
