import os
import subprocess

import qubelsi.udpsendrecv

class AD9082:

    def __init__(self, addr, path, chip):
        self.addr = addr
        self.chip = str(chip)
        self.path = path
        self.handle = qubelsi.udpsendrecv.UDPSendRecv(addr, path)

    def _setenv(self):
        os.environ['TARGET_ADDR'] = self.addr
        os.environ['AD9082_CHIP'] = self.chip
    
    def read_info(self):
        self._setenv()
        ret = subprocess.check_output("{}/src/print_ad9082_info".format(self.path), encoding='utf-8')
        return ret

    def set_nco(self, freq, ch=-1, adc_mode=False, fine_mode=False, both_mode=False, adc_ch=-1, dac_ch=-1):
        self._setenv()
        cmd = ["{}/src/set_nco".format(self.path),
               "--freq={}".format(freq),
        ]
        
        if both_mode: # set both of ADC and DAC
            cmd.append("--both-mode")
            cmd.append("--adc_channel={}".format(adc_ch))
            cmd.append("--dac_channel={}".format(dac_ch))
        else: # set ADC xor DAC
            cmd.append("--channel={}".format(ch))
            if adc_mode: # set ADC (default; DAC)
                cmd.append("--adc-mode")

        if fine_mode: # set fine-mode (default; coarse-mode)
            cmd.append("--fine-mode")

        ret = subprocess.check_output(cmd, encoding='utf-8')
        return ret

    def set_xbar(self, ch, value):
        self._setenv()
        cmd = ["{}/src/set_xbar".format(self.path),
               "--channel={}".format(ch),
               "--value={:02x}".format(value),
        ]
        ret = subprocess.check_output(cmd, encoding='utf-8')
        return ret

    def set_fsc(self, ch, value, rerun_cal=False):
        self._setenv()
        cmd = ["{}/src/set_fsc".format(self.path),
               "--channel={}".format(ch),
               "--value={}".format(value),
        ]
        if rerun_cal:
            cmd.append("--rerun-cal")
        ret = subprocess.check_output(cmd, encoding='utf-8')
        return ret

    def get_jesd_status(self):
        self._setenv()
        ret = subprocess.check_output("{}/src/get_jesd_status".format(self.path), encoding='utf-8')
        return [x for x in [x.split('=') for x in ret.split('\n')] if len(x) == 2]

    def write_value(self, addr, value):
        return self.handle.write_ad9082(self.chip, addr, value)

    def read_value(self, addr):
        return self.handle.read_ad9082(self.chip, addr)

    def do_init(self, monitor=False, main_intrp=6, message_out=False):
        self._setenv()
        if monitor:
            ret = subprocess.check_output([f"{self.path}/v1.0.6/src/hello_monitor", f"{main_intrp}"], encoding='utf-8')
        else:
            ret = subprocess.check_output([f"{self.path}/v1.0.6/src/hello", f"{main_intrp}"], encoding='utf-8')
        if message_out:
            print(ret)
