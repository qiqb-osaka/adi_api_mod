for i in 0 1 2 3 4 5 6 7
do
./udpsendrecv lmx2594 w $i 00 2612 # R6 [14]VCO_PHASE_SYNC=0, [9]OUT_MUTE=1
./udpsendrecv lmx2594 w $i 00 2610
./udpsendrecv lmx2594 r $i 00 0
./udpsendrecv lmx2594 w $i 4E 0001
./udpsendrecv lmx2594 w $i 4D 0000
./udpsendrecv lmx2594 w $i 4C 000C
./udpsendrecv lmx2594 w $i 4B 0840
./udpsendrecv lmx2594 w $i 4A 0000 # R74 [15:2] SYSREF_PULSE_COUNT=0
./udpsendrecv lmx2594 w $i 49 003F
./udpsendrecv lmx2594 w $i 48 0000 # R72 [10:0] SYSREF_DIV
./udpsendrecv lmx2594 w $i 47 0081 # R71 [7:5]SYSREF_DIV_PRE=4='Divided by 4', [3]SYSREF_EN=0, [2]SYSREF_REPEAT=0
./udpsendrecv lmx2594 w $i 46 C350
./udpsendrecv lmx2594 w $i 45 0000
./udpsendrecv lmx2594 w $i 44 03E8
./udpsendrecv lmx2594 w $i 43 0000
./udpsendrecv lmx2594 w $i 42 01F4
./udpsendrecv lmx2594 w $i 41 0000
./udpsendrecv lmx2594 w $i 40 1388
./udpsendrecv lmx2594 w $i 3F 0000
./udpsendrecv lmx2594 w $i 3E 0322
./udpsendrecv lmx2594 w $i 3D 00A8
./udpsendrecv lmx2594 w $i 3C 03E8
./udpsendrecv lmx2594 w $i 3B 0001
./udpsendrecv lmx2594 w $i 3A 8001 # R58 [15]IGNORE=0, [14]HYST=0, [13:12]INPIN_LVL=0=Vin/4, [11:9]FMT=2='sysrefreq=LVDS'
./udpsendrecv lmx2594 w $i 39 0020
./udpsendrecv lmx2594 w $i 38 0000
./udpsendrecv lmx2594 w $i 37 0000
./udpsendrecv lmx2594 w $i 36 0000
./udpsendrecv lmx2594 w $i 35 0000
./udpsendrecv lmx2594 w $i 34 0820
./udpsendrecv lmx2594 w $i 33 0080
./udpsendrecv lmx2594 w $i 32 0000
./udpsendrecv lmx2594 w $i 31 4180
./udpsendrecv lmx2594 w $i 30 0300
./udpsendrecv lmx2594 w $i 2F 0300
./udpsendrecv lmx2594 w $i 2E 07FD # R46 [1:0] OUTB_MUX=1(=VCO)
./udpsendrecv lmx2594 w $i 2D C8DF # R45 [5:0] OUTB_PWR=1F
./udpsendrecv lmx2594 w $i 2C 3220 # R44  3220
./udpsendrecv lmx2594 w $i 2B 0000
./udpsendrecv lmx2594 w $i 2A 0000
./udpsendrecv lmx2594 w $i 29 0000
./udpsendrecv lmx2594 w $i 28 0000
./udpsendrecv lmx2594 w $i 27 0001
./udpsendrecv lmx2594 w $i 26 0000
./udpsendrecv lmx2594 w $i 25 0204
./udpsendrecv lmx2594 w $i 24 0064 # 100
./udpsendrecv lmx2594 w $i 23 0004
./udpsendrecv lmx2594 w $i 22 0000
./udpsendrecv lmx2594 w $i 21 1E21
./udpsendrecv lmx2594 w $i 20 0393
./udpsendrecv lmx2594 w $i 1F 43EC
./udpsendrecv lmx2594 w $i 1E 318C
./udpsendrecv lmx2594 w $i 1D 318C
./udpsendrecv lmx2594 w $i 1C 0488
./udpsendrecv lmx2594 w $i 1B 0002
./udpsendrecv lmx2594 w $i 1A 0DB0
./udpsendrecv lmx2594 w $i 19 0C2B
./udpsendrecv lmx2594 w $i 18 071A
./udpsendrecv lmx2594 w $i 17 007C
./udpsendrecv lmx2594 w $i 16 0001
./udpsendrecv lmx2594 w $i 15 0401
./udpsendrecv lmx2594 w $i 14 C848
./udpsendrecv lmx2594 w $i 13 27B7
./udpsendrecv lmx2594 w $i 12 0064
./udpsendrecv lmx2594 w $i 11 00FA
./udpsendrecv lmx2594 w $i 10 0080
./udpsendrecv lmx2594 w $i 0F 064F
./udpsendrecv lmx2594 w $i 0E 1E10
./udpsendrecv lmx2594 w $i 0D 4000
./udpsendrecv lmx2594 w $i 0C 5001
./udpsendrecv lmx2594 w $i 0B 0018
./udpsendrecv lmx2594 w $i 0A 10D8
./udpsendrecv lmx2594 w $i 09 0604
./udpsendrecv lmx2594 w $i 08 2000
./udpsendrecv lmx2594 w $i 07 00B2
./udpsendrecv lmx2594 w $i 06 C802
./udpsendrecv lmx2594 w $i 05 00C8
./udpsendrecv lmx2594 w $i 04 1B43
./udpsendrecv lmx2594 w $i 03 0642
./udpsendrecv lmx2594 w $i 02 0500
./udpsendrecv lmx2594 w $i 01 080B
./udpsendrecv lmx2594 w $i 00 2618
./udpsendrecv lmx2594 r $i 00 0

done
