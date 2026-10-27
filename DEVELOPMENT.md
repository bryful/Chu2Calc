# 開発

C++20 / Win32 / MSVC x64。単位変換は `src/Units.cpp`、等加速度運動は `src/Motion.cpp`、画面は `src/Main.cpp`。既存Qt版は `reference/` に保存しています。

Visual Studio 2026のC++開発環境とCMakeを使用します。

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -A x64 -T host=x64
cmake --build build --config Debug -- /p:PreferredToolArchitecture=x64
cmake --build build --config Release -- /p:PreferredToolArchitecture=x64
ctest --test-dir build -C Release --output-on-failure
```

実行ファイルは `build/Release/Chu2Calc.exe`。CRTを静的リンクし、Windows標準DLLのみを使用します。

計算は倍精度、距離はm、速度はm/s、質量はkgを基準とします。光年は365.25日。音速の初期値343 m/sは変更可能な基準値です。入力は小数点 `.` の数値と指数表記に対応します。

テストは既知の換算値、単位間の往復変換、入力検証、オーバーフロー、等加速度運動の既知値・逆算・ゼロ入力を確認します。

等加速度運動は `v = at`、`s = vt/2`、逆算は `t = 2s/v`、`a = v/t`。初速0、一定加速度、抵抗なし、非相対論のモデルです。

運動エネルギーは `src/Energy.cpp` に実装。質量kgと速さm/sから `E = mv²/2` を計算します。g入力はkgへ変換してから計算します。ゼロは許容し、負値・非有限値と表現範囲外の結果はエラーにします。二進仮数と指数に分解して、中間の二乗による不要なオーバーフローを避けています。

TNT換算は `src/Units.cpp` のエネルギー単位表で管理。TNT 1 t = 4.184 GJは [NIST SP 811 Appendix B.9](https://www.nist.gov/pml/special-publication-811/nist-guide-si-appendix-b-conversion-factors/nist-guide-si-appendix-b9) に基づきます。テストはg→kg、km/h→m/s、既知のエネルギー値、TNT単位、ゼロ・不正値・極端な値を含みます。

人工衛星の円軌道計算は `src/Orbit.cpp`。`r = R + h`、`v = sqrt(μ/r)`、`T = 2πr/v`、逆算は `r = μ/v²`。半径Rとμは独立した入力です。地球の初期半径は球近似の6371 km、μは [NGA WGS 84](https://earth-info.nga.mil/?action=wgs84&dir=wgs84) の3.986004418×10^14 m³/s²を使用。モデルの参考：[NASA Orbits in Space](https://pwg.gsfc.nasa.gov/stargaze/Sorbit.htm)。

軌道のテストは解析値が既知の仮想天体、地球高度400 km、順逆計算、表面での丸め誤差、不正値・天体内部・オーバーフローを検証します。

## 天体プリセット

`src/BodyPresets.cpp` に同梱し、ネット接続なしで使用します。選択時に半径をkm、μをm³/s²で現在の入力へ適用します。入力欄の数値は有効数字12桁に整形します。高度・速度は変更しません。半径とμは2つの人工衛星ページで共有し、手入力で変更するとカスタムに切り替えます。

採用値（2026-10-27確認）：

| 天体 | 半径km | μ m³/s² |
|---|---:|---:|
| 地球 | 6371 | 3.986004418e14 |
| 月 | 1737.4 | 4.902800118e12 |
| 水星 | 2439.4 | 2.2031868551e13 |
| 金星 | 6051.8 | 3.24858592e14 |
| 火星 | 3389.5 | 4.282837362e13 |
| 木星 | 69911 | 1.266865319e17 |
| 土星 | 58232 | 3.793120623e16 |
| 海王星 | 24622 | 6.83509997e15 |
| 天王星 | 25362 | 5.7939513e15 |
| 太陽 | 695700 | 1.32712440041279419e20 |
| ガニメデ | 2631.2 | 9.88783275e12 |
| ジュノー（小惑星3） | 124.5 | 1.7887124e9 |
| 冥王星 | 1188.3 | 8.693e11 |
| イオ | 1821.49 | 5.95991547e12 |
| エウロパ | 1560.8 | 3.2027121e12 |
| カリスト | 2410.3 | 7.1792834e12 |
| タイタン | 2574.76 | 8.9781371e12 |

地球は従来の初期値を維持（球半径6371km・WGS84のμ）。他の惑星・冥王星の半径は [JPL Planetary Physical Parameters](https://ssd.jpl.nasa.gov/planets/phys_par.html) の平均半径。月・水星・金星と太陽のμは [JPL Astrodynamic Parameters](https://ssd.jpl.nasa.gov/astro_par.html)。火星以遠と冥王星のμは衛星系全体ではなく [JPL Planetary Satellite Physical Parameters](https://ssd.jpl.nasa.gov/sats/phys_par/) の惑星単体の値。衛星の半径・μも同表から採用し、km³/s²からm³/s²へ10^9倍しています。太陽半径は [NASA Sun Fact Sheet](https://nssdc.gsfc.nasa.gov/planetary/factsheet/sunfact.html) の695700km。

ジュノーは [Viikinkoski et al. (2015), VLT/SPHERE- and ALMA-based shape reconstruction of asteroid (3) Juno](https://doi.org/10.1051/0004-6361/201526626) の直径249kmと文献平均質量2.68×10^19kgを採用。半径は直径の半分、μは質量にG=6.67430×10^-11を掛けた推定値です。球近似であり、観測の不確かさや非球形重力を反映するものではありません。巨大惑星の平均半径も球近似で、大気・環・扁平率などはモデルに含みません。

## Tips

`src/Tips.h` にページごとの説明を定義。旧版 `reference/tyu2calc/Satellite.cpp` の高度一覧を参考に整理し、旧版のISS高度15kmは約400kmへ修正しました。地球の例と他天体の計算条件を区別し、月の距離は地表高度ではなく地心距離として表記しています。右パネル下部の読み取り専用テキスト欄は折り返し・縦スクロール・選択コピーに対応します。

高度の参考資料：[NASA 大気の層](https://science.nasa.gov/earth/earth-atmosphere/earths-atmosphere-a-multi-layered-cake/)、[NOAA 大気の層](https://www.noaa.gov/jetstream/atmosphere/layers-of-atmosphere)、[ESA 軌道の種類](https://www.esa.int/Enabling_Support/Space_Transportation/Types_of_orbits)、[CSA ISS FAQ](https://www.asc-csa.gc.ca/eng/iss/information-kit/faq.asp)。Tipsの数値は目安で、計算への入力や自動判定には使用しません。

## 衝撃・回転・投射

`src/Mechanics.cpp` に計算を分離しています。衝撃は `F=mv²/(2d)`、`a=F/m`、`t=2d/v`。Fは停止距離にわたる平均で、減速Gと時間は一定減速モデル。速度0では力・減速・時間0とします。

回転は `ω=2π/T`、`v=rω`、`a=rω²`、`rpm=60/T`。投射は `vx=v cosθ`、`vy=v sinθ`、`t=2vy/g`、`h=vy²/(2g)`、`R=vx t`。0度と90度は境界値を明示的に処理します。

3入力の衝撃・投射画面は停止距離または重力の単位を選択可能。角度は度・rad、力はN・kN・kgf・tf。テストは既知値・倍率関係・境界値・不正入力・オーバーフローとrad→度換算を検証します。

追加の逆算：人工重力は `T=2π√(r/a)`、`v=√(ra)`。0Gでは有限周期が決まらないため正の目標値のみ受理します。跳躍は離陸後の目標高さh、踏み切り距離d、重力gから `v=√(2gh)`、正味加速度 `a=gh/d`、支持力÷質量 `a+g`、踏み切り時間 `2d/v`。地表からの高度ではなく離陸位置からの高さです。既存の周期入力・初速入力画面は維持します。

## 機能一覧・自由落下

`NAV_ORDER` で表示順を定義し、リスト項目のデータに画面IDを格納します。表示順を変更しても画面ごとの入力状態を取り違えません。

自由落下は `src/Motion.cpp`。初速0・一定重力・抵抗なしで、時間からは `v=gt`、`s=gt²/2`、距離からは `v=√(2gs)`、`t=v/g`。時間・距離0は受理し、重力は正の値のみ。既知値・順逆計算・月の重力・ゼロ・負値・オーバーフローを検証します。

二乗抵抗版は `src/DragFall.cpp` に追加。`q=ρCdA/(2m)`、`vt=√(g/q)`、`τ=vt/g`、`v=vt tanh(t/τ)`、`s=log(cosh(t/τ))/q`。距離指定では `z=qs`、`v=vt√(1-exp(-2z))`、`t=τ[z+log(1+√(1-exp(-2z)))]`。log1p・expm1と安定なlog(cosh)で小値の桁落ち・大値のcoshオーバーフローを避けています。抵抗0は真空の既存計算へ委譲し、終端速度はoptionalの空値にします。

脱出速度は `src/Orbit.cpp` の `escapeFromAltitude`。`r=R+h`、`v=√(2μ/r)`、円軌道速度 `vc=√(μ/r)`、局所重力 `g=μ/r²`。天体プリセットとパラメータを人工衛星の2画面と共有。テストは地球・月の既知値域、√2の比、半径による速度・重力の変化、不正値・オーバーフローを検証します。

電力・エネルギーは `src/Energy.cpp`。`P=E/t`、`t=E/P`、`E=Pt`。除数は正、その他の値は0以上。非有限・負値・オーバーフローと正の結果のアンダーフローを検出します。効率100%を前提とします。単位表にPowerとWh・MWhを追加。テストは既知値・相互変換・単位換算・ゼロ・不正値・範囲外を検証します。

## リリース

バージョンはCMakeLists.txtとsrc/Chu2Calc.rcを揃えます。アイコンはassets/Chu2Calc.icoをリソースに埋め込み、エクスプローラーとウィンドウの大小アイコンに使用。原稿と生成プロンプトはassets/に保存しています。

Releaseのビルド・テスト後に `powershell -File scripts/Package-Release.ps1` を実行すると、EXEの製品バージョンからrelease/Chu2Calc-<version>-windows-x64.zipを作成します。EXEと説明書、EXEのSHA-256を収録し、ZIPのSHA-256も別ファイルに出力します。既存の同名出力がある場合は上書きせず停止します。

## 面積とエネルギー単位（1.1.0）

面積計算は `src/Area.cpp`。円はπr²、長方形は縦×横。0は許容し、負値・非有限・範囲外を拒否。比較基準は東京ドーム建築面積46755m²、江戸間1.76×0.88=1.5488m²、京間1.91×0.955=1.82405m²です。出典：[東京ドーム公式](https://www.tokyo-dome.co.jp/dome/about/)、[国土交通省・畳の規格と名称](https://www.qsr.mlit.go.jp/nobeoka/kasen/rekisi/pdf/tatamitei02.pdf)。入力はm²・km²・ha・a・cm²、長さの2乗換算は形状計算前にSIへ変換します。

ジャイアント馬場キックの換算係数はEnergy.hのBABA_KICK_JOULES（3375J）。単位表経由で全エネルギー表示・入力に適用します。
