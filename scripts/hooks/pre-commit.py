#!.venv/bin/python3

import re
import os
import subprocess

version_file_path = "src/steikemann/src/version.h"
version_patch: int

# Bump Patch Version
with open(str(version_file_path), 'r+') as version_file:
    buffer: str = ""
    found_match: bool = False
    for line in version_file.readlines():
        if found_match is False:
            patch: str = "STEIKEMANN_PATCH"
            match_br = re.search(patch, line)
            if match_br is not None:
                first_number_regex = "\d+"
                match_br = re.search(first_number_regex, line)
                assert match_br is not None

                version_patch = int(match_br.group(0))
                increment_patch_version = int(match_br.group(0)) + 1

                ## Write this line to file
                line = re.sub(first_number_regex, str(increment_patch_version), line)
                found_match = True
        buffer += line
    assert buffer is not ""

    # Overwrite file
    version_file.seek(0)
    version_file.write(buffer)
    version_file.truncate()

subprocess.call(["git", "add", str(version_file_path)])
subprocess.call(["git", "commit", "-m", "Version Patch: ", version_patch])
