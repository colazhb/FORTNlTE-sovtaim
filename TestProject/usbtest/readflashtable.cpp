#include "readflashtable.h"

ReadFlashTable::ReadFlashTable(QWidget *parent) : QWidget(parent)
{
    table_view = new QTableView(this);
        item_model = new QStandardItemModel(4,4);  // 4行4列
        table_view->setModel(item_model);          // 关联view与model

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                item_model->setItem(i, j, new QStandardItem(QString("%1").arg(i + j)));  // 向model中添加item

}
