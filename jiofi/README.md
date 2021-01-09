# `jiofi-info`

`jiofi-info` is a tool for getting information from a JioFi device. Currently it is tested only with `JioFi M2` (`Pegasus M2`).

At present, information which does not require login is only available.

[`jiofi.hpp`](jiofi.hpp) : Provides a header only solution for retrieving information from JioFi hotspot.

## Requirements
This tools depends on C++ development files of
- libcurl
- rapidxml

## Building
Use [`build.sh`](build.sh) to build executable in linux.

## Running
Use
```
$ ./jiofi-info
```
in linux.