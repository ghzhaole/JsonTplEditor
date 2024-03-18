#include "TreeItem.h"

//! [0]
TreeItem::TreeItem(const QString& name, const QVariant &data, TreeItem *parent)
    : TreeItem(name, QVariantMap(), data, parent)
{

}

TreeItem::TreeItem(const QString &name, const QVariantMap &discData, const QVariant &data, TreeItem *parent)
    : parentItem(parent),
      itemType(ItemType::kTypeNull),
      itemName(name),
      itemDisc(discData)
{
    if(!itemDisc.isEmpty() && itemDisc.contains("type") && itemDisc.contains("value")){
        QString type = itemDisc["type"].toString();
        if(type == "bool"){
            itemData = data.isValid()?data.toBool():itemDisc["value"].toBool();
            itemType = ItemType::kTypeFundamental;
        } else if(type == "int"){
            itemData = data.isValid()?data.toInt():itemDisc["value"].toInt();
            itemType = ItemType::kTypeFundamental;
        }else if(type == "longlong"){
            itemData = data.isValid()?data.toLongLong():itemDisc["value"].toLongLong();
            itemType = ItemType::kTypeFundamental;
        }else if(type == "uint"){
            itemData = data.isValid()?data.toUInt():itemDisc["value"].toUInt();
            itemType = ItemType::kTypeFundamental;
        }else if(type == "ulonglong"){
            itemData = data.isValid()?data.toULongLong():itemDisc["value"].toULongLong();
            itemType = ItemType::kTypeFundamental;
        }else if(type == "float"){
            itemData = data.isValid()?data.toFloat():itemDisc["value"].toFloat();
            itemType = ItemType::kTypeFundamental;
        }else if(type == "double"){
            itemData = data.isValid()?data.toDouble():itemDisc["value"].toDouble();
            itemType = ItemType::kTypeFundamental;
        }else if(type == "string"){
            itemData = data.isValid()?data.toString():itemDisc["value"].toString();
            itemType = ItemType::kTypeFundamental;
        }else if(type == "array"){
            itemData = data.isValid()?data.toList():QVariantList() ;
            itemType = ItemType::kTypeList;
        }else if(type == "object"){
            itemData = data.isValid()?data.toMap():QVariantMap() ;
            itemType = ItemType::kTypeMap;
        }

        switch (itemType) {
        case ItemType::kTypeList:{
            auto d = itemData.toList();
            int idx=0;
            for(auto && item: d){
                insertChildren(childCount(), QString::number(idx++),itemDisc["value"].toMap(), item);
            }
            break;
        }
        case ItemType::kTypeMap:{
            itemType = ItemType::kTypeMap;
            auto d = itemData.toMap();
            auto subDiscs = itemDisc["value"].toMap();
            for(auto&& item: subDiscs.keys()){
                insertChildren(childCount(), item, subDiscs[item].toMap(), d.contains(item)?d[item]:QVariant());
            }
            break;
        }
        }
    }
}
//! [0]

//! [1]
TreeItem::~TreeItem()
{
    removeChildren(0, childNumber());
}
//! [1]

//! [2]
TreeItem *TreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}
//! [2]

//! [3]
int TreeItem::childCount() const
{
    return childItems.count();
}
//! [3]

//! [4]
int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    return 0;
}
//! [4]

//! [6]
QVariant TreeItem::data(int column) const
{
    if (column <= 0){
        if(parentItem && parentItem->itemType == ItemType::kTypeList){
            return QString("%1").arg(childNumber());
        }
        return name();
    }else if(column == 1){
        if(itemDisc.contains("type")){
            return itemDisc["type"];
        }
        return QVariant();
    }
    else if(column == 2){
        if(itemType == ItemType::kTypeList){
            return QString("%1 items").arg(childCount());
        }
        else if(itemType == ItemType::kTypeMap){
            return QVariant();
        }
    }

    return itemData;
}
//! [6]

//! [7]
TreeItem* TreeItem::insertChildren(int position, TreeItem* childItem)
{
    if(itemType == ItemType::kTypeFundamental){
        return nullptr;
    }
    childItems.insert(position, childItem);
    return childItem;
}

TreeItem *TreeItem::insertChildren(int position, const QString &name, const QVariantMap &discData, const QVariant &value)
{
    if(itemType == ItemType::kTypeFundamental){
        return nullptr;
    }
    auto childItem = new TreeItem(name, discData, value, this);
    return insertChildren(position, childItem);
}
//! [7]

//! [9]
TreeItem *TreeItem::parent()
{
    return parentItem;
}
//! [9]

//! [10]
bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row){
        auto childItem = childItems.takeAt(position);
        delete childItem;
    }

    return true;
}
//! [10]


//! [11]
bool TreeItem::setData(const QVariant &value)
{
    if(itemType == ItemType::kTypeFundamental){
        switch (itemData.type()) {
        case QVariant::Int:
            itemData = value.toInt();
            break;
        case QVariant::UInt:
            itemData = value.toUInt();
            break;
        case QVariant::LongLong:
            itemData = value.toLongLong();
            break;
        case QVariant::ULongLong:
            itemData = value.toULongLong();
            break;
        case QMetaType::Float:
            itemData = value.toFloat();
            break;
        case QVariant::Double:
            itemData = value.toDouble();
            break;
        case QVariant::Bool:
            itemData = value.toBool();
            break;
        case QVariant::String:
            itemData = value.toString();
            break;
        default:
            itemData = value;
            break;
        }
        return true;
    }
    return false;
}

QVariant TreeItem::getData()
{
    if(itemType == ItemType::kTypeFundamental){
        return itemData;
    } else if(itemType == ItemType::kTypeList){
        QVariantList list;
        for(auto&& childItem : childItems){
            list.append(childItem->getData());
        }
        return list;
    } else if(itemType == ItemType::kTypeMap){
        QVariantMap map;
        for(auto&& childItem : childItems){
            map[childItem->name()] = childItem->getData();
        }
        return map;
    }
    return QVariant();
}
//! [11]
