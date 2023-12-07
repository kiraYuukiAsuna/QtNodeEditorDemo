#pragma once

#include <QDialog>
#include <QJsonObject>

class ConnectionPopupWindowBase : public QDialog {
    Q_OBJECT
public:
    ConnectionPopupWindowBase(QJsonObject dataJsonObject, QWidget *parent = nullptr);
    ~ConnectionPopupWindowBase();

    virtual void readFromInternalJsonObject() = 0;
    virtual void writeToInternalJsonObject() = 0;

    QJsonObject& getJsonObject();

private:
    QJsonObject m_DataJsonObject;

};
