FROM mcr.microsoft.com/devcontainers/cpp:0-debian-11

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="none"

# [Optional] Uncomment this section to install additional vcpkg ports.
# RUN su vscode -c "${VCPKG_ROOT}/vcpkg install <your-port-name-here>"

# [Optional] Uncomment this section to install additional packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends bison flex
