import os
from typing import List

def write_markdown_from_dir(src_dir: str, output_file: str = "project_dump.md"):
    """
    Dumps all .c, .h, and .txt files in the current directory and all .txt files in ./ascii into a markdown file.
    Excludes .git and any files not matching the criteria.
    README.md first, then Makefile, then the rest in a logical order.
    All .txt files (including those in ./ascii) are placed at the end.
    """
    def file_sort_key(filename: str):
        fname = filename.lower()
        # ASCII .txt files always at the very end
        if fname.startswith("ascii/"):
            return (4, fname)
        # .txt files (not README.md) at the end, but before ascii
        if fname.endswith(".txt") and fname != "readme.md":
            return (3, fname)
        if fname == "readme.md":
            return (0, fname)
        if fname == "makefile":
            return (1, fname)
        # .c and .h files after Makefile, before .txt
        ext = os.path.splitext(fname)[1].lower()
        if ext in [".c", ".h"]:
            return (2, fname)
        return (5, fname)

    # Gather eligible files
    file_list: List[str] = []

    # Only consider files in the current directory (not subdirectories except ./ascii)
    for f in os.listdir(src_dir):
        if f.startswith('.'):
            continue
        if os.path.isdir(os.path.join(src_dir, f)):
            continue
        ext = os.path.splitext(f)[1].lower()
        if ext in [".c", ".h", ".txt"] or f.lower() in ["readme.md", "makefile"]:
            file_list.append(f)

    # Add .txt files from ./ascii
    ascii_dir = os.path.join(src_dir, "ascii")
    if os.path.isdir(ascii_dir):
        for f in os.listdir(ascii_dir):
            if f.startswith('.'):
                continue
            ext = os.path.splitext(f)[1].lower()
            if ext == ".txt":
                file_list.append(os.path.join("ascii", f))

    # Sort files for nice succession: README.md, Makefile, .c/.h, .txt, ascii .txt
    file_list.sort(key=file_sort_key)

    with open(output_file, "w", encoding="utf-8") as out:
        out.write("# Project Source Dump\n\n")
        out.write("## Game Contents\n\n")
        out.write("This document contains the full source and resources for the project, in the following order:\n\n")
        out.write("1. `README.md`\n2. `Makefile`\n3. All other files (source, headers, resources)\n\n")
        out.write("---\n\n")

        for fname in file_list:
            ext = os.path.splitext(fname)[1].lower()
            lang = ""
            if fname.lower() == "readme.md":
                lang = "markdown"
            elif fname.lower() == "makefile":
                lang = "makefile"
            elif ext in [".c", ".h"]:
                lang = "c"
            elif ext == ".txt":
                lang = ""
            else:
                lang = ""

            out.write(f"## `{fname}`\n\n")
            out.write(f"```{lang}\n")
            try:
                with open(os.path.join(src_dir, fname), "r", encoding="utf-8") as f:
                    out.write(f.read())
            except Exception as e:
                out.write(f"[Could not read file: {e}]\n")
            out.write("\n```\n\n")
        out.write("---\n")
        out.write("End of project dump.\n")

if __name__ == "__main__":
    write_markdown_from_dir(".")