#!/bin/bash
# shellcheck disable=SC2153
# shellcheck disable=SC2034

set_build_type() {
    for arg in "$@"; do
        if [[ "$arg" == "Debug" || "$arg" == "Release" || "$arg" == "RelWithDebInfo" ]]; then
            BUILD_TYPE=$arg
        fi
    done
}

set_target_file() {
    if [[ -z "$1" ]]; then
        throw_error "A compilation target file MUST be provided"
        return 1
    fi
    local target_file
    target_file=$(resolve_path "$1")
    if [ ! -e "$target_file" ]; then
        throw_error "Target file $target_file not found"
        return 1
    fi
    BUILD_TARGET=$target_file
    return 0
}

is_target_file_set() {
    if [[ -z "$BUILD_TARGET" ]]; then
        throw_error "\
A build target file MUST be set.
Did you forget to call 'set_target'?"
    return 1
    fi
    return 0
}

set_install_dir() {
    if [[ -n "$1" ]]; then
        INSTALL_DIR=$1
    fi
}

is_build_clean() {
    local clean=false
    for arg in "$@"; do
        if [ "$arg" = "--clean" ]; then
            clean=true
        fi
    done
    echo $clean
}

is_generated() {
    if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
        echo true
    else
        echo false
    fi
}

check_env() {
    case "$(uname | tr '[:upper:]' '[:lower:]')" in
    msys* | cygwin* | mingw*)
        # Now check if the environment is MSYS2 UCRT
        if [ "$MSYSTEM" != "UCRT64" ]; then
            throw_error "\
On Windows, you must use the MSYS2 UCRT64 terminal environment for this script.\n\
For download and installation, see: https://www.msys2.org"
            return 1
        fi
        SYS_OPTS="\
-DMSWEBVIEW_INCLUDE_DIR=$MSWEBVIEW_INCLUDE_DIR \
-DMSWEBVIEW_LIB_DIR=$MSWEBVIEW_LIB_DIR \
-DCEF_INCLUDE_DIR=$CEF_INCLUDE_DIR \
-DCEF_LIB_DIR=$CEF_LIB_DIR \
-DCEF=ON \
-DMSWEBVIEW2=ON"
        ;;
    esac

    return 0;
}


resolve_path() {
  echo "$(cd -- "$(dirname -- "$1")" && pwd)/$(basename -- "$1")"
}

throw_error() {
    printf 'ERROR: %s\n' "$1" >&2
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
    echo $res
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
