#include "FunctionJobInternalWidget.h"
#include "FunctionDelegateModel.h"

void FunctionJobInternalWidget::requstReCalcSize() {
    emit m_DelegateModel->embeddedWidgetSizeUpdated();
}
