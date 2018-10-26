#include "file_info_struct.h"
FLASH_UN flash_info_write_array[FILE_MAX_NUM];
FLASH_UN flash_info_read_array[FILE_MAX_NUM];
void WriteToFlashInfoStruct(FLASH_UN *info_struct , FLASH_UN info_in)
{
        memcpy(info_struct->buf,&info_in.buf,sizeof(FILE_INFO_CB));
}


void SetFlashInfoStruct(FLASH_UN *info_struct ,uint8_t id, uint8_t flag ,uint8_t times , uint16_t date, uint32_t crc ,uint32_t fileSize , uint32_t binaddr , uint32_t destaddr ,uint8_t *fileName)
{

    if(id < FILE_MAX_NUM)
    {
        info_struct->fileInfo.id = id;
        info_struct->fileInfo.flag = flag;
        info_struct->fileInfo.times = times;
        info_struct->fileInfo.date = date;
        info_struct->fileInfo.crc = crc;
        info_struct->fileInfo.fileSize = fileSize;
        info_struct->fileInfo.binaddr = binaddr;
        info_struct->fileInfo.destaddr = destaddr;
        memcpy(info_struct->fileInfo.fileName,fileName,sizeof(fileName));
    }
}
