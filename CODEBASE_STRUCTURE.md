# Base Repository Structure

## Directory Purposes

### Root Directory
- **Project documentation** — README, build instructions, developer guides
- **Build configuration** — `CMakeLists.txt`, auxiliary CMake scripts
- **Tooling settings** — configurations for formatting, linters, Git
- **Automation scripts** — dependency installation, environment setup

### `bin/`
- **Executables** — application entry point
- **CLI interface** — command line for user interaction

### `docs/`
- **Project documentation** — README, build instructions, developer guides

### `lib/`
- **Core library** — the project's core functionality
- **UI module** — user interface and CLI utilities
- **Shared components** — reusable code parts. Add new modules only under this directory.

### `tests/`
- **Unit tests** — testing individual components
- **Integration tests** — testing interactions between modules
- **Helper functions** — utilities for testing
- **Test data** — examples and fixtures for tests

### `.github/workflows/`
- **CI/CD configuration** — automated build and testing
- **Code quality checks** — static analysis, formatting
- **Deployment** — automated release publishing
