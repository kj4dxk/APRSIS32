"""Add the current site credits to the published copies of archived pages."""

import re
from pathlib import Path

# The original wiki placed this notice in a one-cell row or a paragraph.
LEGACY_CREDITS = re.compile(
    rb"<tr>\s*<td\b[^>]*>(?:(?!</td>).)*?registered trademark of Bob Bruninga"
    rb"(?:(?!</td>).)*?CC-BY-SA</td>\s*</tr>|"
    rb"<p>(?:(?!</p>).)*?registered trademark of Bob Bruninga"
    rb"(?:(?!</p>).)*?CC-BY-SA</p>",
    re.DOTALL,
)
BODY_END = re.compile(rb"</body\s*>|</html\s*>", re.IGNORECASE)


def on_post_build(*, config):
    archive = Path(config.site_dir) / "archive"
    credits = (
        '\n<footer aria-label="Attribution" '
        'style="max-width: 72rem; margin: 2rem auto; padding: 1rem; '
        'border-top: 1px solid; text-align: center; font-size: 0.9rem">'
        f'{config.copyright}</footer>\n'
    ).encode("utf-8")
    for page in archive.rglob("*.html"):
        source = page.read_bytes()
        if b"registered trademark of Bob Bruninga" in source:
            source = LEGACY_CREDITS.sub(b"", source)
        closing_tag = BODY_END.search(source)
        offset = closing_tag.start() if closing_tag else len(source)
        page.write_bytes(source[:offset] + credits + source[offset:])
