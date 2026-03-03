## Description

IDA Discord RPC is a plugin for IDA that displays your current activity as Discord Rich Presence. It enhances your Discord profile with real-time information about what you're working on inside IDA.

## Compatible

- IDA PRO/HOME 9.3 on Linux (Ubuntu x64)

## Build

Requirements:

- CMake 3.15+

- C++17 compiler

- IDA SDK 9.3 extracted locally

### Linux (Ubuntu / IDA 9.3)

```bash
cmake -S . -B build -DIDASDK_PATH=/opt/ida-pro-9.3/ida-sdk
cmake --build build -j"$(nproc)"
```

If auto-detection of the IDA SDK library fails, set it explicitly:

```bash
cmake -S . -B build \
  -DIDASDK_PATH=/opt/ida-pro-9.3/ida-sdk \
  -DIDASDK_LIB=/opt/ida-pro-9.3/libida.so
```

Output plugin: `build/ida_plugins/ida_discord_rpc.so`

## Installation

Linux:

```bash
cp build/ida_plugins/ida_discord_rpc.so ~/.idapro/plugins/
```

You can also copy it to `<IDA_INSTALL_DIR>/plugins`.

After restarting IDA, the plugin will run in the background.
It does not appear in the plugin menu, as it is designed to be invisible during normal use.

## Updates

Click here to check the latest releases: [here](https://github.com/reversedcodes/IDA-RPC/releases)

## License

This project is licensed under the [Apache License 2.0](LICENSE).  
You are free to use, modify, and distribute this software under the terms of the license.
