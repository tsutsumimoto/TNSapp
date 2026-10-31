# Third-party notices

TNSapp itself (Copyright (C) 2026 Reita Tsutsumimoto) is licensed under the **GNU Affero General Public License v3.0**
(see [LICENSE](LICENSE)). It is built with JUCE 9.0.2 under the AGPLv3 option
of JUCE's dual licence.

This repository and the built application include or use the third-party
components below. Full licence texts are in [`licenses/`](licenses/).
Last reviewed: 2026-10-31.

## 1. Audio samples (`TNSapp/patches/sfz/`)

| Component | Used for | Licence | Source |
|---|---|---|---|
| VSCO 2 Community Edition 1.1.0 (Versilian Studios) | Strings: violin, viola, cello (Spiccato and Sustain/Vibrato sections). The `.sfz` files in `patches/sfz/Strings` are derived from the VSCO 2 CE SFZ files. Staccato, accent and sustain variants are produced by editing the SFZ settings (envelopes, filter, velocity layers, volume); the recordings are unmodified. | CC0 1.0 | https://github.com/sgossner/VSCO-2-CE |
| VCSL Keys "Grand Piano, K" (Versilian Studios) | Piano | CC0 1.0 (stated on the publisher's page as a derivative of VCSL) | https://versilian-studios.com/vcsl-keys/ |

CC0 requires no attribution; credit is given here voluntarily. Text:
[licenses/CC0-1.0.txt](licenses/CC0-1.0.txt). No sample material from Logic Pro
or other Apple sample content is included.

## 2. Libraries linked into or shipped with the application

| Component | Version | Licence | Notes |
|---|---|---|---|
| JUCE | 9.0.2 | AGPLv3 (dual-licensed with a commercial licence; used here under AGPLv3) | Not included in this repository. The build expects JUCE next to this repository (see README). The built app also contains code JUCE bundles (e.g. FLAC, Ogg/Vorbis, libpng, libjpeg, zlib, HarfBuzz, SheenBidi, LunaSVG), each under its own licence; see the JUCE distribution (`LICENSE.md` and `modules/*/`). |
| libpd | 0.15.0 | Standard Improved BSD | Headers and the prebuilt `universal/libs/libpd.a` are included. [licenses/Pd-libpd-Standard-Improved-BSD.txt](licenses/Pd-libpd-Standard-Improved-BSD.txt) |
| Pure Data (inside libpd) | 0.55-2 | Standard Improved BSD | Same file as above. |
| cyclone | 0.9-2 (2025-03-14) | BSD 3-Clause | 15 binaries in `externals/cyclone/`. [licenses/cyclone-BSD-3-Clause.txt](licenses/cyclone-BSD-3-Clause.txt) |
| ELSE | 1.0-0 rc13, rev. 2 (2025-05-04) | WTFPL v2 (mostly; some objects differ, see below) | 15 binaries in `externals/else/`, abstractions in `externals/` and `externals/else-abstractions/`, shared library `universal/libs/libelse_shared.dylib`. [licenses/ELSE-WTFPL-2.txt](licenses/ELSE-WTFPL-2.txt) |
| sfizz | 1.2.2 | BSD 2-Clause | Statically included in ELSE `[sfz~]`. [licenses/sfizz-BSD-2-Clause.txt](licenses/sfizz-BSD-2-Clause.txt) |

### ELSE objects actually included

Binaries: `args`, `datetime`, `else` (library loader), `hot`, `lace`, `lb`,
`loadbanger`, `loop`, `message`, `router`, `routetype`, `sfz~`, `slice`,
`sort`, `wrap2`. Abstractions: `add`, `any2symbol`, `count`, `group`,
`iterate`, `list.seq`, `op`, `reverse`, `rotate`, `stream`.

ELSE states that individual objects may use other licences: `[beat~]`,
`[sfont~]`, `[giga.rev~]`, `[conv~]`, `[lua]` are GPL; `[sfz~]` and the
`[bl.*]` oscillators are BSD 2-Clause; `[plaits~]`, `[osc.format]`,
`[osc.parse]`, `[osc.route]` are MIT. **None of the GPL or MIT objects are
included in this repository or in the built app**; only `[sfz~]` (BSD 2-Clause)
above departs from the WTFPL default. (ELSE's accompanying "Live Electronics
Tutorial" is CC BY-NC-ND and is not included.)

### Components inside `[sfz~]` and `libelse_shared.dylib`

`[sfz~]` is built from the sfizz 1.2.2 source tree that ships with ELSE
(`Source/Audio/sfz~/sfizz/` in https://github.com/porres/pd-else, tag
v.1.0-rc13). The licence texts found in that tree are reproduced in
[licenses/sfizz-bundled-libraries.txt](licenses/sfizz-bundled-libraries.txt):
abseil-cpp (Apache-2.0, [licenses/Apache-2.0.txt](licenses/Apache-2.0.txt)),
atomic_queue (MIT), filesystem (MIT), cxxopts (MIT), simde (MIT), cephes
(BSD-3-Clause), st_audiofile (BSD-2-Clause), fmidi and jsl (Boost 1.0),
dr_libs (Public Domain or MIT-0), libaiff (MIT), stb_vorbis (MIT or Public
Domain), and ThreadPool.h (SPDX: zlib). Not every one is necessarily linked
into the `[sfz~]` binary (some are used only by command-line tools).
The same file also reproduces the licences of KISS FFT (BSD-3-Clause), pugixml
(MIT), cpuid (BSD-3-Clause), hiir (WTFPL v2), spline (BSD-3-Clause) and tunings
(MIT, based on the Surge tuning library). The Faust-generated filter code in sfizz
declares BSD-2-Clause in its file headers (author: Jean Pierre Cimalando; generated
with Faust 2.37.3). The Faust sources of some of these filters also build on GRAME's
`rbj_filters` ("LGPL with exception") and on Eric Tarr's Sallen-Key filters
("MIT-style STK-4.3 license"); their licence texts are not reproduced here. TNSapp's
SFZ files use only the `hpf_1p` filter, which uses neither. `libsndfile` is not linked.

The ELSE shared library `libelse_shared.dylib` contains libsamplerate
(BSD-2-Clause, Erik de Castro Lopo); see
[licenses/libsamplerate-BSD-2-Clause.txt](licenses/libsamplerate-BSD-2-Clause.txt).
No TinySoundFont strings were found in that binary. ELSE's GPL-licensed objects
(`[sfont~]`, `[beat~]`, `[giga.rev~]`, `[conv~]`, `[lua]`) are not included in
this repository.

## 3. Not included

* No Logic Pro / Apple sample-derived soundfonts.
* No GeneralUser GS soundfont.
* The full ELSE and cyclone distributions (only the objects listed above).
* The full libpd / Pure Data source tree (only headers needed to build).
