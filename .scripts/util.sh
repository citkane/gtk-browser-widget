#!/bin/bash
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
        local install_dir=$(resolve_path "$1")
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

resolve_path() {
  if cd -- "$(dirname -- "$1")" 2>/dev/null; then
    echo "$(pwd)/$(basename -- "$1")"
  else
    return 1
  fi
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
