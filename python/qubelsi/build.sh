#!/bin/bash

set -eu

# 'src' as a directory for binaries due to historical reason.
# will be fixed in near future by replacing the binaries with python functions.
bindir=qubelsi/bin
bindir120=qubelsi/bin/src
bindir106=qubelsi/bin/v1.0.6/src
utildir=qubelsi/bin/utils

# clean all
rm -rf build dist qubelsi.egg-info ${bindir}

mkdir -p "${bindir}"
cp ../../setup.sh "${bindir}/setup_quel1.sh"
cat ../../setup.sh | sed 's/hello/hello_monitor/g' > "${bindir}/setup_quel1_monitor.sh"
chmod +x "${bindir}/setup_quel1_monitor.sh"

(cd ../../v1.0.6/src; make)
mkdir -p "${bindir106}"
cp ../../v1.0.6/src/hello "${bindir106}"
cp ../../v1.0.6/src/hello_monitor "${bindir106}"

(cd ../../src; make)
mkdir -p "${bindir120}"
cp ../../src/udpsendrecv "${bindir120}"
cp ../../src/get_jesd_status "${bindir120}"
cp ../../src/print_ad9082_info "${bindir120}"
cp ../../src/set_nco "${bindir120}"
cp ../../src/set_xbar "${bindir120}"
cp ../../src/set_fsc "${bindir120}"
cp ../../src/adrf6780.sh "${bindir120}"
cp ../../src/lmx2594_120_a.sh "${bindir120}"
cp ../../src/lmx2594_110_LO.sh "${bindir120}"
cp ../../src/ad5328.sh "${bindir120}"
cp ../../src/rfswitch.sh "${bindir120}"


mkdir -p "${utildir}"
cp ../../utils/config.tcl "${utildir}"

python setup.py bdist_wheel
