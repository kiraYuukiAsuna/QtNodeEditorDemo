#pragma once

#include <QDialog>

namespace Ui {
class ConnectionPopupWindow;
}

class ConnectionPopupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionPopupWindow(QWidget *parent = nullptr);
    ~ConnectionPopupWindow();

private:
    Ui::ConnectionPopupWindow *ui;
};
