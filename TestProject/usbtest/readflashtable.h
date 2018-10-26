#ifndef READFLASHTABLE_H
#define READFLASHTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>

class ReadFlashTable : public QWidget
{
    Q_OBJECT
public:
    explicit ReadFlashTable(QWidget *parent = nullptr);

signals:

public slots:

private:
    QTableView * table_view;
    QStandardItemModel * item_model;
};

#endif // READFLASHTABLE_H
