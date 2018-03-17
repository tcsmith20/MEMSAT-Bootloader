#include "MemSat_Power.h"

uint8_t PWAddr = 0x02;      //P31u address, set by P31u
uint8_t HK_Port = 0x08;     //Housekeeping command port

void Power_Init() {
    // Initialize I2C
    I2C_SWini();
    I2C_HWini();
    I2C_ModuleStart();
    // Get ACK from Power?
    // Check Power Health?
    // Other GOMSpace Stuff?
    // Initialize Rails
}

void Power_Disable_Rail() {
    
}

void Power_Enable_Rail() {
    
}


//Send empty packet to request backwards
//compatible housekeeping struct.
hkparam_t Get_HK()
{
    hkparam_t HK;
    char data[45];
    int result;
    
    result = I2C1_M_Read(PWAddr, HK_Port, sizeof data, data);
    
   // port = &data[0];
   // code = &data[1];
    
    if (result == 0)
    {
        HK.pv[0] = data[2];
        HK.pv[0] <<= 8;
        HK.pv[0] += data[3];
        
        HK.pv[1] = data[4];
        HK.pv[1] <<= 8;
        HK.pv[1] += data[5];
        
        HK.pv[2] = data[6];
        HK.pv[2] <<= 8;
        HK.pv[2] += data[7];
                
        HK.pc = data[8];
        HK.pc <<= 8;
        HK.pc += data[9];
        
        HK.bv = data[10];
        HK.bv <<= 8;
        HK.bv += data[11];
        
        HK.sc = data[12];
        HK.sc <<= 8;
        HK.sc += data[13];
        
        HK.temp[0] = data[14];
        HK.temp[0] <<= 8;
        HK.temp[0] += data[15];
        
        HK.temp[1] = data[16];
        HK.temp[1] <<= 8;
        HK.temp[1] += data[17];
        
        HK.temp[2] = data[18];
        HK.temp[2] <<= 8;
        HK.temp[2] += data[19];
        
        HK.temp[3] = data[20];
        HK.temp[3] <<= 8;
        HK.temp[3] += data[21];
        
        HK.batt_temp[0] = data[22];
        HK.batt_temp[0] <<= 8;
        HK.batt_temp[0] += data[23];
        
        HK.batt_temp[1] = data[24];
        HK.batt_temp[1] <<= 8;
        HK.batt_temp[1] += data[25];
        
        HK.latchup[0] = data[26];
        HK.latchup[0] <<= 8;
        HK.latchup[0] += data[27];
        
        HK.latchup[1] = data[28];
        HK.latchup[1] <<= 8;
        HK.latchup[1] += data[29];
        
        HK.latchup[2] = data[30];
        HK.latchup[2] <<= 8;
        HK.latchup[2] += data[31];
        
        HK.latchup[3] = data[32];
        HK.latchup[3] <<= 8;
        HK.latchup[3] += data[33];
        
        HK.latchup[4] = data[34];
        HK.latchup[4] <<= 8;
        HK.latchup[4] += data[35];

        HK.latchup[5] = data[36];
        HK.latchup[5] <<= 8;
        HK.latchup[5] += data[37];
        
        HK.reset = data[38];
        
        HK.bootcount = data[39];
        HK.bootcount <<= 8;
        HK.bootcount += data[40];

        HK.sw_errors = data[41];
        HK.sw_errors <<= 8;
        HK.sw_errors += data[42];
        
        HK.ppt_mode = data[43];
        
        HK.channel_status = data[44];
    }
    return HK;
}

eps_hk_t Get_HK_2()
{
    eps_hk_t HK2;
    uint8_t type = 0;
    char data[125];
    int result;
    
    result = MemSat_Power_Read(PWAddr, HK_Port, type, sizeof data, data);
    
    if (result == 0)
    {   
        HK2.vboost[0] = data[2];
        HK2.vboost[0] <<= 8;
        HK2.vboost[0] += data[3];
        
        HK2.vboost[1] = data[4];
        HK2.vboost[1] <<= 8;
        HK2.vboost[1] += data[5];
        
        HK2.vboost[2] = data[6];
        HK2.vboost[2] <<= 8;
        HK2.vboost[2] += data[7];
        
        HK2.vboost[3] = data[8];
        HK2.vboost[3] <<= 8;
        HK2.vboost[3] += data[9];
        
        HK2.vbatt = data[10];
        HK2.vbatt <<= 8;
        HK2.vbatt += data[11];
        
        HK2.curin[0] = data[12];
        HK2.curin[0] <<= 8;
        HK2.curin[0] += data[13];
        
        HK2.curin[1] = data[14];
        HK2.curin[1] <<= 8;
        HK2.curin[1] += data[15];
        
        HK2.curin[2] = data[16];
        HK2.curin[2] <<= 8;
        HK2.curin[2] += data[17];
        
        HK2.curin[3] = data[18];
        HK2.curin[3] <<= 8;
        HK2.curin[3] += data[19];
        
        HK2.cursun = data[20];
        HK2.cursun <<= 8;
        HK2.cursun += data[21];
        
        HK2.cursys = data[22];
        HK2.cursys <<= 8;
        HK2.cursys += data[23];
        
        HK2.reserved1 = data[24];
        HK2.reserved1 <<= 8;
        HK2.reserved1 += data[25];
        
        HK2.curout[0] = data[26];
        HK2.curout[0] <<= 8;
        HK2.curout[0] += data[27];
        
        HK2.curout[1] = data[28];
        HK2.curout[1] <<= 8;
        HK2.curout[1] += data[29];
        
        HK2.curout[2] = data[30];
        HK2.curout[2] <<= 8;
        HK2.curout[2] += data[31];
        
        HK2.curout[3] = data[32];
        HK2.curout[3] <<= 8;
        HK2.curout[3] += data[33];
        
        HK2.curout[4] = data[34];
        HK2.curout[4] <<= 8;
        HK2.curout[4] += data[35];
        
        HK2.curout[5] = data[36];
        HK2.curout[5] <<= 8;
        HK2.curout[5] += data[37];
        
        HK2.curout[6] = data[38];
        HK2.curout[6] <<= 8;
        HK2.curout[6] += data[39];
        
        HK2.curout[7] = data[40];
        HK2.curout[7] <<= 8;
        HK2.curout[7] += data[41];
        
        HK2.output[0] = data[42];
        HK2.output[1] = data[43];
        HK2.output[2] = data[44];
        HK2.output[3] = data[45];
        HK2.output[4] = data[46];
        HK2.output[5] = data[47];
        HK2.output[6] = data[48];
        HK2.output[7] = data[49];
        
        HK2.output_on_delta[0] = data[50];
        HK2.output_on_delta[0] <<= 8;
        HK2.output_on_delta[0] += data[51];
        
        HK2.output_on_delta[1] = data[52];
        HK2.output_on_delta[1] <<= 8;
        HK2.output_on_delta[1] += data[53];
        
        HK2.output_on_delta[2] = data[54];
        HK2.output_on_delta[2] <<= 8;
        HK2.output_on_delta[2] += data[55];
        
        HK2.output_on_delta[3] = data[56];
        HK2.output_on_delta[3] <<= 8;
        HK2.output_on_delta[3] += data[57];
        
        HK2.output_on_delta[4] = data[58];
        HK2.output_on_delta[4] <<= 8;
        HK2.output_on_delta[4] += data[59];
        
        HK2.output_on_delta[5] = data[60];
        HK2.output_on_delta[5] <<= 8;
        HK2.output_on_delta[5] += data[61];
        
        HK2.output_on_delta[6] = data[62];
        HK2.output_on_delta[6] <<= 8;
        HK2.output_on_delta[6] += data[63];
        
        HK2.output_on_delta[7] = data[64];
        HK2.output_on_delta[7] <<= 8;
        HK2.output_on_delta[7] += data[65];
        
        HK2.latchup[0] = data[66];
        HK2.latchup[0] <<= 8;
        HK2.latchup[0] += data[67];
        
        HK2.latchup[1] = data[68];
        HK2.latchup[1] <<= 8;
        HK2.latchup[1] += data[69];
        
        HK2.latchup[2] = data[70];
        HK2.latchup[2] <<= 8;
        HK2.latchup[2] += data[71];
        
        HK2.latchup[3] = data[72];
        HK2.latchup[3] <<= 8;
        HK2.latchup[3] += data[73];
        
        HK2.latchup[4] = data[74];
        HK2.latchup[4] <<= 8;
        HK2.latchup[4] += data[75];
        
        HK2.latchup[5] = data[76];
        HK2.latchup[5] <<= 8;
        HK2.latchup[5] += data[77];
        
        HK2.wdt_i2c_time_left = data[78];
        HK2.wdt_i2c_time_left <<= 8;
        HK2.wdt_i2c_time_left += data[79];
        HK2.wdt_i2c_time_left <<= 8;
        HK2.wdt_i2c_time_left += data[80];
        HK2.wdt_i2c_time_left <<= 8;
        HK2.wdt_i2c_time_left += data[81];
        HK2.wdt_i2c_time_left <<= 8;
        
        HK2.wdt_gnd_time_left = data[82];
        HK2.wdt_gnd_time_left <<= 8;
        HK2.wdt_gnd_time_left += data[83];
        HK2.wdt_gnd_time_left <<= 8;
        HK2.wdt_gnd_time_left += data[84];
        HK2.wdt_gnd_time_left <<= 8;
        HK2.wdt_gnd_time_left += data[85];
        HK2.wdt_gnd_time_left <<= 8;
        
        HK2.wdt_csp_pings_left[0] = data[86]; 
        HK2.wdt_csp_pings_left[1] = data[87];
        
        HK2.wdt_gnd_time_left = data[88];
        HK2.wdt_gnd_time_left <<= 8;
        HK2.wdt_gnd_time_left += data[89];
        HK2.wdt_gnd_time_left <<= 8;
        HK2.wdt_gnd_time_left += data[90];
        HK2.wdt_gnd_time_left <<= 8;
        HK2.wdt_gnd_time_left += data[91];
        HK2.wdt_gnd_time_left <<= 8;
        
        HK2.counter_wdt_gnd = data[92];
        HK2.counter_wdt_gnd <<= 8;
        HK2.counter_wdt_gnd += data[93];
        HK2.counter_wdt_gnd <<= 8;
        HK2.counter_wdt_gnd += data[94];
        HK2.counter_wdt_gnd <<= 8;
        HK2.counter_wdt_gnd += data[95];
        HK2.counter_wdt_gnd <<= 8;

        HK2.counter_wdt_csp[0] = data[96];
        HK2.counter_wdt_csp[0] <<= 8;
        HK2.counter_wdt_csp[0] += data[97];
        HK2.counter_wdt_csp[0] <<= 8;
        HK2.counter_wdt_csp[0] += data[98];
        HK2.counter_wdt_csp[0] <<= 8;
        HK2.counter_wdt_csp[0]+= data[99];
        HK2.counter_wdt_csp[0] <<= 8;

        HK2.counter_wdt_csp[1] = data[100];
        HK2.counter_wdt_csp[1] <<= 8;
        HK2.counter_wdt_csp[1] += data[101];
        HK2.counter_wdt_csp[1] <<= 8;
        HK2.counter_wdt_csp[1] += data[102];
        HK2.counter_wdt_csp[1] <<= 8;
        HK2.counter_wdt_csp[1] += data[103];
        HK2.counter_wdt_csp[1] <<= 8;
        
        HK2.counter_boot = data[104];
        HK2.counter_boot <<= 8;
        HK2.counter_boot += data[105];
        HK2.counter_boot <<= 8;
        HK2.counter_boot += data[106];
        HK2.counter_boot <<= 8;
        HK2.counter_boot += data[107];
        HK2.counter_boot <<= 8;
        
        HK2.temp[0] = data[108];
        HK2.temp[0] <<= 8;
        HK2.temp[0] += data[109];
        
        HK2.temp[1] = data[110];
        HK2.temp[1] <<= 8;
        HK2.temp[1] += data[111];
        
        HK2.temp[2] = data[112];
        HK2.temp[2] <<= 8;
        HK2.temp[2] += data[113];
        
        HK2.temp[3] = data[114];
        HK2.temp[3] <<= 8;
        HK2.temp[3] += data[115];
        
        HK2.temp[4] = data[116];
        HK2.temp[4] <<= 8;
        HK2.temp[4] += data[117];
        
        HK2.temp[5] = data[118];
        HK2.temp[5] <<= 8;
        HK2.temp[5] += data[119];
        
        HK2.bootcause =  data[120];
        HK2.battmode =  data[121];
        HK2.pptmode =  data[122];
        
        HK2.reserved2 = data[123];
        HK2.reserved2 <<= 8;
        HK2.reserved2 += data[124];
    }
     return HK2;
}

eps_hk_vi_t Get_HK_2_VI()
{
    eps_hk_vi_t HKI;
    uint8_t type = 1;
    char data[22];
    int result;
    
    result = MemSat_Power_Read(PWAddr, HK_Port, type, sizeof data, data);
    
    if (result == 0)
    {
        HKI.vboost[0] = data[2];
        HKI.vboost[0] <<= 8;
        HKI.vboost[0] += data[3];
        
        HKI.vboost[1] = data[4];
        HKI.vboost[1] <<= 8;
        HKI.vboost[1] += data[5];
        
        HKI.vboost[2] = data[6];
        HKI.vboost[2] <<= 8;
        HKI.vboost[2] += data[7];
        
        HKI.vbatt = data[8];
        HKI.vbatt <<= 8;
        HKI.vbatt += data[9];
        
        HKI.curin[0] = data[10];
        HKI.curin[0] <<= 8;
        HKI.curin[0] += data[11];
        
        HKI.curin[1] = data[12];
        HKI.curin[1] <<= 8;
        HKI.curin[1] += data[13];
        
        HKI.curin[2] = data[14];
        HKI.curin[2] <<= 8;
        HKI.curin[2] += data[15];
        
        HKI.cursun = data[16];
        HKI.cursun <<= 8;
        HKI.cursun += data[17];
        
        HKI.cursys = data[18];
        HKI.cursys <<= 8;
        HKI.cursys += data[19];
        
        HKI.reserved1 = data[20];
        HKI.reserved1 <<= 8;
        HKI.reserved1 += data[21];
    }
     return HKI;
}

eps_hk_out_t Get_HK_Out()
{
    eps_hk_out_t HKO;
    uint8_t type = 2;
    char data[66];
    int result;
    
    result = MemSat_Power_Read(PWAddr, HK_Port, type, sizeof data, data);
    
    if (result == 0)
    {
        HKO.curout[0] = data[2];
        HKO.curout[0] <<= 8;
        HKO.curout[0] += data[3];
        
        HKO.curout[1] = data[4];
        HKO.curout[1] <<= 8;
        HKO.curout[1] += data[5];
        
        HKO.curout[2] = data[6];
        HKO.curout[2] <<= 8;
        HKO.curout[2] += data[7];
        
        HKO.curout[3] = data[8];
        HKO.curout[3] <<= 8;
        HKO.curout[3] += data[9];
        
        HKO.curout[4] = data[10];
        HKO.curout[4] <<= 8;
        HKO.curout[4] += data[11];
        
        HKO.curout[5] = data[12];
        HKO.curout[5] <<= 8;
        HKO.curout[5] += data[13];
        
        HKO.output[0] = data[14];
        HKO.output[1] = data[15];
        HKO.output[2] = data[16];
        HKO.output[3] = data[17];
        HKO.output[4] = data[18];
        HKO.output[5] = data[19];
        HKO.output[6] = data[20];
        HKO.output[7] = data[21];
        
        HKO.output_on_delta[0] = data[22];
        HKO.output_on_delta[0] <<= 8;
        HKO.output_on_delta[0] += data[23];
        
        HKO.output_on_delta[1] = data[24];
        HKO.output_on_delta[1] <<= 8;
        HKO.output_on_delta[1] += data[25];
        
        HKO.output_on_delta[2] = data[26];
        HKO.output_on_delta[2] <<= 8;
        HKO.output_on_delta[2] += data[27];
        
        HKO.output_on_delta[3] = data[28];
        HKO.output_on_delta[3] <<= 8;
        HKO.output_on_delta[3] += data[29];
        
        HKO.output_on_delta[4] = data[30];
        HKO.output_on_delta[4] <<= 8;
        HKO.output_on_delta[4] += data[31];
        
        HKO.output_on_delta[5] = data[32];
        HKO.output_on_delta[5] <<= 8;
        HKO.output_on_delta[5] += data[33];
        
        HKO.output_on_delta[6] = data[34];
        HKO.output_on_delta[6] <<= 8;
        HKO.output_on_delta[6] += data[35];
        
        HKO.output_on_delta[7] = data[36];
        HKO.output_on_delta[7] <<= 8;
        HKO.output_on_delta[7] += data[37];
        
        HKO.output_off_delta[0] = data[38];
        HKO.output_off_delta[0] <<= 8;
        HKO.output_off_delta[0] += data[39];
        
        HKO.output_off_delta[1] = data[40];
        HKO.output_off_delta[1] <<= 8;
        HKO.output_off_delta[1] += data[41];
        
        HKO.output_off_delta[2] = data[42];
        HKO.output_off_delta[2] <<= 8;
        HKO.output_off_delta[2] += data[43];
        
        HKO.output_off_delta[3] = data[44];
        HKO.output_off_delta[3] <<= 8;
        HKO.output_off_delta[3] += data[45];
        
        HKO.output_off_delta[4] = data[46];
        HKO.output_off_delta[4] <<= 8;
        HKO.output_off_delta[4] += data[47];
        
        HKO.output_off_delta[5] = data[48];
        HKO.output_off_delta[5] <<= 8;
        HKO.output_off_delta[5] += data[49];
        
        HKO.output_off_delta[6] = data[50];
        HKO.output_off_delta[6] <<= 8;
        HKO.output_off_delta[6] += data[51];
        
        HKO.output_off_delta[7] = data[52];
        HKO.output_off_delta[7] <<= 8;
        HKO.output_off_delta[7] += data[53];
        
        HKO.latchup[0] = data[54];
        HKO.latchup[0] <<= 8;
        HKO.latchup[0] += data[55];
        
        HKO.latchup[1] = data[56];
        HKO.latchup[1] <<= 8;
        HKO.latchup[1] += data[57];
        
        HKO.latchup[2] = data[58];
        HKO.latchup[2] <<= 8;
        HKO.latchup[2] += data[59];
        
        HKO.latchup[3] = data[60];
        HKO.latchup[3] <<= 8;
        HKO.latchup[3] += data[61];
        
        HKO.latchup[4] = data[62];
        HKO.latchup[4] <<= 8;
        HKO.latchup[4] += data[63];
        
        HKO.latchup[5] = data[64];
        HKO.latchup[5] <<= 8;
        HKO.latchup[5] += data[65];
    }
     return HKO;
}

eps_hk_wdt_t Get_HK_WDT()
{
    eps_hk_wdt_t HKW;
    uint8_t type = 3;
    char data[28];
    int result;
    
    result = MemSat_Power_Read(PWAddr, HK_Port, type, sizeof data, data);
    
    if (result == 0)
    {
        HKW.wdt_i2c_time_left = data[2];
        HKW.wdt_i2c_time_left <<= 8;
        HKW.wdt_i2c_time_left += data[3];
        HKW.wdt_i2c_time_left <<= 8;
        HKW.wdt_i2c_time_left += data[4];
        HKW.wdt_i2c_time_left <<= 8;
        HKW.wdt_i2c_time_left += data[5];
        HKW.wdt_i2c_time_left <<= 8;
        
        HKW.wdt_gnd_time_left = data[6];
        HKW.wdt_gnd_time_left <<= 8;
        HKW.wdt_gnd_time_left += data[7];
        HKW.wdt_gnd_time_left <<= 8;
        HKW.wdt_gnd_time_left += data[8];
        HKW.wdt_gnd_time_left <<= 8;
        HKW.wdt_gnd_time_left += data[9];
        HKW.wdt_gnd_time_left <<= 8;
        
        HKW.wdt_csp_pings_left[0] = data[10];
        HKW.wdt_csp_pings_left[1] = data[11];
        
        HKW.counter_wdt_i2c = data[12];
        HKW.counter_wdt_i2c <<= 8;
        HKW.counter_wdt_i2c += data[13];
        HKW.counter_wdt_i2c <<= 8;
        HKW.counter_wdt_i2c += data[14];
        HKW.counter_wdt_i2c <<= 8;
        HKW.counter_wdt_i2c += data[15];
        HKW.counter_wdt_i2c <<= 8;
        
        HKW.counter_wdt_gnd = data[16];
        HKW.counter_wdt_gnd <<= 8;
        HKW.counter_wdt_gnd += data[17];
        HKW.counter_wdt_gnd <<= 8;
        HKW.counter_wdt_gnd += data[18];
        HKW.counter_wdt_gnd <<= 8;
        HKW.counter_wdt_gnd += data[19];
        HKW.counter_wdt_gnd <<= 8;
        
        HKW.counter_wdt_csp[0] = data[20];
        HKW.counter_wdt_csp[0] <<= 8;
        HKW.counter_wdt_csp[0] += data[21];
        HKW.counter_wdt_csp[0] <<= 8;
        HKW.counter_wdt_csp[0] += data[22];
        HKW.counter_wdt_csp[0] <<= 8;
        HKW.counter_wdt_csp[0] += data[23];
        HKW.counter_wdt_csp[0] <<= 8;
        
        HKW.counter_wdt_csp[1] = data[24];
        HKW.counter_wdt_csp[1] <<= 8;
        HKW.counter_wdt_csp[1] += data[25];
        HKW.counter_wdt_csp[1] <<= 8;
        HKW.counter_wdt_csp[1] += data[26];
        HKW.counter_wdt_csp[1] <<= 8;
        HKW.counter_wdt_csp[1] += data[27];
        HKW.counter_wdt_csp[1] <<= 8;
    }
     return HKW;
}

eps_hk_basic_t Get_HK_2_Basic()
{
    eps_hk_basic_t HKB;
    uint8_t type = 4;
    char data[23];
    int result;
    
    result = MemSat_Power_Read(PWAddr, HK_Port, type, sizeof data, data);
    
    if (result == 0)
    {
        HKB.counter_boot = data[2];
        HKB.counter_boot <<= 8;
        HKB.counter_boot += data[3];
        HKB.counter_boot <<= 8;
        HKB.counter_boot += data[4];
        HKB.counter_boot <<= 8;
        HKB.counter_boot += data[5];
        
        HKB.temp[0] = data[6];
        HKB.temp[0] <<= 8;
        HKB.temp[0] += data[7];
        
        HKB.temp[1] = data[8];
        HKB.temp[1] <<= 8;
        HKB.temp[1] += data[9];
        
        HKB.temp[2] = data[10];
        HKB.temp[2] <<= 8;
        HKB.temp[2] += data[11];
        
        HKB.temp[3] = data[12];
        HKB.temp[3] <<= 8;
        HKB.temp[3] += data[13];
        
        HKB.temp[4] = data[14];
        HKB.temp[4] <<= 8;
        HKB.temp[4] += data[15];
        
        HKB.temp[5] = data[16];
        HKB.temp[5] <<= 8;
        HKB.temp[5] += data[17];
        
        HKB.bootcause = data[18];
        HKB.battmode = data[19];
        HKB.pptmode = data[20];
        
        HKB.reserved2 = data[21];
        HKB.reserved2 <<= 8;
        HKB.reserved2 += data[22];
    }
     return HKB;
}

//Enable output channel by setting bit high.
//bit:   7  6    5     4     3    2   1   0
//     [NC  NC 3.3V3 3.3V2 3.3V1 5V3 5V2 5V1]
//Set output switch states by a bitmask where "1" means
//the channel is switched on and "0" means it is switched off. 
//LSB is channel 1, next bit is channel 2 etc. (BP4 switch 
//and heater cannot be controlled through this command) 
void Set_Output(uint8_t output)
{
    uint8_t OP_Port = 0x09;     //Output setting port, channel 1 is LSB, channel 2 is next etc.
    char data = (char)output;
    I2C1_M_Write(PWAddr, OP_Port, sizeof data, &data);
}

//Set output %channel% to value %value% with delay %delay%,
//Channel (0-5), BP4 heater(6), BP4 switch(7) 
//Value 0 = Off, 1 = On Delay in seconds.  
void Set_Single_Output(uint8_t channel, uint8_t value, uint16_t delay)
{
    uint8_t OSP_Port = 0x0a;
    char data[4];
    data[0] = (char)channel;
    data[1] = (char)value;
    data[2] = delay & 0xFF;
    data[3] = delay >> 8;
    I2C1_M_Write(PWAddr, OSP_Port, sizeof data, data); //&data gives warning
}

//Set the voltage on the photovoltaic inputs
//V1, V2, V3 in mV. 
//Takes effect when MODE = 2, See SET_PV_AUTO. 
//Transmit voltage1 first and voltage3 last.  
void Set_PV_Volt(uint16_t v1, uint16_t v2, uint16_t v3)
{
    uint8_t PVV_Port = 0x0b;    //PV volt setting port
    char output[6];
    output[0] = v1 & 0xFF;
    output[1] = v1 >> 8;
    output[2] = v2 & 0xFF;
    output[3] = v2 >> 8;
    output[4] = v3 & 0xFF;
    output[5] = v3 >> 8;
    
    I2C1_M_Write(PWAddr, PVV_Port, sizeof output, output); //&output gives warning 
}

//Sets the solar cell power tracking mode
//MODE = 0: Hardware default power point   
//MODE = 1: Maximum power point tracking   
//MODE = 2: Fixed software powerpoint, value set with SET_PV_VOLT, default 4V  
void Set_PV_Auto(uint8_t m)
{
    uint8_t PVA_Port = 0x0c;    //PV mode setting port
    char mode = (char)m;
    I2C1_M_Write(PWAddr, PVA_Port, sizeof mode, &mode);
}

//Cmd = 0: Set heater on/off   
//Heater: 0 = BP4, 1= Onboard, 2 = Both   
//Mode: 0 = OFF, 1 = ON
//Command replies with heater modes.
//0 = OFF, 1 = ON.
//To do only query, simple send an empty message.    
uint16_t Set_Heater(uint8_t cmd, uint8_t heater, uint8_t mode)
{
    uint8_t HT_Port = 0x0d;     //Heater Port
    uint16_t result;
    char data[2];
    char command[3];
    command[0] = (char)cmd;
    command[1] = (char)heater;
    command[2] = (char)mode;
    Heater_Read(PWAddr, HT_Port, sizeof command, command, sizeof data, data);
    result = data[0];
    result <<= 8;
    result += data[1];
    return result;
}

//Send this command to reset boot counter and WDT counters.
//magic  =  0x42  
void Reset_Counters(void)
{
    uint8_t COUNTER_PORT = 0x0f;
    char magic = 0x42;
    I2C1_M_Write(PWAddr, COUNTER_PORT, sizeof magic, &magic);
}

//Send this command to reset (kick) dedicated WDT.
//magic = 0x78  
void Reset_WDT()
{
    uint8_t WDT_PORT = 0x10;
    char magic = 0x78;
    I2C1_M_Write(PWAddr, WDT_PORT, sizeof magic, &magic);
}

//Use this command to control the config system.
//cmd = 1: Restore default config.
//cmd = 2: Confirm current config.
void Config_CMD(uint8_t c)
{
    uint8_t CMD_PORT = 0x11;
    char cmd = (char) c;
    I2C1_M_Write(PWAddr, CMD_PORT, sizeof cmd, &cmd);
}

//Use this command to request the P31 config. 
eps_config_t Config_Get()
{
    uint8_t CONF_GET_PORT = 0x12;
    eps_config_t config;
    int result;
    char data[60];
    result =  I2C1_M_Read(PWAddr, CONF_GET_PORT, sizeof data, data);
    if (result == 0)
    {
        config.ppt_mode = data[2];
        config.battheater_mode = data[3];
        config.battheater_low = data[4];
        config.battheater_high = data[5];
        config.output_normal_value[0] = data[6];
        config.output_normal_value[1] = data[7];
        config.output_normal_value[2] = data[8];
        config.output_normal_value[3] = data[9];
        config.output_normal_value[4] = data[10];
        config.output_normal_value[5] = data[11];
        config.output_normal_value[6] = data[12];
        config.output_normal_value[7] = data[13];
        config.output_safe_value[0] = data[14];
        config.output_safe_value[1] = data[15];
        config.output_safe_value[2] = data[16];
        config.output_safe_value[3] = data[17];
        config.output_safe_value[4] = data[18];
        config.output_safe_value[5] = data[19];
        config.output_safe_value[6] = data[20];
        config.output_safe_value[7] = data[21];
        
        config.output_initial_on_delay[0] = data[22];
        config.output_initial_on_delay[0] <<= 8;
        config.output_initial_on_delay[0] += data[23];
        
        config.output_initial_on_delay[1] = data[24];
        config.output_initial_on_delay[1] <<= 8;
        config.output_initial_on_delay[1] += data[25];
        
        config.output_initial_on_delay[2] = data[26];
        config.output_initial_on_delay[2] <<= 8;
        config.output_initial_on_delay[2] += data[27];
        
        config.output_initial_on_delay[3] = data[28];
        config.output_initial_on_delay[3] <<= 8;
        config.output_initial_on_delay[3] += data[29];
        
        config.output_initial_on_delay[4] = data[30];
        config.output_initial_on_delay[4] <<= 8;
        config.output_initial_on_delay[4] += data[31];
        
        config.output_initial_on_delay[5] = data[32];
        config.output_initial_on_delay[5] <<= 8;
        config.output_initial_on_delay[5] += data[33];
        
        config.output_initial_on_delay[6] = data[34];
        config.output_initial_on_delay[6] <<= 8;
        config.output_initial_on_delay[6] += data[35];
        
        config.output_initial_on_delay[7] = data[36];
        config.output_initial_on_delay[7] <<= 8;
        config.output_initial_on_delay[7] += data[37];
        
        config.output_initial_off_delay[0] = data[38];
        config.output_initial_off_delay[0] <<= 8;
        config.output_initial_off_delay[0] += data[39];
        
        config.output_initial_off_delay[1] = data[40];
        config.output_initial_off_delay[1] <<= 8;
        config.output_initial_off_delay[1] += data[41];
        
        config.output_initial_off_delay[2] = data[42];
        config.output_initial_off_delay[2] <<= 8;
        config.output_initial_off_delay[2] += data[43];
        
        config.output_initial_off_delay[3] = data[44];
        config.output_initial_off_delay[3] <<= 8;
        config.output_initial_off_delay[3] += data[45];
        
        config.output_initial_off_delay[4] = data[46];
        config.output_initial_off_delay[4] <<= 8;
        config.output_initial_off_delay[4] += data[47];
        
        config.output_initial_off_delay[5] = data[48];
        config.output_initial_off_delay[5] <<= 8;
        config.output_initial_off_delay[5] += data[49];
        
        config.output_initial_off_delay[6] = data[50];
        config.output_initial_off_delay[6] <<= 8;
        config.output_initial_off_delay[6] += data[51];
        
        config.output_initial_off_delay[7] = data[52];
        config.output_initial_off_delay[7] <<= 8;
        config.output_initial_off_delay[7] += data[53];

        config.vboost[0] = data[54];
        config.vboost[0] <<= 8;
        config.vboost[0] += data[55];
        
        config.vboost[1] = data[56];
        config.vboost[1] <<= 8;
        config.vboost[1] += data[57];
        
        config.vboost[2] = data[58];
        config.vboost[2] <<= 8;
        config.vboost[2] += data[59];
    }
    return config;
}

//Use this command to send a config to the P31 and save it.  
void Config_Set(eps_config_t conf)
{
    uint8_t CONF_SET_PORT = 0x13;
    char data[58];
    data[0] = (char)conf.ppt_mode;
    data[1] = (char)conf.battheater_mode;
    data[2] = (char)conf.battheater_low;
    data[3] = (char)conf.battheater_high;
    data[4] = (char)conf.output_normal_value[0];
    data[5] = (char)conf.output_normal_value[1];
    data[6] = (char)conf.output_normal_value[2];
    data[7] = (char)conf.output_normal_value[3];
    data[8] = (char)conf.output_normal_value[4];
    data[9] = (char)conf.output_normal_value[5];
    data[10] = (char)conf.output_normal_value[6];
    data[11] = (char)conf.output_normal_value[7];
    data[12] = (char)conf.output_safe_value[0];
    data[13] = (char)conf.output_safe_value[1];
    data[14] = (char)conf.output_safe_value[2];
    data[15] = (char)conf.output_safe_value[3];
    data[16] = (char)conf.output_safe_value[4];
    data[17] = (char)conf.output_safe_value[5];
    data[18] = (char)conf.output_safe_value[6];
    data[19] = (char)conf.output_safe_value[7];
    data[20] = (char)conf.output_initial_on_delay[0] & 0xFF;
    data[21] = (char)conf.output_initial_on_delay[0] >> 8;
    data[22] = (char)conf.output_initial_on_delay[1] & 0xFF;
    data[23] = (char)conf.output_initial_on_delay[1] >> 8;
    data[24] = (char)conf.output_initial_on_delay[2] & 0xFF;
    data[25] = (char)conf.output_initial_on_delay[2] >> 8;
    data[26] = (char)conf.output_initial_on_delay[3] & 0xFF;
    data[27] = (char)conf.output_initial_on_delay[3] >> 8;
    data[28] = (char)conf.output_initial_on_delay[4] & 0xFF;
    data[29] = (char)conf.output_initial_on_delay[4] >> 8;
    data[30] = (char)conf.output_initial_on_delay[5] & 0xFF;
    data[31] = (char)conf.output_initial_on_delay[5] >> 8;
    data[32] = (char)conf.output_initial_on_delay[6] & 0xFF;
    data[33] = (char)conf.output_initial_on_delay[6];
    data[34] = (char)conf.output_initial_on_delay[7] & 0xFF;
    data[35] = (char)conf.output_initial_on_delay[7] >> 8;
    data[36] = (char)conf.output_initial_off_delay[0] & 0xFF;
    data[37] = (char)conf.output_initial_off_delay[0] >> 8;
    data[38] = (char)conf.output_initial_off_delay[1] & 0xFF;
    data[39] = (char)conf.output_initial_off_delay[1] >> 8;
    data[40] = (char)conf.output_initial_off_delay[2] & 0xFF;
    data[41] = (char)conf.output_initial_off_delay[2] >> 8;
    data[42] = (char)conf.output_initial_off_delay[3] & 0xFF;
    data[43] = (char)conf.output_initial_off_delay[3] >> 8;
    data[44] = (char)conf.output_initial_off_delay[4] & 0xFF;
    data[45] = (char)conf.output_initial_off_delay[4] >> 8;
    data[46] = (char)conf.output_initial_off_delay[5] & 0xFF;
    data[47] = (char)conf.output_initial_off_delay[5] >> 8;
    data[48] = (char)conf.output_initial_off_delay[6] & 0xFF;
    data[49] = (char)conf.output_initial_off_delay[6] >> 8;
    data[50] = (char)conf.output_initial_off_delay[7] & 0xFF;
    data[51] = (char)conf.output_initial_off_delay[7] >> 8;
    data[52] = (char)conf.vboost[0] & 0xFF;
    data[53] = (char)conf.vboost[0] >> 8;
    data[54] = (char)conf.vboost[1] & 0xFF;
    data[55] = (char)conf.vboost[1] >> 8;
    data[56] = (char)conf.vboost[2] & 0xFF;
    data[57] = (char)conf.vboost[2] >> 8;

    I2C1_M_Write(PWAddr, CONF_SET_PORT, sizeof data, data);
}

//Send this command to perform a hard reset of the P31,
//including cycling permanent 5V and 3.3V and battery outputs.  
void Hard_Reset()
{
    uint8_t HR_PORT = 0x14;
    char cmd = 0x00;
    int size = 0;
    I2C1_M_Write(PWAddr, HR_PORT, size, &cmd);
}

//Use this command to control the config 2 system.
//cmd = 1: Restore default config.
//cmd = 2: Confirm current config.
void Config2_CMD(uint8_t c)
{
    uint8_t CMD_PORT = 0x15;
    char cmd = (char) c;
    I2C1_M_Write(PWAddr, CMD_PORT, sizeof cmd, &cmd);
}

//Use this command to request the P31 config 2.  
eps_config2_t Config2_Get()
{
    uint8_t CONF2_GET_PORT = 0x16;
    eps_config2_t conf;
    int result;
    char data[22];
    result =  I2C1_M_Read(PWAddr, CONF2_GET_PORT, sizeof data, data);
    if (result == 0)
    { 
        conf.batt_maxvoltage = data[2];
        conf.batt_maxvoltage <<= 8;
        conf.batt_maxvoltage += data[3];
        
        conf.batt_safevoltage = data[4];
        conf.batt_safevoltage <<= 8;
        conf.batt_safevoltage += data[5];
        
        conf.batt_criticalvoltage = data[6];
        conf.batt_criticalvoltage <<= 8;
        conf.batt_criticalvoltage += data[7];
        
        conf.batt_normalvoltage = data[8];
        conf.batt_normalvoltage <<= 8;
        conf.batt_normalvoltage += data[9];
        
        conf.reserved1[0] = data[10];
        conf.reserved1[0] <<= 8;
        conf.reserved1[0] += data[11];
        conf.reserved1[0] <<= 8;
        conf.reserved1[0] += data[12];
        conf.reserved1[0] <<= 8;
        conf.reserved1[0] += data[13];
        
        conf.reserved1[1] = data[14];
        conf.reserved1[1] <<= 8;
        conf.reserved1[1] += data[15];
        conf.reserved1[1] <<= 8;
        conf.reserved1[1] += data[16];
        conf.reserved1[1] <<= 8;
        conf.reserved1[1] += data[17];
        
        conf.reserved2[0] = data[18];
        conf.reserved2[1] = data[19];
        conf.reserved2[2] = data[20];
        conf.reserved2[3] = data[21];
    }
    return conf;
}

//Use this command to send config 2 to the P31and save it.
void Config2_Set(eps_config2_t conf)
{
    uint8_t CONF2_SET_PORT = 0x17;
    char data[20];
    data[0] = (char)conf.batt_maxvoltage & 0xFF;
    data[1] = (char)conf.batt_maxvoltage >> 8;
    data[2] = (char)conf.batt_safevoltage & 0xFF;
    data[3] = (char)conf.batt_safevoltage >> 8;
    data[4] = (char)conf.batt_criticalvoltage & 0xFF;
    data[5] = (char)conf.batt_criticalvoltage >> 8;
    data[6] = (char)conf.batt_normalvoltage & 0xFF;
    data[7] = (char)conf.batt_normalvoltage >> 8;
    data[8] = (char)conf.reserved1[0] & 0xFF;
    data[9] = (char)conf.reserved1[0] >> 8;
    data[10] = (char)conf.reserved1[0] >> 16;
    data[11] = (char)conf.reserved1[0] >> 24;
    data[12] = (char)conf.reserved1[1] & 0xFF;
    data[13] = (char)conf.reserved1[1] >> 8;
    data[14] = (char)conf.reserved1[1] >> 16;
    data[15] = (char)conf.reserved1[1] >> 24;
    data[16] = (char)conf.reserved2[0];
    data[17] = (char)conf.reserved2[1];
    data[18] = (char)conf.reserved2[2];
    data[19] = (char)conf.reserved2[3];

    I2C1_M_Write(PWAddr, CONF2_SET_PORT, sizeof data, data);
}