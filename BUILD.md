# Build instructions for Ovum toolset

This documents describes build process for Ovum programming language toolset and tests.

## How to build and run

Run the following commands from the project directory.

1. Create CMake cache

```shell
cmake -S . -B cmake-build
```

2. Build all targets

```shell
cmake --build cmake-build
```

4. Run main executable

* On Windows:

```shell
.\cmake-build\ovum.exe
```

* On *nix:

```shell
./cmake-build/bin/ovum World
```

5. Run tests

* On Windows:

```shell
.\cmake-build\ovum_tests.exe
```

* On *nix:

```shell
./cmake-build/tests/ovum_tests
```

6. Installation

Running this script will build and install Ovum main executable at `$HOME/ovum`.
Define `$SAVE_PREV` environment variable to save previous configuration.

> There is only a shell script, so on Windows one should use Git Bash.

```shell
./install.sh
```