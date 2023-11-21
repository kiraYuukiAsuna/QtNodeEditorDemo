#include "QtNodes/ConnectionPopupWindowBase.h"


ConnectionPopupWindowBase::ConnectionPopupWindowBase(QJsonObject dataJsonObject, QWidget *parent)
    : m_DataJsonObject(dataJsonObject), QDialog(parent) {

}

ConnectionPopupWindowBase::~ConnectionPopupWindowBase() {

}

QJsonObject& ConnectionPopupWindowBase::getJsonObject() {
    return m_DataJsonObject;
}

void ConnectionPopupWindowBase::save() {
    writeToInternalJsonObject();
}
