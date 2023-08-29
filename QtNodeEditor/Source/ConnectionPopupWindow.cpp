#include "ConnectionPopupWindow.h"
#include "ui_ConnectionPopupWindow.h"

ConnectionPopupWindow::ConnectionPopupWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionPopupWindow)
{
    ui->setupUi(this);


    connect(ui->OkBtn, &QPushButton::clicked, this, [&](bool checked){
        accept();
    });

    connect(ui->CancelBtn, &QPushButton::clicked, this, [&](bool checked){
        reject();
    });


}

ConnectionPopupWindow::~ConnectionPopupWindow()
{
    delete ui;
}
