#ifndef FILE_INFO_STRUCT_H
#define FILE_INFO_STRUCT_H
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FILE_NAME_MAX_LEN 30
#define FILE_MAX_NUM 4
#define FILE_BIN_START_ADDR 0x100
#define FILE_BIN_BLOCK 0x10

typedef struct
    {
    uint8_t id;
    uint8_t flag;
    uint8_t times;
    uint8_t manufacturer;
    uint8_t type;
    uint64_t date;
    uint32_t crc;
    uint32_t idcode;
    uint32_t fileSize;
    uint32_t binaddr;
    uint32_t destaddr;
    uint8_t fileName[FILE_NAME_MAX_LEN];
    }FILE_INFO_CB;

typedef union
{
    FILE_INFO_CB fileInfo;
    uint8_t buf[sizeof(FILE_INFO_CB)];
}FLASH_UN;

extern FLASH_UN flash_info_write_array[FILE_MAX_NUM];
extern FLASH_UN flash_info_read_array[FILE_MAX_NUM];

void WriteToFlashInfoStruct(FLASH_UN *info_struct , FLASH_UN info_in );
void SetFlashInfoStruct(FLASH_UN *info_struct ,uint8_t id, uint8_t flag ,uint8_t times , uint16_t date, uint32_t crc ,uint32_t fileSize , uint32_t binaddr , uint32_t destaddr ,uint8_t *fileName);

#ifdef __cplusplus
}
#endif

#endif // FILE_INFO_STRUCT_H
