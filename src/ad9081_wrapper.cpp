#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "adi_ad9081_config.h"
#include "adi_ad9081_hal.h"

namespace py = pybind11;

typedef struct api_revision {
    uint8_t rev_major;
    uint8_t rev_minor;
    uint8_t rev_rc;
} api_revision_t;

typedef struct addr_data {
    uint32_t addr;
    uint8_t data;
} addr_data_t;

typedef struct nco_ftw {
    uint64_t converter_freq;
    uint64_t nco_shift;
    uint64_t ftw;
} nco_ftw_t;


PYBIND11_MODULE(adi_ad9081_v120, m) {

    py::class_<adi_cms_chip_id_t>(m, "CmsChipId")
        .def(py::init<>())
        .def_readonly("chip_type", &adi_cms_chip_id_t::chip_type)
        .def_readonly("prod_id", &adi_cms_chip_id_t::prod_id)
        .def_readonly("prod_grade", &adi_cms_chip_id_t::prod_grade)
        .def_readonly("dev_revision", &adi_cms_chip_id_t::dev_revision)
        ;

    py::enum_<adi_ad9081_reset_e>(m, "Reset", py::arithmetic())
        .value("SOFT_RESET", AD9081_SOFT_RESET, "Soft Reset")
        .value("HARD_RESET", AD9081_HARD_RESET, "Hard Reset")
        .value("SOFT_RESET_AND_INIT", AD9081_SOFT_RESET_AND_INIT, "Soft Reset Then Init")
        .value("HARD_RESET_AND_INIT", AD9081_HARD_RESET_AND_INIT, "Hard Reset Then Init")
        .export_values()
        ;

    py::class_<adi_ad9081_info_t>(m, "Info")
        .def(py::init<>())
        .def_readwrite("dev_freq_hz", &adi_ad9081_info_t::dev_freq_hz)
        .def_readwrite("dac_freq_hz", &adi_ad9081_info_t::dac_freq_hz)
        .def_readwrite("adc_freq_hz", &adi_ad9081_info_t::adc_freq_hz)
        .def_readwrite("dev_rev", &adi_ad9081_info_t::dev_rev)
        .def_readwrite("prod_id", &adi_ad9081_info_t::prod_id)
        ;

    py::enum_<adi_ad9081_ser_swing_e>(m, "SerSwing", py::arithmetic())
        .value("SER_SWING_1000", AD9081_SER_SWING_1000, "1000 mV Swing")
        .value("SER_SWING_850", AD9081_SER_SWING_850, "850 mV Swing")
        .value("SER_SWING_750", AD9081_SER_SWING_750, "750 mV Swing")
        .value("SER_SWING_500", AD9081_SER_SWING_500, "500 mV Swing")
        .export_values()
        ;

    py::enum_<adi_ad9081_ser_pre_emp_e>(m, "SerPreEmp", py::arithmetic())
        .value("SER_PRE_EMP_0DB", AD9081_SER_PRE_EMP_0DB, "0 dB Pre-Emphasis")
        .value("SER_PRE_EMP_3DB", AD9081_SER_PRE_EMP_3DB, "3 dB Pre-Emphasis")
        .value("SER_PRE_EMP_6DB", AD9081_SER_PRE_EMP_6DB, "6 dB Pre-Emphasis")
        .export_values()
        ;

    py::enum_<adi_ad9081_ser_post_emp_e>(m, "SerPostEmp", py::arithmetic())
        .value("SER_POST_EMP_0DB", AD9081_SER_POST_EMP_0DB, "0 dB Post-Emphasis")
        .value("SER_POST_EMP_3DB", AD9081_SER_POST_EMP_3DB, "3 dB Post-Emphasis")
        .value("SER_POST_EMP_6DB", AD9081_SER_POST_EMP_6DB, "6 dB Post-Emphasis")
        .value("SER_POST_EMP_9DB", AD9081_SER_POST_EMP_9DB, "9 dB Post-Emphasis")
        .value("SER_POST_EMP_12DB", AD9081_SER_POST_EMP_12DB, "12 dB Post-Emphasis")                                
        .export_values()
        ;

    py::class_<adi_ad9081_ser_lane_settings_t>(m, "SerLaneSettings")
        .def(py::init<>())
        .def_readwrite("swing_setting", &adi_ad9081_ser_lane_settings_t::swing_setting)
        .def_readwrite("pre_emp_setting", &adi_ad9081_ser_lane_settings_t::pre_emp_setting)
        .def_readwrite("post_emp_setting", &adi_ad9081_ser_lane_settings_t::post_emp_setting)
        ;
    PYBIND11_NUMPY_DTYPE(adi_ad9081_ser_lane_settings_t, swing_setting, pre_emp_setting, post_emp_setting);
    typedef enum { SWING_SETTING = 0, PRE_EMP_SETTING = 1, POST_EMP_SETTING = 2 } ser_lane_settings_field_e;
    py::enum_<ser_lane_settings_field_e>(m, "SerLaneSettingsField", py::arithmetic())
        .value("SWING_SETTING", SWING_SETTING)
        .value("PRE_EMP_SETTING", PRE_EMP_SETTING)
        .value("POST_EMP_SETTING", POST_EMP_SETTING)
        .export_values()
        ;

    py::class_<adi_ad9081_ser_settings_t>(m, "SerSettings")
        .def(py::init<>())
        .def_readwrite("invert_mask", &adi_ad9081_ser_settings_t::invert_mask)
        .def_property_readonly("lane_mapping0", [](py::object& obj) {
            adi_ad9081_ser_settings_t& o = obj.cast<adi_ad9081_ser_settings_t&>();
            return py::array{8, o.lane_mapping[0], obj};
        })
        .def_property_readonly("lane_mapping1", [](py::object& obj) {
            adi_ad9081_ser_settings_t& o = obj.cast<adi_ad9081_ser_settings_t&>();
            return py::array{8, o.lane_mapping[1], obj};
        })
        .def_property_readonly("lane_settings", [](py::object& obj) {
            adi_ad9081_ser_settings_t& o = obj.cast<adi_ad9081_ser_settings_t&>();
            return py::array{8, o.lane_settings, obj};
        })        
        ;

    py::class_<adi_ad9081_des_settings_t>(m, "DesSettings")
        .def(py::init<>())
        .def_readwrite("boost_mask", &adi_ad9081_des_settings_t::boost_mask)
        .def_readwrite("invert_mask", &adi_ad9081_des_settings_t::invert_mask)
        .def_property_readonly("ctle_filter", [](py::object& obj) {
            adi_ad9081_des_settings_t& o = obj.cast<adi_ad9081_des_settings_t&>();
            return py::array{8, o.ctle_filter, obj};
        })
        .def_property_readonly("lane_mapping0", [](py::object& obj) {
            adi_ad9081_des_settings_t& o = obj.cast<adi_ad9081_des_settings_t&>();
            return py::array{8, o.lane_mapping[0], obj};
        })
        .def_property_readonly("lane_mapping1", [](py::object& obj) {
            adi_ad9081_des_settings_t& o = obj.cast<adi_ad9081_des_settings_t&>();
            return py::array{8, o.lane_mapping[1], obj};
        })
        ;

    py::class_<adi_ad9081_serdes_settings_t>(m, "SerdesSettings")
        .def(py::init<>())
        .def_readwrite("ser_settings", &adi_ad9081_serdes_settings_t::ser_settings)
        .def_readwrite("des_settings", &adi_ad9081_serdes_settings_t::des_settings)
        ;

    py::enum_<adi_cms_spi_sdo_config_e>(m, "CmsSpiSdoConfig", py::arithmetic())
        .value("SPI_NONE", SPI_NONE, "keep this for test")
        .value("SPI_SDO", SPI_SDO, "SDO active, 4-wire only")
        .value("SPI_SDIO", SPI_SDIO, "SDIO active, 3-wire only")
        .export_values()
        ;

    py::enum_<adi_cms_spi_msb_config_e>(m, "CmsSpiMsbConfig", py::arithmetic())
        .value("SPI_MSB_LAST", SPI_MSB_LAST, "LSB first")
        .value("SPI_MSB_FIRST", SPI_MSB_FIRST, "MSB first")
        .export_values()
        ;

    py::enum_<adi_cms_spi_addr_inc_e>(m, "CmsSpiAddrInc", py::arithmetic())
        .value("SPI_ADDR_DEC_AUTO", SPI_ADDR_DEC_AUTO, "auto decremented")
        .value("SPI_ADDR_INC_AUTO", SPI_ADDR_INC_AUTO, "auto incremented")
        .export_values()
        ;

    // TODO: target_set and spi_conf_set will be revised after the refactoring of the hal-layer design.
    py::class_<adi_ad9081_device_t>(m, "Device")
        .def(py::init<>())
        .def_readwrite("dev_info", &adi_ad9081_device_t::dev_info)
        .def_readwrite("serdes_info", &adi_ad9081_device_t::serdes_info)
        .def("sockfd_set", [](adi_ad9081_device_t& self, int sockfd) {
            if (sockfd < 0) 
                throw std::invalid_argument("an invalid sockfd " + std::to_string(sockfd) + " is given");
            self.udp_env_info.sock = sockfd;
        })
        .def("target_set", [](adi_ad9081_device_t& self, const char *addr, int port) {
            if (inet_aton(addr, &self.udp_env_info.addr.sin_addr) == 0) {
                throw std::invalid_argument("an invalid IP address is given");
            }
            self.udp_env_info.addr.sin_family = AF_INET;
            self.udp_env_info.addr.sin_port = htons(port);
        })
        .def("spi_conf_set", [](adi_ad9081_device_t& self, adi_cms_spi_sdo_config_e sdo, adi_cms_spi_msb_config_e msb, adi_cms_spi_addr_inc_e addr_inc) {
            self.hal_info.sdo = sdo;
            self.hal_info.msb = msb;
            self.hal_info.addr_inc = addr_inc;
        })
        ;

    py::class_<api_revision_t>(m, "ApiRevision")
        .def(py::init<>())
        .def_readonly("major", &api_revision_t::rev_major)
        .def_readonly("minor", &api_revision_t::rev_minor)
        .def_readonly("rc", &api_revision_t::rev_rc)
        ;
    m.def("device_api_revision_get", 
        [](adi_ad9081_device_t* device, api_revision_t* rev) {
            return adi_ad9081_device_api_revision_get(device, &(rev->rev_major), &(rev->rev_minor), &(rev->rev_rc));
        });

    m.def("device_reset", adi_ad9081_device_reset);
    m.def("device_init", adi_ad9081_device_init);
    m.def("device_chip_id_get", adi_ad9081_device_chip_id_get);
    m.def("device_clk_config_set", adi_ad9081_device_clk_config_set);

    py::class_<addr_data_t>(m, "AddrData")
        .def(py::init([](uint32_t addr) {
                auto p = std::unique_ptr<addr_data_t>(new addr_data_t{addr, 0U});
                return p;
            }))
        .def(py::init([](uint32_t addr, uint8_t data) {
                auto p = std::unique_ptr<addr_data_t>(new addr_data_t{addr, data});
                return p;
            }))
        .def_readwrite("addr", &addr_data_t::addr)
        .def_readwrite("data", &addr_data_t::data)
        ;
    m.def("hal_reg_get", [](adi_ad9081_device_t* device, addr_data_t* reg) {
        return adi_ad9081_hal_reg_get(device, reg->addr, &(reg->data));
    });
    m.def("hal_reg_set", [](adi_ad9081_device_t* device, addr_data_t* reg) {
        // adi_ad9081_hal_reg_set() takes int32_t as its third argument.
        // however, it is fine for us to use int8_t because revision 3 chip doesn't use 32bit registers.
        return adi_ad9081_hal_reg_set(device, reg->addr, reg->data);
    });

    py::class_<nco_ftw>(m, "NcoFtw")
        .def(py::init<>())
        .def_readwrite("converter_freq", &nco_ftw::converter_freq)
        .def_readwrite("nco_shift", &nco_ftw::nco_shift)
        .def_readonly("ftw", &nco_ftw::ftw)
        ;
    m.def("hal_calc_tx_nco_ftw", [](adi_ad9081_device_t* device, nco_ftw_t* nco_ftw) {
        return adi_ad9081_hal_calc_tx_nco_ftw(device, nco_ftw->converter_freq, nco_ftw->nco_shift, &(nco_ftw->ftw));
    });
    m.def("hal_calc_rx_nco_ftw", [](adi_ad9081_device_t* device, nco_ftw_t* nco_ftw) {
        return adi_ad9081_hal_calc_rx_nco_ftw(device, nco_ftw->converter_freq, nco_ftw->nco_shift, &(nco_ftw->ftw));
    });

    py::enum_<adi_ad9081_dac_select_e>(m, "DacSelect", py::arithmetic())
        .value("DAC_NONE", AD9081_DAC_NONE)
        .value("DAC_0", AD9081_DAC_0)
        .value("DAC_1", AD9081_DAC_1)
        .value("DAC_2", AD9081_DAC_2)
        .value("DAC_3", AD9081_DAC_3)
        .value("DAC_ALL", AD9081_DAC_ALL)
        .export_values()
        ;
    py::enum_<adi_ad9081_dac_channel_select_e>(m, "DacChannelSelect", py::arithmetic())
        .value("DAC_CH_NONE", AD9081_DAC_CH_NONE)
        .value("DAC_CH_0", AD9081_DAC_CH_0)
        .value("DAC_CH_1", AD9081_DAC_CH_1)
        .value("DAC_CH_2", AD9081_DAC_CH_2)
        .value("DAC_CH_3", AD9081_DAC_CH_3)
        .value("DAC_CH_4", AD9081_DAC_CH_4)
        .value("DAC_CH_5", AD9081_DAC_CH_5)
        .value("DAC_CH_6", AD9081_DAC_CH_6)
        .value("DAC_CH_7", AD9081_DAC_CH_7)
        .value("DAC_CH_ALL", AD9081_DAC_CH_ALL)
        .export_values()
        ;
    m.def("dac_duc_nco_set", adi_ad9081_dac_duc_nco_set);

#if 0
    // m.def("dac_duc_nco_enable_set", adi_ad9081_dac_duc_nco_enable_set);
    m.def("dac_duc_nco_ftw_set", adi_ad9081_dac_duc_nco_ftw_set);
    m.def("dac_xbar_set", adi_ad9081_dac_xbar_set);
    m.def("dac_fsc_set", adi_ad9081_dac_fsc_set);

    m.def("adc_ddc_coarse_nco_mode_set", adi_ad9081_adc_ddc_coarse_nco_mode_set);
    m.def("adc_ddc_coarse_nco_set", adi_ad9081_adc_ddc_coarse_nco_set);
    m.def("adc_ddc_coarse_nco_ftw_set", adi_ad9081_adc_ddc_coarse_nco_ftw_set);
    m.def("adc_ddc_coarse_dcm_decode", adi_ad9081_adc_ddc_coarse_dcm_decode);    

    m.def("adc_ddc_fine_nco_mode_set", adi_ad9081_adc_ddc_fine_nco_mode_set);
    m.def("adc_ddc_fine_nco_ftw_set", adi_ad9081_adc_ddc_fine_nco_ftw_set);

    m.def("jesd_tx_conv_sel_set", adi_ad9081_jesd_tx_conv_sel_set);
#endif
}
