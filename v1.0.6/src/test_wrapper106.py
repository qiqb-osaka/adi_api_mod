import socket
import time
import adi_ad9081_v106 as ad9081


def check_core_status(dev):
    reg3742 = ad9081.AddrData(0x3742)
    rc = ad9081.hal_reg_get(dev, reg3742)
    assert rc == 0
    return reg3742.data

def check_link_status(dev):
    reg055e = ad9081.AddrData(0x055e)
    rc = ad9081.hal_reg_get(dev, reg055e)
    assert rc == 0

    reg05bb = ad9081.AddrData(0x05bb)
    rc = ad9081.hal_reg_get(dev, reg05bb)
    assert rc == 0

    return {
        '055e': reg055e.data, 
        '05bb': reg05bb.data
        }

def serdes_setting(dev: ad9081.Device):
    dev.serdes_info.des_settings.boost_mask = 0xff
    dev.serdes_info.des_settings.invert_mask = 0x00
    for i in range(8):
        dev.serdes_info.des_settings.ctle_filter[i] = 0
        dev.serdes_info.des_settings.lane_mapping0[i] = i
        dev.serdes_info.des_settings.lane_mapping1[i] = i

    dev.serdes_info.ser_settings.invert_mask = 0x00
    for i in range(8):
        dev.serdes_info.ser_settings.lane_mapping0[i] = i
        dev.serdes_info.ser_settings.lane_mapping1[i] = i
        dev.serdes_info.ser_settings.lane_settings[i][ad9081.SWING_SETTING] = ad9081.SER_SWING_850;
        dev.serdes_info.ser_settings.lane_settings[i][ad9081.PRE_EMP_SETTING] = ad9081.SER_PRE_EMP_3DB;
        dev.serdes_info.ser_settings.lane_settings[i][ad9081.POST_EMP_SETTING] = ad9081.SER_POST_EMP_3DB;

# check API revision
dev = ad9081.Device()
apirev = ad9081.ApiRevision()
rc = ad9081.device_api_revision_get(dev, apirev)
assert rc == 0
print(f"api_revision = {apirev.major}.{apirev.minor}.{apirev.rc}")

# init
sock = socket.socket(socket.AF_INET, type=socket.SOCK_DGRAM)
dev.sockfd_set(sock.fileno())
#dev.target_set("10.5.0.16", 16384)
dev.target_set("10.5.0.31", 16384)
serdes_setting(dev)
dev.spi_conf_set(ad9081.SPI_SDO, ad9081.SPI_MSB_FIRST, ad9081.SPI_ADDR_INC_AUTO)
rc = ad9081.device_reset(dev, ad9081.SOFT_RESET)
assert rc == 0

rc = ad9081.device_init(dev)
assert rc == 0

# it takes some time to comlete initialization!
time.sleep(0.1)

chip_id = ad9081.CmsChipId()
rc = ad9081.device_chip_id_get(dev, chip_id)
assert rc == 0
print(f"chip_type = {chip_id.chip_type}, prod_id = {chip_id.prod_id:04x}, prod_grade = {chip_id.prod_grade}, dev_revision = {chip_id.dev_revision}")

# XXX: reduced frequency for using 204B transceiver configuration.
dac_clk_hz = 11640000000
adc_clk_hz = 2910000000*2
dev_ref_clk_hz =11640000000
rc = ad9081.device_clk_config_set(dev, dac_clk_hz, adc_clk_hz, dev_ref_clk_hz)
assert rc == 0

tx_main_interp = 6
tx_chan_interp = 4

# for AD9082 #0
tx_dac_chan = [0x01, 0x02, 0x1c, 0xe0]

# for AD9082 #1
# tx_dac_chan = [0x07, 0x38, 0x40, 0x80]

shift = 970000000 # 970MHz
tx_main_shift = [shift, shift, shift, shift]
tx_chan_shift = [0, 0, 0, 0, 0, 0, 0, 0]

jrx_param0 = ad9081.AdiCmsJesdParam()
jrx_param0.l = 8
jrx_param0.f = 4
jrx_param0.m = 16
jrx_param0.s = 1
jrx_param0.hd = 0
jrx_param0.k = 64
jrx_param0.n = 16
jrx_param0.np = 16
jrx_param0.cf = 0
jrx_param0.cs = 0
jrx_param0.did = 0
jrx_param0.bid = 0
jrx_param0.lid0 = 0
jrx_param0.subclass = 0
jrx_param0.scr = 1
jrx_param0.duallink = 0
jrx_param0.jesdv = 2
jrx_param0.mode_id = 16
jrx_param0.mode_c2r_en = 0
jrx_param0.mode_s_sel = 0

rc = ad9081.device_startup_tx(dev, tx_main_interp, tx_chan_interp, tx_dac_chan, tx_main_shift, tx_chan_shift, jrx_param0)
assert rc == 0
for i in range(8):
    rc = ad9081.jesd_rx_lane_xbar_set(dev, ad9081.LINK_0, i, i)
    assert rc == 0

# TODO: check what it is.
reg0401 = ad9081.AddrData(0x0401, 0x00)
rc = ad9081.hal_reg_set(dev, reg0401)
assert rc == 0

rc = ad9081.jesd_rx_link_enable_set(dev, ad9081.LINK_0, 1)
assert rc == 0

# TODO: check what they are.
reg001b = ad9081.AddrData(0x001b, 0x0f)
rc = ad9081.hal_reg_set(dev, reg001b)
assert rc == 0

reg0117 = ad9081.AddrData(0x0117, 0xa0)
rc = ad9081.hal_reg_set(dev, reg0117)
assert rc == 0

reg0118 = ad9081.AddrData(0x0118, 0xff)
rc = ad9081.hal_reg_set(dev, reg0118)
assert rc == 0


rx_cddc_select = ad9081.ADC_CDDC_ALL
cddc_val = 970000000
cddc_shift = [cddc_val, cddc_val, cddc_val, cddc_val]
cddc_dcm = [ad9081.ADC_CDDC_DCM_6, ad9081.ADC_CDDC_DCM_6, ad9081.ADC_CDDC_DCM_6, ad9081.ADC_CDDC_DCM_6]
rx_cddc_c2r = [0, 0, 0, 0]

rx_fddc_select = ad9081.ADC_FDDC_ALL
fddc_val = 0
fddc_shift = [fddc_val, fddc_val, fddc_val, fddc_val, fddc_val, fddc_val, fddc_val, fddc_val]
fddc_dcm = [ad9081.ADC_FDDC_DCM_2, ad9081.ADC_FDDC_DCM_2, ad9081.ADC_FDDC_DCM_2, ad9081.ADC_FDDC_DCM_2, 
            ad9081.ADC_FDDC_DCM_2, ad9081.ADC_FDDC_DCM_2, ad9081.ADC_FDDC_DCM_2, ad9081.ADC_FDDC_DCM_2]
rx_fddc_c2r = [0, 0, 0, 0, 0, 0, 0, 0]

jtx_param0 = ad9081.AdiCmsJesdParam()
jtx_param0.l = 8
jtx_param0.f = 4
jtx_param0.m = 16
jtx_param0.s = 1
jtx_param0.hd = 0
jtx_param0.k = 64
jtx_param0.n = 16
jtx_param0.np = 16
jtx_param0.cf = 0
jtx_param0.cs = 0
jtx_param0.did = 0
jtx_param0.bid = 0
jtx_param0.lid0 = 0
jtx_param0.subclass = 0
jtx_param0.scr = 1
jtx_param0.duallink = 0
jtx_param0.jesdv = 2
jtx_param0.mode_id = 17
jtx_param0.mode_c2r_en = 0
jtx_param0.mode_s_sel = 0

jtx_param1 = ad9081.AdiCmsJesdParam()
jtx_param1.l = 8
jtx_param1.f = 4
jtx_param1.m = 16
jtx_param1.s = 1
jtx_param1.hd = 0
jtx_param1.k = 64
jtx_param1.n = 16
jtx_param1.np = 16
jtx_param1.cf = 0
jtx_param1.cs = 0
jtx_param1.did = 0
jtx_param1.bid = 0
jtx_param1.lid0 = 0
jtx_param1.subclass = 0
jtx_param1.scr = 1
jtx_param1.duallink = 0
jtx_param1.jesdv = 2
jtx_param1.mode_id = 17
jtx_param1.mode_c2r_en = 0
jtx_param1.mode_s_sel = 0

jesd_conv_sel0 = ad9081.JtxConvSel()
jesd_conv_sel0.virtual_converter0_index = 0
jesd_conv_sel0.virtual_converter1_index = 1
jesd_conv_sel0.virtual_converter2_index = 2
jesd_conv_sel0.virtual_converter3_index = 3
jesd_conv_sel0.virtual_converter4_index = 4
jesd_conv_sel0.virtual_converter5_index = 5
jesd_conv_sel0.virtual_converter6_index = 6
jesd_conv_sel0.virtual_converter7_index = 7
jesd_conv_sel0.virtual_converter8_index = 8
jesd_conv_sel0.virtual_converter9_index = 9
jesd_conv_sel0.virtual_convertera_index = 10
jesd_conv_sel0.virtual_converterb_index = 11
jesd_conv_sel0.virtual_converterc_index = 12
jesd_conv_sel0.virtual_converterd_index = 13
jesd_conv_sel0.virtual_convertere_index = 14
jesd_conv_sel0.virtual_converterf_index = 15

jesd_conv_sel1 = ad9081.JtxConvSel()
jesd_conv_sel1.virtual_converter0_index = 0
jesd_conv_sel1.virtual_converter1_index = 1
jesd_conv_sel1.virtual_converter2_index = 2
jesd_conv_sel1.virtual_converter3_index = 3
jesd_conv_sel1.virtual_converter4_index = 4
jesd_conv_sel1.virtual_converter5_index = 5
jesd_conv_sel1.virtual_converter6_index = 6
jesd_conv_sel1.virtual_converter7_index = 7
jesd_conv_sel1.virtual_converter8_index = 8
jesd_conv_sel1.virtual_converter9_index = 9
jesd_conv_sel1.virtual_convertera_index = 10
jesd_conv_sel1.virtual_converterb_index = 11
jesd_conv_sel1.virtual_converterc_index = 12
jesd_conv_sel1.virtual_converterd_index = 13
jesd_conv_sel1.virtual_convertere_index = 14
jesd_conv_sel1.virtual_converterf_index = 15

rc = ad9081.device_startup_rx(dev, rx_cddc_select, rx_fddc_select,
                              cddc_shift, fddc_shift, cddc_dcm, fddc_dcm,
                              rx_cddc_c2r, rx_fddc_c2r,
                              (jtx_param0, jtx_param1), (jesd_conv_sel0, jesd_conv_sel1))
assert rc == 0

lane = [0, 1, 2, 3, 4, 5, 6, 7]
rc = ad9081.jesd_tx_lanes_xbar_set(dev, ad9081.LINK_0, lane)
assert rc == 0

lid = [0, 0, 0, 0, 0, 0, 0, 0]
rc = ad9081.jesd_tx_lids_cfg_set(dev, ad9081.LINK_0, lid)
assert rc == 0
rc = ad9081.jesd_tx_link_enable_set(dev, ad9081.LINK_0, 1)
assert rc == 0

# GTH reset
reg05bb = ad9081.AddrData(0x05bb, 0x00)
rc = ad9081.hal_reg_set(dev, reg05bb)
assert rc == 0
reg05bb.data = 0x01
rc = ad9081.hal_reg_set(dev, reg05bb)
assert rc == 0

print(check_link_status(dev))

reg067x = ad9081.AddrData(0x0670)
for i in range(0x670, 0x678):
    reg067x.addr = i
    ad9081.hal_reg_get(dev, reg067x)
    print(f"{reg067x.addr:04x}: {reg067x.data:02x}")
for i in (0x702, 0x728, 0x0ca):
    reg067x.addr = i
    ad9081.hal_reg_get(dev, reg067x)
    print(f"{reg067x.addr:04x}: {reg067x.data:02x}")
