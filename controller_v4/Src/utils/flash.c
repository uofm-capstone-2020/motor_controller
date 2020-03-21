#include "utils/flash.h"
#include "stdbool.h"

/**
 * Calls IS_FLASH_SECTOR_X to locate the correct sector
 */
Sector sector_from_address(Address address){
    Sector sector = -1;
    if(IS_FLASH_SECTOR_0(address)){
        sector = FLASH_SECTOR_0;
    }
    else if(IS_FLASH_SECTOR_1(address)){
        sector = FLASH_SECTOR_1;
    }
    else if(IS_FLASH_SECTOR_2(address)){
        sector = FLASH_SECTOR_2;
    }
    else if(IS_FLASH_SECTOR_3(address)){
        sector = FLASH_SECTOR_3;
    }
    else if(IS_FLASH_SECTOR_4(address)){
        sector = FLASH_SECTOR_4;
    }
    else if(IS_FLASH_SECTOR_5(address)){
        sector = FLASH_SECTOR_5;
    }
    else if(IS_FLASH_SECTOR_6(address)){
        sector = FLASH_SECTOR_6;
    }
    else if(IS_FLASH_SECTOR_7(address)){
        sector = FLASH_SECTOR_7;
    }
    else if(IS_FLASH_SECTOR_8(address)){
        sector = FLASH_SECTOR_8;
    }
    else if(IS_FLASH_SECTOR_9(address)){
        sector = FLASH_SECTOR_9;
    }
    else if(IS_FLASH_SECTOR_10(address)){
        sector = FLASH_SECTOR_10;
    }
    else if(IS_FLASH_SECTOR_11(address)){
        sector = FLASH_SECTOR_11;
    }
    return sector;
}

/**
 * Sets up a FLASH_EraseInitTypeDef and calls flash_erase()
 * EraseInitStruct is configured as follows:
 *
 * Set the Erase Type to Sector Mode (instead of mass erase)
 * Set the Voltage Range to what's specified by the user
 *  (Ideally this will be VOLTAGE_RANGE_1 but C doesn't let me do function overloads for defaults)
 * Set the Sector to what's specified by the user
 * Set the Number of Sectors to 1
 *
 */
HAL_StatusTypeDef flash_erase_sector(Sector sector, uint8_t voltage_range, void (*error_handler)()){
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = voltage_range;
    EraseInitStruct.Sector = sector; //Specify sector number
    EraseInitStruct.NbSectors = 1; //This is also important!
    return flash_erase(&EraseInitStruct, error_handler);
}


/**
 * Unlock the flash, erase the sector, lock the flash
 */
HAL_StatusTypeDef flash_erase(FLASH_EraseInitTypeDef* EraseInitStruct, void (*error_handler)()){
    uint32_t SectorError;
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef result = HAL_FLASHEx_Erase(EraseInitStruct, &SectorError);
    HAL_FLASH_Lock();
    if(result != HAL_OK){
        error_handler();
    }
    return result;
}

/**
 * Determine the sector from the address, erase the sector, program the flash
 */
HAL_StatusTypeDef flash_program_byte(Address address, Byte byte, void (*error_handler)()){
    Sector sector = sector_from_address(address);
    HAL_StatusTypeDef result;
    if(sector == -1){
        return HAL_ERROR;
    }
    result = flash_program(address, byte, error_handler);
    return result;
}

/**
 * Determine the range of sectors the data belongs in, erase those sectors, and program the data
 */
HAL_StatusTypeDef flash_program_sequential(Address address, Byte bytes[], void (*error_handler)()){
    uint32_t N = sizeof(bytes) / sizeof(bytes[0]);
    Sector sector_start = sector_from_address(address);
    Sector sector_end = sector_from_address(address + N);
    HAL_StatusTypeDef result;
    if(sector_start == -1 || sector_end == -1){
        return HAL_ERROR;
    }
    for(int i = 0; i < N; i++){
        result = flash_program(address+i, bytes[i], error_handler);
        if(result != HAL_OK){
            break;
        }
    }
    return result;
}

/**
 * Unlock the flash, program the flash, lock the flash
 */
HAL_StatusTypeDef flash_program(Address address, Byte byte, void (*error_handler)()){
    HAL_FLASH_Unlock();
    HAL_StatusTypeDef result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address, byte);
    HAL_FLASH_Lock();
    if(result != HAL_OK){
        error_handler();
    }
    return result;
}
