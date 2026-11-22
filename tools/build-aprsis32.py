import argparse
from concurrent.futures import ThreadPoolExecutor
import os
from pathlib import Path
import subprocess
import xml.etree.ElementTree as ET

parser = argparse.ArgumentParser(description="Build the 32-bit APRSIS32 Visual Studio project from an already staged dependency tree.")
parser.add_argument("--source", type=Path, required=True, help="APRSISCE directory in the staged dependency tree")
parser.add_argument("--curl-library", type=Path, required=True, help="32-bit static Schannel libcurl library")
parser.add_argument("--output", type=Path, required=True)
args = parser.parse_args()
source = args.source.resolve()
output = args.output.resolve()
output.mkdir(parents=True, exist_ok=True)
namespace = {"ms": "http://schemas.microsoft.com/developer/msbuild/2003"}
project = source / "APRSIS32/APRSIS32.vcxproj"
entries = ET.parse(project).findall("ms:ItemGroup/ms:ClCompile", namespace)
files = [(project.parent / element.attrib["Include"]).resolve() for element in entries]
missing = [path for path in files if not path.is_file()]
if missing:
    parser.error("Missing sources:\n" + "\n".join(map(str, missing)))

code = source.parent.parent
flags = [
    "/nologo", "/c", "/MT", "/O2", "/EHsc", "/Zc:strictStrings-", "/DWIN32", "/D_WINDOWS", "/DUNICODE",
    "/D_UNICODE", "/DNDEBUG", "/D_CRT_SECURE_NO_WARNINGS", "/DCURL_STATICLIB",
    "/I" + str(code), "/I" + str(source.parent / "include"), "/I" + str(source),
]
def execute(command, cwd):
    response = subprocess.run(command, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, errors="replace")
    return response.returncode, response.stdout
def compile_one(entry):
    index, path = entry
    obj = output / f"{index:03d}_{path.stem}.obj"
    status, log = execute(["cl", *flags, "/Fo" + str(obj), str(path)], output)
    return status, str(path), log, obj

with ThreadPoolExecutor(max_workers=min(os.cpu_count() or 1, 8)) as pool:
    results = list(pool.map(compile_one, enumerate(files)))
for status, name, log, _ in results:
    if status:
        print(name, log)
if any(status for status, *_ in results):
    raise SystemExit(1)

resource = output / "APRSISCE.res"
status, log = execute(["rc", "/nologo", "/d", "UNICODE", "/d", "_UNICODE", "/fo", str(resource), "APRSISCE.rc"], source)
print(log, end="")
if status:
    raise SystemExit(status)

libraries = [
    "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32",
    "uuid", "odbc32", "odbccp32", "crypt32", "wldap32", "ws2_32", "msimg32", "comctl32",
    "dbghelp", "winmm", "iphlpapi", "version", "normaliz", "secur32", "bcrypt",
]
link_args = ["/nologo", "/SUBSYSTEM:WINDOWS", "/MACHINE:X86", "/OPT:REF", "/OPT:ICF",
             f'/OUT:"{output / "APRSIS32.exe"}"']
link_args += [f'"{obj}"' for _, _, _, obj in results]
link_args += [f'"{resource}"', f'"{args.curl_library.resolve()}"', *(lib + ".lib" for lib in libraries)]
response_file = output / "link.rsp"
response_file.write_text("\n".join(link_args), encoding="utf-8")
status, log = execute(["link", "@" + str(response_file)], output)
print(log, end="")
raise SystemExit(status)
