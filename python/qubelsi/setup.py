import setuptools
 
setuptools.setup(
    name="qubelsi",
    version="1.0",
    author="miyo",
    author_email="miyo@wasamon.net",
    description="QuBE LSIs manipulator",
    long_description="QuBE LSIs manipulator",
    long_description_content_type="text/markdown",
    packages=setuptools.find_packages(),
    scripts=[
        "scripts/get_qubelsi_path.sh",
        "scripts/setup_quel1.sh",
        "scripts/setup_quel1_monitor.sh",
        "scripts/get_jesd_status.sh"
    ],
    package_data={
        "qubelsi": ["bin/v1.0.6/src/hello", "bin/v1.0.6/src/hello_monitor",
                    "bin/src/udpsendrecv", "bin/src/get_jesd_status", "bin/src/print_ad9082_info",
                    "bin/src/set_nco", "bin/src/set_xbar", "bin/src/set_fsc",
                    "bin/src/adrf6780.sh", "bin/src/lmx2594_120_a.sh", "bin/src/lmx2594_110_LO.sh",
                    "bin/src/ad5328.sh", "bin/src/rfswitch.sh",
                    "bin/utils/config.tcl",
                    "bin/setup_quel1.sh", "bin/setup_quel1_monitor.sh"],
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GPL",
        "Operating System :: OS Independent",
    ]
)
