#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <QStandardItemModel>
#include "hidapi.h"
#include "file_info_struct.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTime>
#include <QDir>
#include <QFile>


#include <windows.h>
#include <dbt.h>
#include<devguid.h>
#include<SetupAPI.h>
#include<InitGuid.h>


#define printf qDebug

#define MAX_STR 50
#define HID_SEND_BUF_MAX 64
unsigned char hid_buf[HID_SEND_BUF_MAX];
hid_device *handle;
int res;
    unsigned char buf[65];
    wchar_t wstr[MAX_STR];
QString deviceName;


int table_save_id =0 ;
QByteArray file_bin_array[FILE_MAX_NUM];

QStandardItemModel * item_model;


FLASH_UN flash_info_temp;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

#if 1
    static const GUID GUID_DEVINTERFACE_LIST[] =
    {
    // GUID_DEVINTERFACE_USB_DEVICE
    { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },
    // GUID_DEVINTERFACE_DISK
    { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
    // GUID_DEVINTERFACE_HID,
    { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
    // GUID_NDIS_LAN_CLASS
    { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }
    //// GUID_DEVINTERFACE_COMPORT
    //{ 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } },
    //// GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR
    //{ 0x4D36E978, 0xE325, 0x11CE, { 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 } },
    //// GUID_DEVINTERFACE_PARALLEL
    //{ 0x97F76EF0, 0xF883, 0x11D0, { 0xAF, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x84, 0x5C } },
    //// GUID_DEVINTERFACE_PARCLASS
    //{ 0x811FC6A5, 0xF728, 0x11D0, { 0xA5, 0x37, 0x00, 0x00, 0xF8, 0x75, 0x3E, 0xD1 } }
    };

    //注册插拔事件
    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotifacationFiler;
    ZeroMemory(&NotifacationFiler,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    NotifacationFiler.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotifacationFiler.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

    for(int i=0;i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID);i++)
    {
        NotifacationFiler.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];//GetCurrentUSBGUID();//m_usb->GetDriverGUID();

        hDevNotify = RegisterDeviceNotification((HANDLE)this->winId(),&NotifacationFiler,DEVICE_NOTIFY_WINDOW_HANDLE);
        if(!hDevNotify)
        {
            int Err = GetLastError();
            qDebug() << "注册失败" <<endl;
        }
        //else
    }
#endif
    //usb_init();
   // usb_find_busses(); /* find all busses */
   // usb_find_devices(); /* find all connected devices */

 //my_usb_get_device_list();
   handle = hid_open(0xC251, 0xF001, NULL);
   if(handle!=NULL)
   {
    res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
        wprintf(L"Manufacturer String: %s\n", wstr);
        deviceName = QString::fromWCharArray(wstr);

    }
    ui->setupUi(this);
    ui->labDeviceName->setText(deviceName);
    //table_view = new QTableView(this);
/*
    dataModel->setItem(0, 0, new QStandardItem("1"));
    dataModel->setItem(0, 1, new QStandardItem("pucbinsssfdfasdfasfa_sdfs.bin"));
    dataModel->setItem(0, 2, new QStandardItem("384294"));
    dataModel->setItem(0, 3, new QStandardItem("0x9000"));
    dataModel->setItem(0, 4, new QStandardItem("擦除|校验"));


    dataModel->setItem(1, 0, new QStandardItem("1"));
    dataModel->setItem(1, 1, new QStandardItem("pucbinsssfdfasdfasfa_sdfs.bin"));
    dataModel->setItem(1, 2, new QStandardItem("384294"));
    dataModel->setItem(1, 3, new QStandardItem("0x9000"));
    dataModel->setItem(1, 4, new QStandardItem("擦除|校验"));
*/
    //item_model = new QStandardItemModel(4,4);  // 4行4列
    //ui->tableView->setModel(item_model);
    //ui->tableView_2->setModel(item_model);
   //  ui->tableView->setModel(dataModel);  //绑定数据模型
    flash_read_item_model= new QStandardItemModel();
    flash_write_item_model = new QStandardItemModel();
     funInitTableViewTitleInit(ui->tvFLashRead,flash_read_item_model);
     funInitTableViewTitleInit(ui->tvFashWirte,flash_write_item_model);
      //  funTableViewFlashInsert(ui->tableView,1);

}
void MainWindow::   funInitTableViewTitleInit(QTableView *table ,QStandardItemModel *model)
{
  //  QStandardItemModel* dataModel = new QStandardItemModel();

    model->setColumnCount(5);
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("文件名"));
    model->setHeaderData(2, Qt::Horizontal, tr("文件大小Kb"));
    model->setHeaderData(3, Qt::Horizontal, tr("起始地址"));
    model->setHeaderData(4, Qt::Horizontal, tr("配置"));
    table->setModel(model);
}
void MainWindow::  funTableViewFlashInsert(QTableView *table ,QStandardItemModel *model,int id ,FLASH_UN flash_info)
{

    model->setItem(id, 0, new QStandardItem("1"));
    model->setItem(id, 1, new QStandardItem(QString(QLatin1String((char *)flash_info.fileInfo.fileName))));
    model->setItem(id, 2, new QStandardItem(flash_info.fileInfo.fileSize));
    model->setItem(id, 3, new QStandardItem("0x9000"));
    model->setItem(id, 4, new QStandardItem(funGetFlagStrByflagValue(flash_info.fileInfo.flag)));

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpenFile_clicked()
{
    QString fileName;

    ui->txtFilePath->clear();
    QString filePath = QFileDialog::getOpenFileName(this,tr("open bin file"),"./",tr("BIN files(*.bin);;All files (*.*)"));
    QFileInfo fileinfo =QFileInfo(filePath);


    QFile fileRead(filePath);
    fileRead.open(QIODevice::ReadOnly);
    ui->txtFilePath->setText(filePath);
    QByteArray by;
        char *bf;
        by = fileRead.readAll();
        bf = by.data();
        fileRead.close();
   unsigned long long fileSize = by.length();
   //QDataStream  readDataStream(&fileRead);

    flash_info_temp.fileInfo.id = 0;
    flash_info_temp.fileInfo.crc = 0;
    flash_info_temp.fileInfo.date = fileinfo.lastModified().toTime_t();  //文件修改时间戳
    flash_info_temp.fileInfo.times = ui->txtTimes->text().toInt(); //烧录次数
    flash_info_temp.fileInfo.destaddr = 0;
    flash_info_temp.fileInfo.binaddr = 0;
    flash_info_temp.fileInfo.idcode = 0;
    flash_info_temp.fileInfo.fileSize = fileinfo.size();  //文件大小 bytes

    int fileNameLen= fileinfo.fileName().toLatin1().length();

    fileName = fileinfo.fileName();
    memcpy(flash_info_temp.fileInfo.fileName,fileName.toLatin1().data(),fileNameLen);
}
int MainWindow::funGetFlagByUi()
{
    int result=0;
    int tempCmbSelectIndex = 0;
    if( ui->cbIsCheckCRC->isChecked()==true) result|=1;
    if(ui->cbAutoUnLockReadFlag->isChecked()==true) result|= 1<<1;
    if(ui->cbAutoResetAndRun->isChecked()==true) result|= 1<<2;
    if(ui->cmbEraseWay->currentIndex() == 1)  result|= 1<<3;

    tempCmbSelectIndex = ui->cmbReadProtectLevel->currentIndex();
    if(tempCmbSelectIndex==1) result|= 1<<4;
    else if (tempCmbSelectIndex==2) result|= 1<<5;
    return result;

}
void MainWindow::on_btnAddToList_clicked()
{
    QString flagStr;
    if(ui->txtFilePath->text()!="")
    {
        flash_info_temp.fileInfo.destaddr = ui->txtDestAddr->text().toLong();
        flash_info_temp.fileInfo.times = ui->txtTimes->text().toInt();
        flash_info_temp.fileInfo.flag = funGetFlagByUi();
        flagStr = funGetFlagStrByflagValue(flash_info_temp.fileInfo.flag);
        WriteToFlashInfoStruct(&flash_info_write_array[table_save_id],flash_info_temp);
        funTableViewFlashInsert(ui->tvFashWirte,flash_write_item_model,0,flash_info_temp);

    }

}
QString MainWindow:: funGetFlagStrByflagValue(int flag)
{
    QString result;

    if((flag&0x01)==1) result+="校验|";
    if(((flag>>1)&0x01)==1) result+="写保护|";
    if(((flag>>2)&0x01)==1) result+="重启运行|";
    if(((flag>>3)&0x01)==1)
        result+="扇区擦除|";
    else result+="整片擦除|";
    flag=flag>>4;
    if(flag == 0)
    {result+="无保护";}
    else if(flag == 1)
    {result+="存储保护";}
    else
    {result+="芯片保护";}

    return result;
}
QString MainWindow:: funGetMcuStr(int manufacturer,int type)
{

}

void MainWindow::on_btnBurnInFlash_clicked()
{
    memset(hid_buf,0,sizeof(hid_buf));
    unsigned char errorbuf[100];
    hid_buf[0]=00;
    hid_buf[1]=0x17;
    res =  hid_write(handle,hid_buf,HID_SEND_BUF_MAX);
    if(res != 0)
    {
      printf("%s",hid_error(handle));
        hid_buf[0]=0;

    }
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = reinterpret_cast<MSG*>(message);
        int msgType = msg->message;
        if(msgType==WM_DEVICECHANGE)
        {
            //qDebug() << "Event DEVICECHANGE Happend" << endl;
            PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
            switch (msg->wParam) {
            case DBT_DEVICEARRIVAL:
                if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                {
                    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                    if(lpdbv->dbcv_flags ==0)
                    {
                        //插入u盘
                        //QString USBDisk = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                       // qDebug() << "USB_Arrived and The USBDisk is: "<<USBDisk ;
                       // ui->textBrowser->append("USB_Arrived and The USBDisk is " + USBDisk);

                    }
                }
                if(lpdb->dbch_devicetype = DBT_DEVTYP_DEVICEINTERFACE)
                {
                    PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;

                 QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);

                   // ui->textBrowser->append("插入设备 ：" + strname);
                }
                break;
            case DBT_DEVICEREMOVECOMPLETE:
                qDebug() << "设备移除" <<endl;
                if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
                {
                    PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                    if(lpdbv->dbcv_flags == 0)
                    {
                        //qDebug() << "USB_Removed";
                      //  ui->textBrowser->append("USB_Remove");

                    }
                }
                if(lpdb->dbch_devicetype = DBT_DEVTYP_DEVICEINTERFACE)
                {
                    PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
                    qDebug()<< "移除设备(name)：" << pDevInf->dbcc_name;
                    //qDebug()<< "移除设备(guid)：" << pDevInf->dbcc_classguid;
                    //qDebug()<< "移除设备(size)：" << pDevInf->dbcc_size;

                    //QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);

                    //ui->textBrowser->append("移除设备 " + strname);
                }
                break;
            }
        }
        return false;

}

