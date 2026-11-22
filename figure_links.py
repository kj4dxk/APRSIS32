"""Resolve repository-relative figure-table links in the built manual."""

import re
from html import escape, unescape
from posixpath import dirname, join, normpath
from urllib.parse import urlsplit, urlunsplit

from mkdocs.utils import get_relative_url

TABLE_OR_LINK = re.compile(r'<table\b[^>]*>|</table\s*>|\b(href|src)="([^"]*)"', re.IGNORECASE)


def on_page_markdown(markdown, *, page, files, **kwargs):
    def convert_link(match):
        attribute, value = match.groups()
        parsed = urlsplit(unescape(value))
        if parsed.scheme or parsed.netloc or not parsed.path or parsed.path.startswith("/"):
            return match[0]
        source = normpath(join(dirname(page.file.src_uri), parsed.path))
        target = files.get_file_from_path(source)
        if target is None:
            return match[0]
        relative_path = get_relative_url(target.url, page.url)
        url = urlunsplit(("", "", relative_path, parsed.query, parsed.fragment))
        return f'{attribute}="{escape(url, quote=True)}"'

    tables = []

    def convert(match):
        token = match[0]
        if token.lower().startswith("<table"):
            tables.append("aprs-figure-table" in token)
        elif token.lower().startswith("</table"):
            if tables:
                tables.pop()
        elif any(tables):
            return convert_link(match)
        return token

    return TABLE_OR_LINK.sub(convert, markdown)


# Markdown tables without a source header can acquire an empty header row.
EMPTY_TABLE_HEADER = re.compile(r"<thead>\s*<tr>\s*(?:<th[^>]*>\s*</th>\s*)+</tr>\s*</thead>")


def on_page_content(html, **kwargs):
    return EMPTY_TABLE_HEADER.sub("", html)
