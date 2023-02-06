# API to manipulate LSIs in QuBE

## Setup (Conventional)

For example,

```
% cd ${TMPDIR}
% git clone https://github.com/qiqb-osaka/adi_api_mod # use HTTPS
% git clone git@github.com:qiqb-osaka/adi_api_mod.git # or use SSH
% cd ${PIPENVDIR}
% pipenv shell
% pipenv install ${TMPDIR}/python/qubelsi
```

## Setup (New)
qubelsiのwhlパッケージを作成して、仮想環境にインストールする。
パッケージの作成が一手間かかるが、バイナリやtclファイルもパッケージに含まれるので、実行環境からリポジトリを参照する必要がないのが利点。

ビルド用の仮想環境にて（実行環境がビルド環境を兼ねてもよい）
```
% cd ${TMPDIR}
% git clone git@github.com:qiqb-osaka/adi_api_mod.git
% cd adi_api_mod/python/qubelsi
% ./build.sh
```

実行環境にインストール。
```
% cd ${PIPENVDIR}
% pipenv shell
% pipenv install ${TMPDIR}/python/qubelsi/dist/qubelsi-1.0-py3-none-any.whl
```

上記の方法でもできるのだが、どこか自分でビルドしたパッケージをまとめて置く場所をつくり、
Pipenvファイルから参照するのが一番良い。

## Example

### Access LSIs in QuBE

```
% cd ${PIPENVDIR}
% pipenv shell
% python
>>> import qubelsi.qube
>>> q = qubelsi.qube.Qube("10.5.0.14", "/home/miyo/adi_api_mod/")
>>> q
<qubelsi.qube.Qube object at 0x7f22e6d2d880>
>>> q.ad9082
[<qubelsi.ad9082.AD9082 object at 0x7f22e6d2dbb0>, <qubelsi.ad9082.AD9082 object at 0x7f22e6d2dc70>]
>>> print(q.ad9082[0].read_info())
CHIP_TYPE     = 0f
PROD_ID_LSB   = 82
PROD_ID_MSB   = 90
CHIP_GRADE    = 23
SPI_REVISION  = 01
VENDER_ID_LSB = 56
VENDER_ID_MSB = 04
CHIP_TYPE     = 0f
PROD_ID_LSB   = 82
PROD_ID_MSB   = 90
CHIP_GRADE    = 23
SPI_REVISION  = 01
VENDER_ID_LSB = 56
VENDER_ID_MSB = 04

>>> q.lmx2594
[<qubelsi.lmx2594.LMX2594 object at 0x7f22e6d2dca0>, ..., <qubelsi.lmx2594.LMX2594 object at 0x7f22e6c69040>]
>>> q.lmx2594_ad9082
[<qubelsi.lmx2594.LMX2594 object at 0x7f22e6d2da90>, <qubelsi.lmx2594.LMX2594 object at 0x7f22e6c691c0>]
>>> q.adrf6780
[<qubelsi.adrf6780.ADRF6780 object at 0x7f22e6c69100>, ..., <qubelsi.adrf6780.ADRF6780 object at 0x7f22e6c697c0>]
>>> q.ad5328
<qubelsi.ad5328.AD5328 object at 0x7f22e6c69280>
>>> q.gpio
<qubelsi.gpio.GPIO object at 0x7f22e6c69910>
>>> q.gpio.read_value()
16383
>>> 

```

### Initialization

```
>>> q = qubelsi.qube.Qube("10.5.0.14", "/home/miyo/adi_api_mod/")
>>> q.do_init() # without FPGA configuration
>>> q.do_init(bitfile="hoge.bit") # FPGA configuration with hoge.bit
                                  # required Vivado settings
>>> q.ad9082[0].get_jesd_status()
[['0x55E', '0xE0'], ['0x5BB', '0x01'], ['0x62E', '0x01'], ['pll_status', '0x01'], ['0x728', '0x0B']]
>>> q.ad9082[1].get_jesd_status()
[['0x55E', '0xE0'], ['0x5BB', '0x01'], ['0x62E', '0x01'], ['pll_status', '0x01'], ['0x728', '0x0B']]
>>> 
```
