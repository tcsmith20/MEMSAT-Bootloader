/*
 * File:   progmem.c
 * Author: Tanner Smith
 * 
 * This file is for writing and reading to and from program memory.
 * Data will be written in blocks of 512 instructions (2048 bytes)
 * Everything in this document can be found in the PIC24F Family 
 * Reference Manual, Section 4.
 * 
 * Created on February 28, 2018, 12:30 PM
 */

#include "progmem.h"


/* Used to store the memory being written to program memory.
 * Stored in bytes because read directly from CDH chip.
 * Every four bytes make up one word. Three bytes make
 * up a 24 bit instruction word and one 'phantom byte'
 * (0x00) is added to align instructions on even addresses.
 */
static uint8_t mem_block[2048];

// Index for loops
static int i = 0;

// Program functions
void PROGMEM_UpdateBlock(uint8_t *mem_data, uint32_t address);
uint8_t* PROGMEM_ReadProgramMemory(uint16_t base_address);
void PROGMEM_LoadNewMemory(uint8_t *mem_data);
void PROGMEM_EraseBlock(unsigned int base_address);
void PROGMEM_WriteBlock(uint16_t base_address);
uint32_t PROGMEM_Read(uint32_t address);



/* Updates program memory blocks of 512 instructions at a time
 * 
 * @param *img_data     new image piece to be written to program memory
 * @param address       starting address of image piece
 */
void PROGMEM_UpdateBlock(uint8_t *mem_data, uint32_t address){
    
    TBLPAG = address >> 16;                             // Initialize PM Page Boundary SFR
    uint16_t offset = address & 0xFFFF;             // Initialize lower word of address  
    
    PROGMEM_LoadNewMemory(mem_data);                    // Store 512 instructions in memory buffer in preparation to write
    PROGMEM_EraseBlock(offset);                         // Erase the block
    PROGMEM_WriteBlock(offset);                    // Write mem_data to program memory block    
    
}

/* Stores 512 instructions starting at a specific base address into a 2048 byte array
 * 
 * @param base_address     address in program memory to start reading
 */
uint8_t* PROGMEM_ReadProgramMemory(uint16_t base_address){  

    uint16_t address = base_address;

    for(i = 0; i < 2048; i++){
        uint16_t lower_word = __builtin_tblrdl(address);        // Reads lower word of instruction at address
        uint16_t upper_word = __builtin_tblrdh(address);        // Reads upper word of instruction at address
        mem_block[i++] = lower_word & 0xFF;                     // Stores first byte of lower word in memory buffer
        mem_block[i++] = lower_word >> 8;                       // Stores second byte of lower word in memory buffer
        mem_block[i++] = upper_word & 0xFF;                     // Stores first byte of upper word in memory buffer
        mem_block[i]   = 0x00;                                  // Stores phantom byte in memory buffer

        address += 2;                                           // Incrememnt address by 2 to stay aligned on even program addresses
    }
    
    return mem_block;
}

/* Stores a 2048 long byte image array into the memory block which will be written to program memory.
 * Every four bytes is one instruction (three bytes for 24 bit instruction and one 'phantom byte'
 * 
 * @param *img_data     pointer to first byte in iamge array 
 */
void PROGMEM_LoadNewMemory(uint8_t *mem_data){    

    for(i = 0; i < 2048; i++){
        mem_block[i] = *mem_data++;
    }   
}

/* Erases 8 rows (defined by PIC) 512 instructions from program memory at a specified base address
 * 
 * NOTE: Erase blocks are edge-aligned from the beginning to end of program memory in sections of 1536 bytes
 * 
 * @param base_address     base address of block erased 
 */
void PROGMEM_EraseBlock(uint16_t base_address){ 

    __builtin_tblwtl(base_address, 0x0000);             // Set base address of erase block with dummy latch write
    NVMCON = WRITE_ENABLE_BIT | ERASE_BIT | PAGE_ERASE; // Initialize NVMCON (NVM control register) for Page Erase
    __builtin_write_NVM();                              // C30 function to perform unlock sequence and set WRITE_ENABLE_BIT
                                                        // Unlock sequence uses the NVMKEY register, find more in Section 4 of datasheet
}

/* Programs 8 row of 64 instructions each to program memory at a specified base address
 * 
 * NOTE: Function breaks it into 8 single row programs due to built in function limitations
 * NOTE: Program rows are edge-aligned from the beginning to end of program memory in sections of 192 bytes
 * 
 * @param base_address     base address of block program
 * @param row              specific row being written in the current image piece (1-8)
 */
void PROGMEM_WriteBlock(uint16_t base_address) {

    uint16_t current_address = base_address;
    uint8_t row = 0;

    for(row = 0; row < 8; row++){

        NVMCON = WRITE_ENABLE_BIT | ROW_PROGRAM;            // Initialize NVMCON for Row Programming

        uint16_t row_index = row*256;                       // Corresponding index to current row (4 bytes * 64 instructions)

        for(i = row_index; i < (row_index + 4*NUM_INSTRUCTION_PER_ROW); i++){
            
            uint16_t byte1_low = (mem_block[i++]);                  // Create lower word by combining lower byte 1 with upper byte 2
            uint16_t byte2_low = ((0xFFFF & mem_block[i++]) << 8);  // Create lower word by combining lower byte 1 with upper byte 2
            uint16_t word_low = byte1_low | byte2_low;              // Create lower word by combining lower byte 1 with upper byte 2
            uint16_t word_high = mem_block[i++];                    // Create upper word with byte 3 (Byte 4 will be 0x00 'phantom byte')
            
            __builtin_tblwtl(current_address, word_low);            // Write to lower word
            __builtin_tblwth(current_address, word_high);           // Write to upper word

            current_address += + 2;        // Increment address by 2 because each word takes up one 16 bit register
        }
        
        __builtin_write_NVM();             // C30 function to perform unlock sequence and set WRITE_ENABLE_BIT
                                           // Unlock sequence uses the NVMKEY register, find more in Section 4 of datasheet
    }
}


/* Reads a single address of program memory
 * 
 * @param address       address that will be read
 */
uint32_t PROGMEM_Read(uint32_t address) {
    uint32_t upper_byte = ((0xFFFFFFFF & __builtin_tblrdh(address))<<16);
    uint32_t lower_byte = (__builtin_tblrdl(address));
    return upper_byte | lower_byte;
}
