/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : ata.h                         |
 *    |  SRC MOD   : 08/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __DRIVERS_ATA__
#define __DRIVERS_ATA__

#include <std/types.h>

#define ATA_DATA_PORT 0x1F0
#define ATA_ERROR_PORT 0x1F1
#define ATA_SECTOR_COUNT_PORT 0x1F2
#define ATA_SECTOR_NUM_PORT 0x1F3
#define ATA_CYLINDER_LOW_PORT 0x1F4
#define ATA_CYLINDER_HIGH_PORT 0x1F5
#define ATA_DRIVE_HEAD_PORT 0x1F6
#define ATA_STATUS_PORT 0x1F7
#define ATA_COMMAND_PORT 0x1F7
#define ATA_READ_COMMAND 0x20
#define ATA_WRITE_COMMANDO 0x30

extern __u8 ata_chs_write(void*, __u8, __u16, __u16);
//extern __u8 ata_chs_read(__u8, __u16, __u16, __u16);

#endif


