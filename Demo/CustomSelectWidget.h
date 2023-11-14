#pragma once

#include <QDialog>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>

class CustomSelectWidget : public QWidget {
Q_OBJECT

public:
    CustomSelectWidget(const std::vector<std::string>& paths, bool withCheckBox, bool checkBoxStatus = false, QIcon icon = {} ,QWidget* parent = nullptr)
            : QWidget(parent), m_Icon(icon) {
        QVBoxLayout* layout = new QVBoxLayout(this);

        treeWidget = new QTreeWidget(this);
        treeWidget->setColumnCount(1);
        treeWidget->header()->hide();

        treeWidget->setHeaderHidden(true);
        treeWidget->setColumnCount(1);
        treeWidget->setAnimated(true);
        treeWidget->setItemsExpandable(true);
        treeWidget->setFrameStyle(QFrame::NoFrame);
        treeWidget->setSortingEnabled(true);

        for (const auto& path : paths) {
            addPath(path, withCheckBox, checkBoxStatus);
        }

        layout->addWidget(treeWidget);
    }

    void clearAllAndUpdate(std::vector<std::string>& paths, bool withCheckBox, bool checkBoxStatus = false){
        clearAllItems();
        for (const auto& path : paths) {
            addPath(path, withCheckBox, checkBoxStatus);
        }
    }

    std::vector<std::string> getSelectedPaths() {
        std::vector<std::string> selectedPaths;
        getSelectedPathsRecursive(treeWidget->invisibleRootItem(), selectedPaths);
        return selectedPaths;
    }

    void addPath(const std::string& path, bool withCheckBox, bool checkBoxStatus = false) {
        QStringList parts = QString::fromStdString(path).split('/');
        QTreeWidgetItem* parent = nullptr;

        for (int i = 0; i < parts.size(); ++i) {
            const auto& part = parts.at(i);
            QTreeWidgetItem* item = findChildItem(parent, part);
            if (!item) {
                item = new QTreeWidgetItem(QStringList(part));
                item->setIcon(0, m_Icon);
                if (withCheckBox && i == parts.size() - 1) {
                    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                    if(checkBoxStatus){
                        item->setCheckState(0, Qt::Checked);
                    }else{
                        item->setCheckState(0, Qt::Unchecked);
                    }
                }
                if (parent) {
                    parent->addChild(item);
                } else {
                    treeWidget->addTopLevelItem(item);
                }
            }
            parent = item;
        }
    }

    void update(const std::string& path, bool withCheckBox, bool checkBoxStatus = false) {
        QStringList parts = QString::fromStdString(path).split('/');
        QTreeWidgetItem* parent = nullptr;

        for (int i = 0; i < parts.size(); ++i) {
            const auto& part = parts.at(i);
            QTreeWidgetItem* item = findChildItem(parent, part);
            if (!item) {
                item = new QTreeWidgetItem(QStringList(part));
                item->setIcon(0, m_Icon);
                if (parent) {
                    parent->addChild(item);
                } else {
                    treeWidget->addTopLevelItem(item);
                }
            }

            if (i == parts.size() - 1) {
                if (withCheckBox) {
                    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                    if(checkBoxStatus){
                        item->setCheckState(0, Qt::Checked);
                    }else{
                        item->setCheckState(0, Qt::Unchecked);
                    }
                } else {
                    item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable);
                }
            }

            parent = item;
        }
    }

    bool removeItemByPath(const std::string& path) {
        QStringList parts = QString::fromStdString(path).split('/');
        QTreeWidgetItem* parent = nullptr;
        QTreeWidgetItem* itemToRemove = nullptr;

        for (int i = 0; i < parts.size(); ++i) {
            const auto& part = parts.at(i);
            QTreeWidgetItem* item = findChildItem(parent, part);
            if (!item) {
                return false; // 如果路径不存在，则返回 false
            }

            if (i == parts.size() - 1) {
                itemToRemove = item;
            }
            parent = item;
        }

        if (itemToRemove) {
            if (parent) {
                int index = parent->indexOfChild(itemToRemove);
                if (index != -1) {
                    parent->takeChild(index);
                }
            } else {
                int index = treeWidget->indexOfTopLevelItem(itemToRemove);
                if (index != -1) {
                    treeWidget->takeTopLevelItem(index);
                }
            }
            delete itemToRemove;
            return true;
        }

        return false;
    }


    void clearAllItems() {
        treeWidget->clear();
    }

private:
    QTreeWidget* treeWidget;
    QIcon m_Icon;

    QTreeWidgetItem* findChildItem(QTreeWidgetItem* parent, const QString& text) {
        QTreeWidgetItem* root = parent ? parent : treeWidget->invisibleRootItem();
        for (int i = 0; i < root->childCount(); ++i) {
            if (root->child(i)->text(0) == text) {
                return root->child(i);
            }
        }
        return nullptr;
    }

    void getSelectedPathsRecursive(QTreeWidgetItem* item, std::vector<std::string>& selectedPaths, const QString& currentPath = "") {
        for (int i = 0; i < item->childCount(); ++i) {
            QTreeWidgetItem* child = item->child(i);

            QString newPath;
            if(currentPath == ""){
                newPath = child->text(0);
            }else{
                newPath = currentPath + "/" + child->text(0);
            }

            if (child->checkState(0) == Qt::Checked) {
                selectedPaths.push_back(newPath.toStdString());
            }
            getSelectedPathsRecursive(child, selectedPaths, newPath);
        }
    }

};
