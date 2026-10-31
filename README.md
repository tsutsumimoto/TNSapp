# TNSapp

A macOS application (JUCE + libpd) that turns the stroke data of the Ishikawa
Kyūyō exhibition into MIDI data for a string quartet and a piano. The musical logic lives in a Pure Data patch
(`TNSapp/patches/main.pd`); the C++ part (`TNSapp/Source`) is the GUI and the
bridge to libpd. Samples are played by the ELSE `[sfz~]` object.

石川九楊展の筆跡データから、弦楽四重奏とピアノの MIDI データを生成する macOS アプリです。変換ロジックは Pure Data のパッチ
(`TNSapp/patches/main.pd`)、C++ 側は GUI と libpd への橋渡しです。

> A detailed description of how the input data is converted into the output will be
> published separately.

## Background and credits

TNSapp was made for the score-music performance 書譜楽「歎異抄 No.18〜いはんや悪人をや〜
Ver.2.0」 (the second of two events; organiser: 石川九楊傘寿記念祝賀会実行委員会), held
for the Ishikawa Kyūyō exhibition. I have been involved in the development since the
first event; this application was written for the second one.

* **Measurement and analysis of the data** (`TNSapp/patches/abs/parameters_chapter*.txt`):
  Scott Allen and Latent Media Lab., Kyoto Seika University (Faculty of Media
  Expression). The data is included here with their permission. Three rows were
  derived by me for this application and are not part of the original data:
  `coefficient`, `Major0Minor1` and `art-length` (the last one is calculated from
  the text2emotion values in the original data).
* **Application, sound programming and the conversion logic** (everything else in
  this repository): Reita Tsutsumimoto.

**AI assistance.** The Pure Data patch and the conversion logic are my own design. The
C++ part (GUI, libpd bridge, MIDI post-processing) and this README were written with the
help of an AI assistant (Claude); I built and tested the result.

`TNSapp/patches/abs/artlist_chapter*.txt` are articulation values derived from that
data by my own conversion (see the patch); they are not part of the original data.

The licence below applies to the source code. The analysis data was produced by
Latent Media Lab.; it is included as a demonstration data set for the application.

---

石川九楊展のための書譜楽「歎異抄 No.18 Ver.2.0」（全2回の第2回）用に制作しました。第1回から
開発に関わっています。データの計測・解析は Scott Allen 氏と京都精華大学 Latent Media Lab.
によるもので、許可を得て同梱しています（`coefficient`・`Major0Minor1`・`art-length` の3行は、
アプリ用に元データから Reita Tsutsumimoto が変換した値です）。アプリ・サウンドプログラム・変換ロジックは
Reita Tsutsumimoto の制作です。

**AIの利用について。** Pure Data パッチと変換ロジックは、私自身の設計です。C++ 部分（GUI、libpd との橋渡し、
MIDI の後処理）と README は、AIアシスタント（Claude）の支援を受けて作成し、ビルドと動作確認は私が行いました。

## Usage

TNSapp plays the stroke data of one chapter as a string quartet and a piano,
and writes the result as MIDI files. A short description of the window follows;
the detailed conversion rules will be published separately.

**Basic flow**

1. Choose a **chapter** (第1章 / 第3章 / 第10章 / 結 = chapter 18).
2. Choose which input drives which output in the **Parameter Sets** table
   (see below), the **Beat Variation** and the options.
3. Press **Compile**. Nothing changes until you press it; every change of the
   settings needs a new Compile. If you press it during playback, the piece
   restarts from the beginning with the new settings.
4. Press **Play/Stop**. Press **Save** to write the settings and the MIDI files.

**Transport**

* **Play/Stop**: starts and stops playback. Stopping returns to the beginning.
* **Pause**: pauses; pressing it again resumes from the same place. Pressing
  Stop while paused returns to the beginning.
* **Loop**: repeats the playback.
* **BPM** (60–200, default 120): tempo.
* **Position / Last Pos / Total Time**: the current position in the data, the
  last position, and the length of the piece at the current BPM.
* **String / Piano**: switch the sound of each group on and off, like mute
  buttons in a DAW (light = sounding, dark = muted).

**Settings**

* **Parameter Sets**: each column is an output (Octave, Permutation, Chord,
  Key-1/2/3, Pan-1/2, Velocity, Beat, Articulation) and each row is an input
  (`penspeed`, `angle`, `penblur`, `erosion`, `penslight`, `coefficient`,
  `width`, `height`, `speed`, `depth`, `baseline`). Select exactly one input per
  output.
* **Beat Variation**: *Simple* and *Complex* use different sets of note values
  (Complex has more, including dotted notes).
* **Beat-invert**, **Arpeggio-invert**, **Pan-invert**: reverse the direction of
  the corresponding conversion (for example, short notes become long ones). In a
  small window the button is shown as "Arpeggio…".
* **Default**: resets the table, the chapter, Beat Variation, the three invert
  options and the BPM to their initial values. Press Compile afterwards.

**Files** (in `~/Documents/TNSapp/`)

* **Save** asks for two names: a preset (`presets/Preset_<date>_<time>.txt`, the
  settings and BPM) and a folder `save/Save_<date>_<time>/` that receives one
  MIDI file per part (for example `cello_20261025_21.mid`).
* **Load File** reads a preset from `presets/` and compiles it automatically.
* `backup/` receives an automatic backup each time Compile is pressed. It is
  never cleaned up, so delete old files from time to time.

**MIDI files**

Each Save writes, for every part, a `.mid` file and a `.txt` file
(`cello`, `viola`, `violin1`, `violin2`, `piano`).

* The `.mid` files contain note-on and note-off events. Each note lasts until the
  next event, and rests are gaps. In the piano file, the notes of a chord are
  shifted one after another by a quarter of the note length in the passages where
  the arpeggio is on, as in the playback inside the app. (When the patch writes the
  files, rests are marked by low note numbers, 0 or 12. TNSapp removes these markers
  and adds the note-offs right after saving.)
* The `.txt` files are the raw sequences written by the patch (time, status, note,
  velocity). They contain note-on events only; the rest markers (note 0, and note 12
  in the piano file) are still in them, and the arpeggio is not applied.

Three more files carry information that is not written into the notes themselves,
because the way to select articulations differs from one software to another. They
are stored as note numbers and have to be assigned by hand in your DAW or sampler.

| File | Note number | Meaning |
|---|---|---|
| `string-articulation` | 10 / 11 / 12 / 13 / 14 / 15 | none / slur / tenuto / accent / staccato / spiccato |
| `piano-articulation` | 10 / 11 / 12 | no pedal / sustain pedal / damper (a muffled sound) |
| `piano-arpeggio` | 10 / 11 | chord played at once / arpeggio (already applied to the piano `.mid`; kept for reference) |

(The "damper" here is not the damper pedal of a real piano; it is a setting that
narrows the low-pass filter to muffle the sound.)

---

**使い方（要約）**: 章を選び、Parameter Sets の表で「どの入力をどの出力につなぐか」と Beat
Variation などを選び、**Compile** を押してから Play します（設定を変えるたびに Compile が必要です）。
Save で設定（`presets`）と、パートごとの MIDI（`save/Save_日時/`）を `~/Documents/TNSapp/`
に書き出します。Load File は `presets` の設定を読み込み、自動で Compile します。
String / Piano ボタンは、DAW のミュートのように、音のオン・オフを切り替えます。
MIDI には note-on と note-off が入り、休符はすき間になります（ピアノは、分散和音の区間で、和音を少しずつずらして書き出します）。奏法は、別ファイルにノートナンバーで書き出すので、DAW 側で手作業で割り当てます。

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
* The window is laid out for a large screen. In a small window some labels are
  shortened (the Arpeggio-invert button is shown as "Arpeggio…").
* The `backup/` folder grows with every Compile and is never cleaned up.
* Only macOS has been tested.

## Licence

Copyright (C) 2026 Reita Tsutsumimoto.
TNSapp is licensed under the GNU AGPL v3.0 ([LICENSE](LICENSE)).
Third-party components (JUCE, libpd/Pd, cyclone, ELSE, sfizz, VSCO 2 CE,
VCSL Keys) and their licences are listed in
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
