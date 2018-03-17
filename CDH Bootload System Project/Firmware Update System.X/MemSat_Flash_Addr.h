/* 
 * File:   MemSat_Flash_Addr.h
 * Author: Nick Felker
 *
 * Created on April 18, 2017, 1:52 PM
 * 
 * FLASH     8MB
 *  ____________________________________________
 * |                                            |
 * | 0 - 256000         Firmware Old 1          |
 * | 256001 - 512000    Firmware Old 2          |
 * | 512001 - 768000    Firmware Old 3          |
 * | 768001 - 1024000   Firmware New 1          |
 * | 1024001 - 1280000  Firmware New 2          |
 * | 1280001 - 1536000  Firmware New 3          |
 * | 1536001 - 1792000  Firmware Curr 1         |       ____________________
 * | 1792001 - 2048000  Firmware Curr 2         |      |                    |
 * | 2048001 - 2304000  Firmware Curr 3         |      |  System Launched?  |
 * | 2304001 - 2305000  C&DH Config Stuff       | ---> |    Storage Ptrs    |
 * | 2305001 - 4705000  Payload Results         |      |    Other things    |
 * | 4705001 - 8000000  System Logs             |      |____________________|
 * |    4705001 - 5000000 System Status
 * |    5000001 - 8000000 History
 * |____________________________________________|
 */

#ifndef MEMSAT_FLASH_ADDR_H
#define	MEMSAT_FLASH_ADDR_H

#define FLASH_FIRMWARE_ORG_1 0
#define FLASH_FIRMWARE_ORG_2 256001
#define FLASH_FIRMWARE_ORG_3 512001
#define FLASH_FIRMWARE_NEW_1 768001
#define FLASH_FIRMWARE_NEW_2 1024001
#define FLASH_FIRMWARE_NEW_3 1280001
#define FLASH_FIRMWARE_CUR_1 1536001
#define FLASH_FIRMWARE_CUR_2 1792001
#define FLASH_FIRMWARE_CUR_3 2048001

#define FLASH_CDH_CONFIG     2304001

#define FLASH_PAYLOAD_PRIMARY_DATA   2305001

#define FLASH_SYSTEM_LOGS    4705001

#define FLASH_PAYLOAD_LOGS 4705001
#define FLASH_PAYLOAD_LOGS_END 4705050
#define FLASH_POWER_LOGS 4705051
#define FLASH_ADCS_LOGS 
#define FLASH_COMMS_LOGS
#define CDH_LOGS
#define CDH_HISTORY 5000001

#define FLASH_TOTAL_CAPACITY 8388608

#define FLASH_CDH_SYSTEM_LAUNCHED   2304001
#define FLASH_CDH_PAYLOAD_START     2304002
#define FLASH_CDH_PAYLOAD_END       2304003
#define FLASH_CDH_LOGS_START        2304004
#define FLASH_CDH_LOGS_END          2304005

#define FLASH_CAPACITY_FLASH (FLASH_SYSTEM_LOGS - FLASH_PAYLOAD_PRIMARY_DATA)
#define FLASH_CAPACITY_LOGS  (FLASH_TOTAL_CAPACITY - FLASH_SYSTEM_LOGS)

#endif	/* MEMSAT_FLASH_ADDR_H */

