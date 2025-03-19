#!.venv/bin/python3

import re

# Bump Patch Version
with open("src/steikemann/src/version.h", 'r+') as version_file:
    buffer: str = ""
    found_match: bool = False
    for line in version_file.readlines():
        if found_match is False:
            patch: str = "STEIKEMANN_PATCH"
            match_br = re.search(patch, line)
            if match_br is not None:
                # first_number_regex = "([0-9]+ .*)"
                first_number_regex = "\d+"
                match_br = re.search(first_number_regex, line)
                assert match_br is not None

                print("PATCH_VERSION: ")
                print(match_br.group(0))
                increment_patch_version = int(match_br.group(0)) + 1
                print(increment_patch_version)

                ## Write this line to file
                line = re.sub(first_number_regex, str(increment_patch_version), line)
                found_match = True
        print(f"line: {line}")
        buffer += line
    assert buffer is not ""

    # Overwrite file
    version_file.seek(0)
    version_file.write(buffer)
    version_file.truncate()
