#!/bin/bash
# shellcheck disable=SC2034

set_build_type() {
    for arg in "$@"; do
        if [[ "$arg" == "Debug" || "$arg" == "Release" || "$arg" == "RelWithDebInfo" ]]; then
            BUILD_TYPE=$arg
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

resolve_path() {
  echo "$(cd -- "$(dirname -- "$1")" && pwd)/$(basename -- "$1")"
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
