# APRSISCE/32 manual

## Current Windows build

> **Maintainer’s note, November 21, 2026.** After comparing the available versions with those in the [APRS Foundation’s APRSISCE repository](https://github.com/APRSFoundation/APRSISCE), **KJ4DXK** identified the Windows build listed here as **October 10, 2025** as the newest build he could find of the APRSISCE/32 software left by its original developer, **Lynn W. Deffenbaugh (KJ4ERJ SK)**.

[Get the current Windows build](https://kj4dxk.github.io/APRSIS32/manual/downloads/).

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
