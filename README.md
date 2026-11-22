# APRSISCE/32 manual

**APRSIS32 2026.11.22.0813 for Windows**

[Download APRSIS32.exe](https://github.com/kj4dxk/APRSIS32/releases/download/2026.11.22.0813/APRSIS32.exe) · [Release notes](https://github.com/kj4dxk/APRSIS32/releases/tag/2026.11.22.0813)

## In memory of Lynn Deffenbaugh, KJ4ERJ SK

Lynn passed away in December 2024. He loved developing APRSISCE/32 and sharing it with fellow radio amateurs. This project preserves the work he loved.

[APRS Foundation remembrance](https://www.aprsfoundation.org/lynn-kj4erj-silent-key/)

## Preserved Windows build

> **Maintainer’s note, November 21, 2026.** After comparing the available versions with those in the [APRS Foundation’s APRSISCE repository](https://github.com/APRSFoundation/APRSISCE), **KJ4DXK** identified **2024.09.05.1714** as the newest surviving Windows build he could find of APRSISCE/32 from its original developer, **Lynn W. Deffenbaugh (KJ4ERJ SK)**. The executable was linked on **September 5, 2024, at 5:14:29 p.m. EDT**. The matching source snapshot records its last main-source edit at **5:13:53 p.m. EDT** that day.

[Browse all Windows downloads](https://kj4dxk.github.io/APRSIS32/manual/downloads/). [Browse the unpacked source and build files](source/APRSISCE/).

[Read the published manual](https://kj4dxk.github.io/APRSIS32/) or [browse the Markdown articles](docs/manual/index.md) on GitHub.

This is a documentation project adapted from the APRSISCE/32 community wiki. It uses Material for MkDocs, with GitHub Actions building and publishing the site to GitHub Pages.

## Editing

Edit a Markdown file under `docs/` and open a pull request. The published site also has an edit button on each page. Navigation is configured in `mkdocs.yml`.

`docs/archive/` contains the captured HTML, figures, attachments, and crawl manifest. The [unavailable resource report](docs/archive/unavailable.txt) records links that the original crawl could not retrieve.

## Local preview

```sh
python -m pip install -r requirements.txt
python -m mkdocs serve
```

Run `python -m mkdocs build --strict` to verify the build.

## Attribution

The manual credits the original APRSISCE/32 wiki contributors. The source wiki identifies its content as [CC BY-SA 3.0](https://creativecommons.org/licenses/by-sa/3.0/) unless otherwise stated. The same license applies to adaptations of that material. Linked software, attachments, and third-party material retain their respective terms.
