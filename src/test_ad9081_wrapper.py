import socket
import adi_ad9081_v120 as ad9081

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


def show_link_status(dev: ad9081.Device):
    addrs = [0x55e, 0x5bb]
    retval = {}
    for addr in addrs:
        reg = ad9081.AddrData(addr)
        rc = ad9081.hal_reg_get(dev, reg)
        assert rc == 0
        retval[addr] = reg.data
    return retval


# check API revision
dev = ad9081.Device()
apirev = ad9081.ApiRevision()
rc = ad9081.device_api_revision_get(dev, apirev)
assert rc == 0
print(f"api_revision = {apirev.major}.{apirev.minor}.{apirev.rc}")

# init
dev.spi_conf_set(ad9081.SPI_SDO, ad9081.SPI_MSB_FIRST, ad9081.SPI_ADDR_INC_AUTO)
serdes_setting(dev)
sock = socket.socket(socket.AF_INET, type=socket.SOCK_DGRAM)
dev.sockfd_set(sock.fileno())
dev.target_set("10.5.0.16", 16384)
rc = ad9081.device_init(dev)
assert rc == 0

# check chip ID
chip_id = ad9081.CmsChipId()
rc = ad9081.device_chip_id_get(dev, chip_id)
assert rc == 0
print(f"chip_type = {chip_id.chip_type}, prod_id = {chip_id.prod_id:04x}, prod_grade = {chip_id.prod_grade}, dev_revision = {chip_id.dev_revision}")

# clk_config_set
dac_clk_hz = 12000000000
adc_clk_hz = 6000000000
dev_ref_clk_hz = 12000000000
rc = ad9081.device_clk_config_set(dev, dac_clk_hz, adc_clk_hz, dev_ref_clk_hz)
assert rc == 0

link_status = show_link_status(dev)
print(f"link_status[0x55e] = {link_status[0x55e]:02x}")
print(f"link_status[0x5bb] = {link_status[0x5bb]:02x}")

# ad9081.dac_duc_nco_set(dev, ad9081.DAC_0, ad9081.DAC_CH_NONE, 3000000000)
