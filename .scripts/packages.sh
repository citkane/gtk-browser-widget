#!/bin/bash

BROWSER=""
BROWSER_INCLUDE_DIR=""
BROWSER_LIB_DIR=""

NUGET=$PACKAGES_DIR/bin/nuget.exe
PACKAGES_DOWNLOAD_DIR="$PACKAGES_DIR/.downloads"

MSWEBVIEW_NUGET_V=1.0.3351.48
CHROMIUM_FIRST_PART_V=138.0.33
CHROMIUM_SECOND_PART_V=g276ed6d
CHROMIUM_THIRD_PART_V=138.0.7204.169
CEF_TARGET_DIR="$PACKAGES_DIR/cef"


WEBKIT_WPE_V=2.48.3
WEBKIT_BACKEND_V=1.16.0
WEBKIT_LIB_V=1.16.2
WEBKIT_TARGET_DIR="$PACKAGES_DIR/webkit"

SYS_OPTS=""

set_sys_opts() {
    if [ "$BROWSER" = "chromium" ]; then
        SYS_OPTS="\
        -DBROWSER_INCLUDE_DIR=$PACKAGES_DIR/chromiumembeddedframework.runtime.$CHROMIUM_NUGET_V/build/native/include \
        -DBROWSER_LIB_DIR=$PACKAGES_DIR/chromiumembeddedframework.runtime.$CHROMIUM_NUGET_V/build/native/x64"
    elif [ "$BROWSER" = "mswebview2" ]; then
        SYS_OPTS="\
        -DBROWSER_INCLUDE_DIR=$PACKAGES_DIR/Microsoft.Web.WebView2.$MSWEBVIEW_NUGET_V/build/native/include \
        -DBROWSER_LIB_DIR=$PACKAGES_DIR/Microsoft.Web.WebView2.$MSWEBVIEW_NUGET_V/build/native/x64"
    elif [ "$BROWSER" = "webkit" ]; then
        SYS_OPTS="\
        -DBROWSER_INCLUDE_DIR=$PACKAGES_DIR/webkit-$WEBKIT_WPE_V/include \
        -DBROWSER_LIB_DIR=$PACKAGES_DIR/webkit-$WEBKIT_WPE_V/lib"
    fi
}

install_cef() {
    # Nuget doesn't work fill in once install is complete
    echo "nothing"
}

install_mswebview2() {   
    verify_nuget 
    $NUGET install Microsoft.Web.WebView2 -Version $MSWEBVIEW_NUGET_V -OutputDirectory "$PACKAGES_DIR"
    echo "MS WebView2 installed successfully"
}

install_webkit() {
    mkdir -p "$WEBKIT_TARGET_DIR"

    local wpe_src_dir="$WEBKIT_TARGET_DIR/wpewebkit-$WEBKIT_WPE_V"
    local backend_src_dir="$WEBKIT_TARGET_DIR/wpebackend-fdo-$WEBKIT_BACKEND_V"
    local lib_src_dir="$WEBKIT_TARGET_DIR/libwpe-$WEBKIT_LIB_V"

    if ! download_webkit "$wpe_src_dir" "$backend_src_dir" "$lib_src_dir"; then
        return 1;
    fi

    export PKG_CONFIG_PATH="$WEBKIT_TARGET_DIR/lib/x86_64-linux-gnu/pkgconfig:$PKG_CONFIG_PATH"

    meson setup "$lib_src_dir/build" "$lib_src_dir" \
        --prefix="$WEBKIT_TARGET_DIR" \
        --buildtype=release
    meson compile -C "$lib_src_dir/build"
    meson install -C "$lib_src_dir/build"

    meson setup "$backend_src_dir/build" "$backend_src_dir" \
        --prefix="$WEBKIT_TARGET_DIR" \
        --buildtype=release
    meson compile -C "$backend_src_dir/build"
    meson install -C "$backend_src_dir/build"

    cmake -B "$wpe_src_dir/build" -GNinja -S "$wpe_src_dir" \
        -DCMAKE_INSTALL_PREFIX="$WEBKIT_TARGET_DIR" \
        -DCMAKE_BUILD_TYPE=Release \
        -DPORT=WPE \
        -DENABLE_DOCUMENTATION=OFF \
        -DENABLE_JOURNALD_LOG=OFF \
        -DUSE_LIBBACKTRACE=OFF
    ninja -C "$wpe_src_dir/build"
    ninja -C "$wpe_src_dir/build" install

}

# params:
# $1 - wpe_src_dir
# $2 - backend_src_dir
# $3 - lib_src_dir
download_webkit() {
    local wpe_uri="https://wpewebkit.org/releases/wpewebkit-$WEBKIT_WPE_V.tar.xz"
    local wpe_sum_uri="https://wpewebkit.org/releases/wpewebkit-$WEBKIT_WPE_V.tar.xz.sums"
    
    local backend_uri="https://wpewebkit.org/releases/wpebackend-fdo-$WEBKIT_BACKEND_V.tar.xz"
    local backend_sums_uri="https://wpewebkit.org/releases/wpebackend-fdo-$WEBKIT_BACKEND_V.tar.xz.sums"
    
    local lib_uri="https://wpewebkit.org/releases/libwpe-$WEBKIT_LIB_V.tar.xz"
    local lib_sums_uri="https://wpewebkit.org/releases/libwpe-$WEBKIT_LIB_V.tar.xz.sums"
    

    if ! download_tar_xz $wpe_uri $wpe_sum_uri "$1"; then
        return 1
    fi
    if ! download_tar_xz $backend_uri $backend_sums_uri "$2"; then
        return 1
    fi
    if ! download_tar_xz $lib_uri $lib_sums_uri "$3"; then
        return 1
    fi
}

# params:
# $1 - cef_src_dir
# $2 - backend_src_dir
# $3 - lib_src_dir
download_cef_windows_64() {
    local cef_uri="https://cef-builds.spotifycdn.com/cef_binary_${CHROMIUM_FIRST_PART_V}%2B${CHROMIUM_SECOND_PART_V}%2Bchromium-${CHROMIUM_THIRD_PART_V}_windows64.tar.bz2"
    local sha_1_uri="https://cef-builds.spotifycdn.com/cef_binary_${CHROMIUM_FIRST_PART_V}%2B${CHROMIUM_SECOND_PART_V}%2Bchromium-${CHROMIUM_THIRD_PART_V}_windows64.tar.bz2.sha1"
    local cef_target_dir="$PACKAGES_DIR/cef"  # Define a valid target directory

    if ! download_tar_bz2 "$cef_uri" "$sha_1_uri" "$cef_target_dir"; then
        return 1
    fi
}

download_cef_windows_32() {
    local cef_uri="https://cef-builds.spotifycdn.com/cef_binary_${CHROMIUM_FIRST_PART_V}%2B${CHROMIUM_SECOND_PART_V}%2Bchromium-${CHROMIUM_THIRD_PART_V}_windows32.tar.bz2"
    local sha_1_uri="https://cef-builds.spotifycdn.com/cef_binary_${CHROMIUM_FIRST_PART_V}%2B${CHROMIUM_SECOND_PART_V}%2Bchromium-${CHROMIUM_THIRD_PART_V}_windows32.tar.bz2.sha1"
    local cef_target_dir="$PACKAGES_DIR/cef"  # Define a valid target directory

    if ! download_tar_bz2 "$cef_uri" "$sha_1_uri" "$cef_target_dir"; then
        return 1
    fi
}

download_cef_mac_64_bit() {
    local cef_uri="https://cef-builds.spotifycdn.com/cef_binary_${CHROMIUM_FIRST_PART_V}%2B${CHROMIUM_SECOND_PART_V}%2Bchromium-${CHROMIUM_THIRD_PART_V}_macosx64.tar.bz2"
    local sha_1_uri="https://cef-builds.spotifycdn.com/cef_binary_${CHROMIUM_FIRST_PART_V}%2B${CHROMIUM_SECOND_PART_V}%2Bchromium-${CHROMIUM_THIRD_PART_V}_macosx64.tar.bz2.sha1"
    local cef_target_dir="$PACKAGES_DIR/cef"  # Define a valid target directory

    if ! download_tar_bz2 "$cef_uri" "$sha_1_uri" "$cef_target_dir"; then
        return 1
    fi
}

download_cef_linux_64_bit() {
    local cef_uri="https://cef-builds.spotifycdn.com/cef_binary_${CHROMIUM_FIRST_PART_V}%2B${CHROMIUM_SECOND_PART_V}%2Bchromium-${CHROMIUM_THIRD_PART_V}_linux64.tar.bz2"
    local sha_1_uri="https://cef-builds.spotifycdn.com/cef_binary_${CHROMIUM_FIRST_PART_V}%2B${CHROMIUM_SECOND_PART_V}%2Bchromium-${CHROMIUM_THIRD_PART_V}_linux64.tar.bz2.sha1"
    local cef_target_dir="$PACKAGES_DIR/cef"  # Define a valid target directory

    if ! download_tar_bz2 "$cef_uri" "$sha_1_uri" "$cef_target_dir"; then
        return 1
    fi
}
# params:
# $1 - the tar.bz2 file url
# $2 - the tar.bz2.sums checksum file url
# $3 - the target extraction dir
download_tar_bz2() {
    if [ -d "$3" ]; then
        return 0
    fi

    local dl_target
    local dl_sums_target
    dl_target="$PACKAGES_DOWNLOAD_DIR/$(basename "$1")"
    dl_sums_target="$PACKAGES_DOWNLOAD_DIR/$(basename "$2")"

    if [ ! -f "$dl_target" ]; then
        curl -o "$dl_target" "$1" --create-dirs
    fi
    if [ ! -f "$dl_sums_target" ]; then
        curl -o "$dl_sums_target" "$2" --create-dirs
    fi
    # Ensure the target extraction directory exists
    mkdir -p "$3"

    if check_sha1sum "$dl_target" "$dl_sums_target"; then
        tar -xf "$dl_target" -C "$3"
    fi
}


# params:
# $1 - the tar.xz file url
# $2 - the tar.xz.sums checksum file url
# $3 - the target extraction dir
download_tar_xz() {
    if [ -d "$3" ]; then
        return 0
    fi

    local dl_target
    local dl_sums_target
    dl_target="$PACKAGES_DOWNLOAD_DIR/$(basename "$1")"
    dl_sums_target="$PACKAGES_DOWNLOAD_DIR/$(basename "$2")"

    if [ ! -f "$dl_target" ]; then
        curl -o "$dl_target" "$1" --create-dirs
    fi
    if [ ! -f "$dl_sums_target" ]; then
        curl -o "$dl_sums_target" "$2" --create-dirs
    fi
    
    if check_sha256sum "$dl_target" "$dl_sums_target"; then
        tar -xf "$dl_target" -C "$WEBKIT_TARGET_DIR"
    fi
}

# params:
# $1 - the tar.xz file path
# $2 - the tar.xz.sums checksum file path
check_sha256sum() {
    local hash
    local hash_file_path
    local file_name

    file_name=$(basename "$1")
    hash_file_path="$PACKAGES_DOWNLOAD_DIR/$file_name.sha256"
    hash=$(grep 'sha256sum:' "$2" | awk '{print $2}')

    echo "$hash  $1" > "$hash_file_path"
    sha256sum -c "$hash_file_path"
}

check_sha1sum() {
    local hash
    local temp_hash_file
    local file_name

    file_name=$(basename "$1")
    temp_hash_file="$PACKAGES_DOWNLOAD_DIR/$file_name.sha1.tmp"

    # Read the hash directly from the .sha1 file
    hash=$(awk '{print $1}' "$2")  # Extract only the checksum

    # Format the hash file for sha1sum
    echo "$hash  $1" > "$temp_hash_file"

    # Verify the checksum
    if ! sha1sum -c "$temp_hash_file"; then
        echo "SHA1 checksum verification failed!"
        rm -f "$temp_hash_file"
        return 1
    fi

    # Clean up the temporary file
    rm -f "$temp_hash_file"
}
verify_nuget() {
    if [ ! -f "$NUGET" ]; then
        curl -o "$NUGET" https://dist.nuget.org/win-x86-commandline/latest/nuget.exe --create-dirs
    fi
}
