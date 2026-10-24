# TNSapp

A macOS application (JUCE + libpd) that turns the stroke data of the Ishikawa
Kyūyō exhibition into a score. The musical logic lives in a Pure Data patch
(`TNSapp/patches/main.pd`); the C++ part (`TNSapp/Source`) is the GUI and the
bridge to libpd. Samples are played by the ELSE `[sfz~]` object.

石川九楊展の筆跡データから楽譜を生成する macOS アプリです。変換ロジックは Pure Data のパッチ
(`TNSapp/patches/main.pd`)、C++ 側は GUI と libpd への橋渡しです。

> Status: work in progress. Detailed documentation (the parameter-mapping
> logic, usage) will be added.

## Background and credits

TNSapp was made for the score-music performance 書譜楽「歎異抄 No.18〜いはんや悪人をや〜
Ver.2.0」 (the second of two events; organiser: 石川九楊傘寿記念祝賀会実行委員会), held
for the Ishikawa Kyūyō exhibition. I have been involved in the development since the
first event; this application was written for the second one.

* **Measurement and analysis of the data** (`TNSapp/patches/abs/parameters_chapter*.txt`):
  Scott Allen and Latent Media Lab., Kyoto Seika University (Faculty of Media
  Expression). The data is included here with their permission.
* **Application, sound programming and the conversion logic** (everything else in
  this repository): Reita Tsutsumimoto.

`TNSapp/patches/abs/artlist_chapter*.txt` are articulation values derived from that
data by my own conversion (see the patch); they are not part of the original data.

The licence below applies to the source code. The analysis data was produced by
Latent Media Lab.; it is included as a demonstration data set for the application.

---

石川九楊展のための書譜楽「歎異抄 No.18 Ver.2.0」（全2回の第2回）用に制作しました。第1回から
開発に関わっています。データの計測・解析は Scott Allen 氏と京都精華大学 Latent Media Lab.
によるもので、許可を得て同梱しています。アプリ・サウンドプログラム・変換ロジックは
Reita Tsutsumimoto の制作です。

## Build (macOS)

Requirements: Xcode, Projucer and [JUCE 9.0.2](https://juce.com/) (AGPLv3
option), Apple Silicon or Intel.

1. Put the JUCE folder **next to this repository folder** (so that
   `TNSapp/../../JUCE/modules` exists), e.g.

   ```
   Documents/
   ├── JUCE/            <- JUCE 9.0.2
   └── TNSapp/          <- this repository
       └── TNSapp/TNSapp.jucer
   ```

   (If JUCE is somewhere else, change the module paths in Projucer.)
2. Open `TNSapp/TNSapp.jucer` in Projucer and choose *Save Project and Open
   in IDE*. The Xcode project is generated; it is not stored in the repository.
3. In Xcode, add `TNSapp/universal/libs/libpd.a` to *Build Phases → Link Binary
   With Libraries* (*Add Other…*). Then build (Debug or Release; the project
   builds a universal binary).
4. A post-build script copies `patches/sfz/` and the ELSE abstractions
   (`externals/else-abstractions/`, with the `else/` prefix removed because
   libpd resolves objects from a flat folder) into the app's `Resources`.

`[Pd print]` output (useful for debugging) is only shown in Debug builds.

## Known issues

* In Debug builds, about 48 `couldn't create` lines appear in the console at
  start-up, before the patch is opened. They come from the Tcl GUI snippets of
  cyclone/ELSE and have no effect on the sound or the behaviour.
* Only macOS has been tested.

## Licence

Copyright (C) 2026 Reita Tsutsumimoto.
TNSapp is licensed under the GNU AGPL v3.0 ([LICENSE](LICENSE)).
Third-party components (JUCE, libpd/Pd, cyclone, ELSE, sfizz, VSCO 2 CE,
VCSL Keys) and their licences are listed in
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
