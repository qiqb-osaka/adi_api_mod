#ifdef GET_JESD_STATUS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "adi_ad9081_config.h"
#include "adi_ad9081_hal.h"
#define MHZ 1000000

#include "udpsendrecv.h"

void SetDevinfo(adi_ad9081_device_t *ad9081_dev){
    int i;
    ad9081_dev->hal_info.sdo=SPI_SDO;
    ad9081_dev->hal_info.msb=SPI_MSB_FIRST;
    ad9081_dev->hal_info.addr_inc=SPI_ADDR_INC_AUTO;

    ad9081_dev->serdes_info.des_settings.boost_mask=0xff;
    ad9081_dev->serdes_info.des_settings.invert_mask=0x00;
    for(i=0;i<8;i++){
        ad9081_dev->serdes_info.des_settings.ctle_filter[i]=0;
        ad9081_dev->serdes_info.des_settings.lane_mapping[0][i]=i;
        ad9081_dev->serdes_info.des_settings.lane_mapping[1][i]=i;
    }
    ad9081_dev->serdes_info.ser_settings.invert_mask=0x00;
    for(i=0;i<8;i++){
        ad9081_dev->serdes_info.ser_settings.lane_mapping[0][i]=i;
        ad9081_dev->serdes_info.ser_settings.lane_mapping[1][i]=i;
        ad9081_dev->serdes_info.ser_settings.lane_settings[i].post_emp_setting=AD9081_SER_POST_EMP_3DB;
        ad9081_dev->serdes_info.ser_settings.lane_settings[i].pre_emp_setting=AD9081_SER_PRE_EMP_3DB;
        ad9081_dev->serdes_info.ser_settings.lane_settings[i].swing_setting=AD9081_SER_SWING_850;
    }
}

void ad9082_print_info(adi_ad9081_device_t *ad9081_dev)
{
    uint8_t reg_data;
    adi_ad9081_hal_reg_get(ad9081_dev, 0x3, &reg_data);
    printf("CHIP_TYPE     = %02x\n", reg_data);
    adi_ad9081_hal_reg_get(ad9081_dev, 0x4, &reg_data);
    printf("PROD_ID_LSB   = %02x\n", reg_data);
    adi_ad9081_hal_reg_get(ad9081_dev, 0x5, &reg_data);
    printf("PROD_ID_MSB   = %02x\n", reg_data);
    adi_ad9081_hal_reg_get(ad9081_dev, 0x6, &reg_data);
    printf("CHIP_GRADE    = %02x\n", reg_data);
    adi_ad9081_hal_reg_get(ad9081_dev, 0xb, &reg_data);
    printf("SPI_REVISION  = %02x\n", reg_data);
    adi_ad9081_hal_reg_get(ad9081_dev, 0xc, &reg_data);
    printf("VENDER_ID_LSB = %02x\n", reg_data);
    adi_ad9081_hal_reg_get(ad9081_dev, 0xd, &reg_data);
    printf("VENDER_ID_MSB = %02x\n", reg_data);
}

void ad9082_setup(adi_ad9081_device_t *ad9081_dev)
{
    int i;
    uint8_t reg_data;
#if 0
    //uint64_t dac_clk_hz =     11640000000;
    //uint64_t adc_clk_hz =     2910000000;
    //uint64_t dev_ref_clk_hz = 11640000000;
    uint64_t dac_clk_hz =     12000000000;
    uint64_t adc_clk_hz =     3000000000;
    uint64_t dev_ref_clk_hz = 12000000000;
    //uint64_t dac_clk_hz =     9600000000;
    //uint64_t adc_clk_hz =     2400000000;
    //uint64_t dev_ref_clk_hz = 9600000000;

    int ret;

    // DEVICE INITIALIZATION AND CLOCK CONFIGURATION
    uint8_t rev_major, rev_minor, rev_rc;
    adi_ad9081_device_api_revision_get(ad9081_dev, &rev_major, &rev_minor, &rev_rc);
    printf("API Rev: %d.%d.%d\n", rev_major, rev_minor, rev_rc);
    
    ret = adi_ad9081_device_reset(ad9081_dev, AD9081_SOFT_RESET);
    ret = adi_ad9081_device_init(ad9081_dev);
    if(ret < 0){ printf("device init error\n"); return; }
    ret = adi_ad9081_device_clk_config_set(ad9081_dev, dac_clk_hz, adc_clk_hz, dev_ref_clk_hz);
    if(ret < 0){ printf("clk_config_set error\n"); return; }

    ad9082_print_info(ad9081_dev);

    // Tx CONFIGURATION
    uint8_t tx_main_interp = 4;
    uint8_t tx_chan_interp = 6;
    uint8_t tx_dac_chan[] = {0x1, 0x2, 0x1c, 0xe0};
    int64_t tx_main_shift[] = {1000*MHZ, 1000*MHZ, 1000*MHZ, 1000*MHZ };
    int64_t tx_chan_shift[] = {0*MHZ, 0*MHZ, 0*MHZ, 0*MHZ, 0*MHZ, 0*MHZ, 0*MHZ, 0*MHZ };
    adi_cms_jesd_param_t jrx_param[2] =
        //  L  F   M  S HD   K   N  NP CF CS DID BID LID SCL SCR DUAL  B/C  ID  C2R S
        //{ { 1, 4, 2, 1, 0, 64, 16, 16, 0, 0, 0,  0, 0,   0,  0, 0,    2,   2,   0,  0 },
        //  { 1, 4, 2, 1, 0, 64, 16, 16, 0, 0, 0,  0, 0,   0,  0, 0,    2,   2,   0,  0 } };
        { { 8, 4, 16, 1, 0, 64, 16, 16, 0, 0, 0,  0, 0,   0,  0, 0,    2,   16,   0,  0 },
          { 8, 4, 16, 1, 0, 64, 16, 16, 0, 0, 0,  0, 0,   0,  0, 0,    2,   16,   0,  0 } };

    for(i = 0; i < 8; i++){
       adi_ad9081_jesd_rx_lane_xbar_set(ad9081_dev, AD9081_LINK_0, i, i);
    }
    adi_ad9081_device_startup_tx(ad9081_dev, tx_main_interp, tx_chan_interp,
                                 tx_dac_chan, tx_main_shift, tx_chan_shift, jrx_param);

    uint16_t tx_gains[] = {1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000};
    adi_ad9081_dac_duc_nco_gains_set(ad9081_dev, tx_gains);
    adi_ad9081_dac_mode_set(ad9081_dev, AD9081_DAC_MODE_SWITCH_GROUP_ALL, AD9081_DAC_MODE_0);
    //adi_ad9081_dac_xbar_set(ad9081_dev, AD9081_DAC_0, 0x01);
    //adi_ad9081_dac_xbar_set(ad9081_dev, AD9081_DAC_1, 0x01);
    //adi_ad9081_dac_xbar_set(ad9081_dev, AD9081_DAC_2, 0x01);
    //adi_ad9081_dac_xbar_set(ad9081_dev, AD9081_DAC_3, 0x01);
    adi_ad9081_dac_fsc_set(ad9081_dev, AD9081_DAC_ALL, 26000);
    adi_ad9081_dac_duc_nco_set(ad9081_dev, AD9081_DAC_ALL, AD9081_DAC_CH_ALL, 3000000000);

    // Rx CONFIGURATION
    uint8_t rx_cddc_select = AD9081_ADC_CDDC_ALL;
    uint8_t rx_fddc_select = AD9081_ADC_FDDC_ALL;
    int64_t cdcc_shift[]={0,250000000,500000000,750000000};
    int64_t fdcc_shift[]={0,100000000,0,100000000,0,100000000,0,100000000};
    uint8_t cddc_dcm[] = { AD9081_CDDC_DCM_3,AD9081_CDDC_DCM_3,AD9081_CDDC_DCM_3,AD9081_CDDC_DCM_3};
    uint8_t fddc_dcm[]={AD9081_FDDC_DCM_1,AD9081_FDDC_DCM_1,AD9081_FDDC_DCM_1,AD9081_FDDC_DCM_1,AD9081_FDDC_DCM_1,AD9081_FDDC_DCM_1,AD9081_FDDC_DCM_1,AD9081_FDDC_DCM_1};
    uint8_t rx_cddc_c2r[]={0x1,0x1,0x1,0x1};
    uint8_t rx_fddc_c2r[]={0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1};

    adi_cms_jesd_param_t jtx_param[2] =
        //  L  F  M  S HD   K   N  NP CF CS DID BID LID SCL SCR DUAL  B/C  ID  C2R S
        { { 8, 2, 8, 1, 0, 128,16, 16, 0, 0, 0,  0, 0,   0,  1, 0,    2,   16, 0,  0 },
	  { 8, 2, 8, 1, 0, 128,16, 16, 0, 0, 0,  0, 0,   0,  1, 0,    2,   16, 0,  0 } };
    adi_ad9081_jtx_conv_sel_t jesd_conv_sel[2]={{0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7},{0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,7}};

    //printf("0>"); getchar();
    adi_ad9081_device_startup_rx(ad9081_dev,
                                 rx_cddc_select,
                                 rx_fddc_select,
                                 cdcc_shift,
                                 fdcc_shift,
                                 cddc_dcm,
                                 fddc_dcm,
                                 rx_cddc_c2r,
                                 rx_fddc_c2r,
                                 jtx_param,
                                 jesd_conv_sel);
    //printf("1>"); getchar();
    adi_ad9081_adc_ddc_coarse_nco_mode_set(ad9081_dev, 0xff, AD9081_ADC_NCO_VIF);
    //printf("2>"); getchar();
    uint8_t adc_cddc_xbar, cddc_fddc_xbar;
    adc_cddc_xbar = AD9081_ADC_2_ADC_COMP_MODE;
    cddc_fddc_xbar = AD9081_ADC_CDDC0_TO_FDDC0 | AD9081_ADC_CDDC1_TO_FDDC1 |
                     AD9081_ADC_CDDC0_TO_FDDC2 | AD9081_ADC_CDDC1_TO_FDDC3;
    cddc_fddc_xbar |= AD9081_ADC_CDDC2_TO_FDDC4 |
                      AD9081_ADC_CDDC3_TO_FDDC5 |
                      AD9081_ADC_CDDC2_TO_FDDC6 | AD9081_ADC_CDDC3_TO_FDDC7;
    adi_ad9081_adc_xbar_set(ad9081_dev, adc_cddc_xbar, cddc_fddc_xbar);
    //printf("3>"); getchar();
    adi_ad9081_jesd_tx_fbw_sel_set(ad9081_dev, AD9081_LINK_ALL, 0xFFFF);
    //printf("4>"); getchar();
    adi_ad9081_adc_nyquist_zone_set(ad9081_dev, AD9081_ADC_NYQUIST_ZONE_ODD);
    //printf("5>"); getchar();
    adi_ad9081_adc_ddc_fine_gain_set(ad9081_dev, AD9081_ADC_FDDC_ALL, 0);
    //printf("6>"); getchar();

    // LINK ESTABLISHMENT AND MONITORING
    adi_ad9081_adc_nco_master_slave_sync(ad9081_dev, 0, 0, 0, 0);
    //printf("7>"); getchar();
    adi_ad9081_jesd_tx_link_enable_set(ad9081_dev, AD9081_LINK_ALL, 1);
    //printf("8>"); getchar();
    adi_ad9081_jesd_rx_link_enable_set(ad9081_dev, AD9081_LINK_ALL, 1);
    //printf("9>"); getchar();
    adi_ad9081_jesd_rx_calibrate_204c(ad9081_dev, 1, 0x00, 0);
    //printf("10>"); getchar();
    adi_ad9081_jesd_rx_link_enable_set(ad9081_dev, AD9081_LINK_ALL, 1);
    //printf("11>"); getchar();

#endif

    for(i=0;i<4;i++){
        adi_ad9081_hal_reg_set(ad9081_dev, 0x5BB, 0x00);
        adi_ad9081_hal_reg_set(ad9081_dev, 0x5BB, 0x01);
        
    	adi_ad9081_hal_reg_get(ad9081_dev,0x55E,&reg_data);
    	printf("0x55E=%X\n",reg_data);
        adi_ad9081_hal_reg_get(ad9081_dev, 0x5BB, &reg_data);
        printf("0x5BB=%X\n", reg_data);
        adi_ad9081_hal_reg_get(ad9081_dev, 0x62E, &reg_data);
        printf("0x62E=%X\n", reg_data);
    }

    uint8_t pll_status;
    adi_ad9081_jesd_pll_lock_status_get(ad9081_dev, &pll_status);
    printf("pll_status=%02x\n", pll_status);

    //adi_ad9081_device_direct_loopback_set(ad9081_dev, 1, 0xFF);
    //adi_ad9081_jesd_loopback_mode_set(ad9081_dev, 1);
}

int main()
{
    adi_ad9081_device_t ad9081_dev;

    char *target_addr = "10.0.0.3";
    char *val = getenv("TARGET_ADDR");
    if(val != NULL){
        target_addr = val;
    }
    //printf("target addr:%s\n", target_addr);
    open_socket(&ad9081_dev.udp_env_info, target_addr, 16384);

    SetDevinfo(&ad9081_dev);
    
    ad9082_setup(&ad9081_dev);

    ad9082_print_info(&ad9081_dev);

    close_socket(&ad9081_dev.udp_env_info);

    return 0;
}
#endif /* GET_JESD_STATUS */