- 编译Skia
  - python tools/git-sync-deps
  - bin/gn gen out/debug --args='clang_win=\"C:\Program Files\LLVM\" cc=\"clang\" cxx=\"clang++\" extra_cflags=[\"/MTd\"] is_trivial_abi=false is_official_build=true is_debug=false skia_use_system_expat=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_system_harfbuzz=false skia_use_system_icu=false skia_enable_skparagraph=true skia_enable_skshaper=true skia_enable_skunicode=true'
  - ninja -C out/debug
  - bin/gn gen out/release --args='clang_win=\"C:\Program Files\LLVM\" cc=\"clang\" cxx=\"clang++\" extra_cflags=[\"/MT\"] is_debug=false is_official_build=true skia_use_system_expat=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_system_harfbuzz=false skia_use_system_icu=false skia_enable_skparagraph=true skia_enable_skshaper=true skia_enable_skunicode=true'
  - ninja -C out/release 
  - 还要改几行配置文件的代码，不然它找不到我的VS2022
  - python3有问题时没办法编译通过的
    - 删除没用的python3 C:\Users\liuxiaolun\AppData\Local\Microsoft\WindowsApps>del python3.exe
- 编译quickjs
  - CMake Gui开始前要设置以下 -T ClangCL
  - 我自己写的示例https://github.com/xland/SimpleGUI/tree/main/SimpleGUI
- 目前验证了纯软渲染的效果，暂时先不搞GPU加速了
- 明天搞CMake工程，LIB库，Demo验证程序
- 预处理器加上SK_ENABLE_SKSL，软渲染支持RuntimeEffect


---

- 当支持GPU且窗口为正常窗口时，使用GPU加速，反之才使用纯软渲染
- 验证纯软渲染的runtime effect是否支持马赛克渲染
  - https://skia.org/docs/user/sksl/
- 编译skunicode.lib、sktext.lib、skparagraph.lib等库
  - 学习这些库的能力，验证相关功能



