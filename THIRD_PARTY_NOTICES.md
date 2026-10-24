# Third-party notices

TNSapp itself (Copyright (C) 2026 Reita Tsutsumimoto) is licensed under the **GNU Affero General Public License v3.0**
(see [LICENSE](LICENSE)). It is built with JUCE 9.0.2 under the AGPLv3 option
of JUCE's dual licence.

This repository and the built application include or use the third-party
components below. Full licence texts are in [`licenses/`](licenses/).
Last reviewed: 2026-10-24.

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

According to sfizz's README (https://github.com/sfztools/sfizz), sfizz uses:
Abseil (Apache-2.0, [licenses/Apache-2.0.txt](licenses/Apache-2.0.txt)),
atomic_queue (MIT), KISS FFT (BSD-3-Clause), pugixml (MIT), cpuid (BSD-3-Clause),
cephes (BSD-3-Clause), hiir (WTFPL v2), Surge tuning (MIT), filesystem
(BSD-3-Clause), faust-libraries-derived DSP code, and dr_libs (MIT-0/public
domain) for audio decoding. Symbol names in the shipped `sfz~` binary are
consistent with Abseil, KISS FFT, pugixml, atomic_queue, cpuid, stb_vorbis,
dr_flac and Faust-generated code; `libsndfile` is not linked. The ELSE shared
library appears to contain TinySoundFont (MIT) and libsamplerate (BSD-2-Clause)
code. Their own copyright and licence texts are available in the upstream
repositories (links above).

## 3. Not included

* No Logic Pro / Apple sample-derived soundfonts.
* No GeneralUser GS soundfont.
* The full ELSE and cyclone distributions (only the objects listed above).
* The full libpd / Pure Data source tree (only headers needed to build).
