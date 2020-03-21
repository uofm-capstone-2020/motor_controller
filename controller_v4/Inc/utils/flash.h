#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"

typedef uint32_t Address;
typedef uint32_t Sector;
typedef unsigned char Byte;

// Addresses bounding each sector in the Flash memory.
#define FLASH_SECTOR_0_START    0x08000000UL
#define FLASH_SECTOR_0_END      0x08007FFFUL
#define FLASH_SECTOR_1_START    0x08008000UL
#define FLASH_SECTOR_1_END      0x0800FFFFUL
#define FLASH_SECTOR_2_START    0x08010000UL
#define FLASH_SECTOR_2_END      0x08017FFFUL
#define FLASH_SECTOR_3_START    0x08018000UL
#define FLASH_SECTOR_3_END      0x0801FFFFUL
#define FLASH_SECTOR_4_START    0x08020000UL
#define FLASH_SECTOR_4_END      0x0803FFFFUL
#define FLASH_SECTOR_5_START    0x08040000UL
#define FLASH_SECTOR_5_END      0x0807FFFFUL
#define FLASH_SECTOR_6_START    0x08080000UL
#define FLASH_SECTOR_6_END      0x080BFFFFUL
#define FLASH_SECTOR_7_START    0x080C0000UL
#define FLASH_SECTOR_7_END      0x080FFFFFUL
#define FLASH_SECTOR_8_START    0x08100000UL
#define FLASH_SECTOR_8_END      0x0813FFFFUL
#define FLASH_SECTOR_9_START    0x08140000UL
#define FLASH_SECTOR_9_END      0x0817FFFFUL
#define FLASH_SECTOR_10_START   0x08180000UL
#define FLASH_SECTOR_10_END     0x081BFFFFUL
#define FLASH_SECTOR_11_START   0x081C0000UL
#define FLASH_SECTOR_11_END     0x081FFFFFUL

/**
 * Helper functions checking if an address is located in a specific Flash sector
 *
 * @param ADDRESS The address being queried
 * @return true or false
 */
#define IS_FLASH_SECTOR_0(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_0_START) && ((ADDRESS) <= FLASH_SECTOR_0_END))
#define IS_FLASH_SECTOR_1(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_1_START) && ((ADDRESS) <= FLASH_SECTOR_1_END))
#define IS_FLASH_SECTOR_2(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_2_START) && ((ADDRESS) <= FLASH_SECTOR_2_END))
#define IS_FLASH_SECTOR_3(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_3_START) && ((ADDRESS) <= FLASH_SECTOR_3_END))
#define IS_FLASH_SECTOR_4(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_4_START) && ((ADDRESS) <= FLASH_SECTOR_4_END))
#define IS_FLASH_SECTOR_5(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_5_START) && ((ADDRESS) <= FLASH_SECTOR_5_END))
#define IS_FLASH_SECTOR_6(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_6_START) && ((ADDRESS) <= FLASH_SECTOR_6_END))
#define IS_FLASH_SECTOR_7(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_7_START) && ((ADDRESS) <= FLASH_SECTOR_7_END))
#define IS_FLASH_SECTOR_8(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_8_START) && ((ADDRESS) <= FLASH_SECTOR_8_END))
#define IS_FLASH_SECTOR_9(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_9_START) && ((ADDRESS) <= FLASH_SECTOR_9_END))
#define IS_FLASH_SECTOR_10(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_10_START) && ((ADDRESS) <= FLASH_SECTOR_10_END))
#define IS_FLASH_SECTOR_11(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_11_START) && ((ADDRESS) <= FLASH_SECTOR_11_END))
/**
 * Returns the sector containing a given addtress
 *
 * @param address The 32bit Flash address being sectorized
 * @return The sector coresponding to the given address, or -1 if address is located outside the flash
 */
Sector sector_from_address(Address address);

/**
 * Erase a Flash sector
 *
 * @param sector The sector to be erased
 * @param voltage_range The voltage_range that the MCU operates in (see HAL libraries for documentation)
 * @param error_handler a pointer to a function that returns nothing and takes no inputs
 *  will be called on HAL error
 * @return the HAL_StatusTypeDef result.
 */
HAL_StatusTypeDef flash_erase_sector(Sector sector, uint8_t voltage_range, void (*error_handler)());

/**
 * Program a byte into Flash memory after erasing the sector
 *
 * @param address The address in memory to program
 * @param byte The 8-bit data you wish to store
 * @param error_handler a pointer to a function that returns nothing and takes no inputs
 *  will be called on HAL error
 * @return the HAL_StatusTypeDef result.
 */
HAL_StatusTypeDef flash_program_byte(Address address, Byte byte, void (*error_handler)());

/**
 * Program an array of byte into Flash memory after erasing the sector(s).
 * The data is stored sequentially.
 *
 * @param address The starting address in memory to begin programming
 * @param byte A pointer to an array of bytes that will be stored one by one
 * @param error_handler a pointer to a function that returns nothing and takes no inputs
 *  will be called on HAL error
 * @return the HAL_StatusTypeDef result.
 */
HAL_StatusTypeDef flash_program_sequential(Address address, Byte bytes[], void (*error_handler)());

/**
 * Erases the Flash memory as configured by the FLASH_EraseInitTypeDef
 * This function is called by flash_erase_sector but can be called and configured separately
 *
 * @param EraseInitStruct A pointer to the struct configuring the flash erase command
 * @param error_handler a pointer to a function that returns nothing and takes no inputs
 *  will be called on HAL error
 * @return the HAL_StatusTypeDef result.
 */
HAL_StatusTypeDef flash_erase(FLASH_EraseInitTypeDef* EraseInitStruct, void (*error_handler)());

/**
 * Unlocks the flash, programs a byte, and locks the flash.
 * This does not erase the sector. This is called by flash_program_byte and flash_program_sequential
 *
 * @param address The address where data is stored
 * @param byte The data that will be stored
 * @param error_handler a pointer to a function that returns nothing and takes no inputs
 *  will be called on HAL error
 * @return the HAL_StatusTypeDef result.
 */
HAL_StatusTypeDef flash_program(Address address, Byte byte, void (*error_handler)());

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H */
