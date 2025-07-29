#!/bin/bash

: '
 * MIT License
 *
 * Copyright (c) 2025 Michael Jonker
 * Copyright (c) 2025 Zachary Kleiman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 '

# shellcheck disable=SC2034

set_build_type() {
    for arg in "$@"; do
        if [[ "$arg" == "Debug" || "$arg" == "Release" || "$arg" == "RelWithDebInfo" ]]; then
            local build_type=$arg
            if [ "$build_type" != "$BUILD_TYPE"];then
                echo "Changed build type from '$BUILD_TYPE' to '$build_type'"
                BUILD_TYPE="$build_type"
            else
                echo "Using build type '$BUILD_TYPE'"
            fi
        fi
    done
}

# params:
# $1 the browser
default_target() {
    echo "examples/$1/main.cc"
}

set_install_dir() {
    if [[ -n "$1" ]]; then
        local install_dir=$(resolve_rel_path "$1")
        mkdir -p "$install_dir"
        echo -e "The install directory was set:\n from: $INSTALL_DIR\n to: $install_dir\n"
        INSTALL_DIR=$install_dir
    fi
}

is_build_clean() {
    local clean=false
    for arg in "$@"; do
        if [ "$arg" = "--clean" ]; then
            clean=true
        fi
    done
    if [ $clean = true ]; then
        return 0
    else
        return 1
    fi
}

is_generated() {
    if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
        return 0
    else
        return 1
    fi
}

#params:
# $1 The relative path
resolve_rel_path() {
  if cd -- "$(dirname -- "$1")" 2>/dev/null; then
    echo "$(pwd)/$(basename -- "$1")"
  else
    print_error "Could not resolve path \"$1\""
    return 1
  fi
}

#params:
# $1 The absolute Windows style path
win_to_unixpath(){
    unixpath="${1//\\//}"

    if [[ "$unixpath" =~ ^([A-Za-z]):(.*)$ ]]; then
        drive=${BASH_REMATCH[1],,}
        path_rest=${BASH_REMATCH[2]}
        unixpath="/$drive$path_rest"
    fi

    echo "$unixpath"
}


print_error() {
    printf 'ERROR: %s\n\n' "$1" >&2
    return 1
}

is_help() {
    local res="false"
    while [[ $# -gt 0 ]]; do
        case "$1" in
        -h | --help)
            res="true"
            ;;
        *) ;;
        esac
        shift
    done
    if [ "$res" = "true" ];then
        return 0;
    else
        return 1;
    fi
}

print_help() {
    local message
    message=$(
        cat <<EOF
$1
EOF
    )
    echo "$message"
}

is_chromium() {
    if [ "$BROWSER" = "chromium" ]; then
        return 0
    else
        return 1
    fi
}
is_webkit() {
    if [ "$BROWSER" = "webkit" ]; then
        return 0
    else
        return 1
    fi
}
is_mswebview2() {
    if [ "$BROWSER" = "mswebview2" ]; then
        return 0
    else
        return 1
    fi
}

