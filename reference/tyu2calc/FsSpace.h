#ifndef FSSPACE_H
#define FSSPACE_H

#define FL8(s) QString::fromLocal8Bit(s)

#define T_SEC FL8("秒")
#define T_MIN FL8("分")
#define T_HOUR FL8("時")
#define T_DAY FL8("日")
#define T_YEAR FL8("年")
#define T_YDHMS FL8("YDHMS")

#define D_M FL8("m")
#define D_KM FL8("Km")
#define D2_TKM QString(tr("TenThousandKm"))
#define D_TKM FL8("万Km")
#define D_SS FL8("音秒")
#define D_SM FL8("海里")
#define D_ML FL8("Mile")
#define D_FT FL8("Feet")
#define D_YD FL8("Yard")
#define D_IC FL8("Inch")
#define D_LS FL8("光秒")
#define D_LM FL8("光分")
#define D_LH FL8("光時")
#define D_LD FL8("光日")
#define D_LY FL8("光年")
#define D_AU FL8("AU")
#define D_PC FL8("パーセク")

#define S_MPS FL8("m/s")
#define S_KMPS FL8("Km/s")
#define S_MPM FL8("m/M")
#define S_KMPM FL8("Km/M")
#define S_MPH FL8("m/h")
#define S_KMPH FL8("Km/h")
#define S_TKMPH  FL8("万Km/h")

#define S_KNT FL8("ノット")
#define S_FPS FL8("Feet/s")
#define S_FPM FL8("Feet/M")
#define S_FPH FL8("Feet/h")

#define S_MACH FL8("マッハ")
#define S_LPS FL8("light/s")
#define S_LPM FL8("light/M")
#define S_LPH FL8("light/h")
#define S_LPD FL8("light/d")
#define S_LPY FL8("light/y")

#define A_MPS2 FL8("m/s^2")
#define A_KMPS2 FL8("Km/s^2")
#define A_TKMPS2 FL8("万Km/s^2")
#define A_MPM2 FL8("m/M^2")
#define A_KMPM2 FL8("Km/M^2")
#define A_TKMPM2 FL8("万Km/M^2")
#define A_MPH2 FL8("M/h^2")
#define A_KMPH2 FL8("Km/h^2")
#define A_TKMPH2 FL8("万Km/h^2")
#define A_G FL8("G")


#define BYMIN 60.0
#define BYHOUR 3600.0
#define BYDAY 86400.0
#define BYYEAR 31536000.0

#define SND_SPEED 340.0
#define LIGHT_SPEED 299792458.0

#define KNOT_M (1852.0/3600.0)
#define SEA_MILE 1852.0
#define FEET_M 0.3048
#define INCH_M (25.4/1000.0)
#define YARD_M 0.9144
#define MILE_M 1609.344
#define AU_M 149597870700.0
#define PARSEC_M 30856800000000000.0
#define GACL 9.980665

#define MICRO_S  100000.0

#endif // FSSPACE_H
