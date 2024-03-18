#include "TreeWidget.h"
#include "TreeItemDelegate.h"
#include <QApplication>
#include <QMenu>
#include<QDebug>

TreeWidget::TreeWidget(QWidget *parent)
    : QTreeView{parent},model_(new TreeModel)
{
    setModel(model_);
    auto itemDelegate = new TreeItemDelegate(this);
    setItemDelegate(itemDelegate);
    connect(model_, &QAbstractItemModel::dataChanged,[=](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles){
        selectionModel()->setCurrentIndex(topLeft,QItemSelectionModel::ClearAndSelect|QItemSelectionModel::Rows);
        emit sigDataChanged();
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &TreeWidget::onTreeViewMenuRequested);
}

void TreeWidget::setData(const QVariantMap &disc,const QVariant & data)
{
    model_->setData( disc,data);
    this->expandToDepth(1);
}

QVariant TreeWidget::getData()
{
    return model_->getData();
}

void TreeWidget::insertRow()
{
    const QModelIndex index = selectionModel()->currentIndex();

    if(model_->insertRow(index.row(), index)){

    }
}

void TreeWidget::removeRow()
{
    const QModelIndex index = selectionModel()->currentIndex();
    model_->removeRow(index.row(), index.parent());
}

void TreeWidget::onTreeViewMenuRequested(const QPoint &pos)
{
    QModelIndex curIndex = indexAt(pos);
    if (curIndex.isValid()) // 右键选中了有效index
    {
        QIcon view = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
        QIcon test = QApplication::style()->standardIcon(QStyle::SP_DesktopIcon);

        // 创建菜单
        QMenu menu;
        menu.addAction(view, QString("添加一行"), this, &TreeWidget::insertRow);
        menu.addSeparator();
        menu.addAction(test, QString("删除一行"), this, &TreeWidget::removeRow);
        menu.exec(QCursor::pos());
    }
}

