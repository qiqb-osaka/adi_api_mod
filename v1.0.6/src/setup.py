from pybind11.setup_helpers import Pybind11Extension
from setuptools import setup

ext_modules = [
    Pybind11Extension(
        "adi_ad9081_v106",
        ["ad9081_wrapper.cpp", "adi_ad9081_adc.c", "adi_ad9081_dac.c", "adi_ad9081_device.c", "adi_ad9081_hal.c", "adi_ad9081_jesd.c", "udpsendrecv.c"],
    )
]

setup(
    ext_modules=ext_modules,
)
