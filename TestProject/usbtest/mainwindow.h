#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QObject>
#include <QWidget>
#include <QTableView>
#include "file_info_struct.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnOpenFile_clicked();

    void on_btnAddToList_clicked();

    void on_btnBurnInFlash_clicked();



private:
    Ui::MainWindow *ui;
    QStandardItemModel * flash_read_item_model;
    QStandardItemModel * flash_write_item_model;
    QString funGetFlagStrByflagValue(int flag);
    int funGetFlagByUi();
    QString funGetMcuStr(int manufacturer,int type);
    void funInitTableViewTitleInit(QTableView *table ,QStandardItemModel *model);
    void funTableViewFlashInsert(QTableView *table ,QStandardItemModel *model,int id ,FLASH_UN flash_info);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
};

#endif // MAINWINDOW_H
