FROM ubuntu:24.04 AS base


RUN export DEBIAN_FRONTEND=noninteractive \
    && apt-get -qq update && apt-get install -yq --no-install-recommends \
    ccache \
    cmake \
    g++-14 \
    g++-14-multilib \
    gcc-14 \
    git \
    gnupg \
    libfmt-dev \
    libgtest-dev \
    lsb-release \
    ninja-build \
    python3-pip \
    python3-setuptools \
    python3-venv \
    python3-wheel \
    software-properties-common \
    wget \
    && rm -rf /var/lib/apt/lists/* \
    && rm /tmp/* -rf

RUN wget https://apt.llvm.org/llvm.sh && chmod +x llvm.sh && ./llvm.sh 19 all

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14    10  && \
    update-alternatives --install /usr/bin/cc cc  /usr/bin/gcc-14     10  && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14    10  && \
    update-alternatives --install /usr/bin/cxx cxx /usr/bin/g++-14    10

RUN update-alternatives --install /usr/bin/clang-tidy   clang-tidy   /usr/bin/clang-tidy-19 10 && \
    update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-19 10
