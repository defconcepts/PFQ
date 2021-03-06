PFQ
===

The following Guide is also available at [Getting-Starte-Guide](https://github.com/pfq/PFQ/wiki/Getting-Started-Guide).

# Table of Contents

This guide covers the following topics:

1. What is PFQ.
2. HW and SW Requirements.
3. Linux Distributions.
4. Obtaining source codes.
5. Satisfy library dependencies.
6. Build the software.
7. Software components. 

## What is PFQ

PFQ is a multi-language network monitoring framework designed for the Linux Kernel 3.x. It is highly optimized for multi-core processors, as well as for network devices equipped with multiple hardware queues (i.e. Intel 82599 10G).

PFQ consists in a Linux Kernel module and user-space libraries, for the C, C++11/14 and Haskell languages.

## HW and SW Requirements

* A 32/64-bit Linux operating system (Intel/AMD architectures are currently supported).
* Linux kernel 3.0 or higher.
* Kernel headers, required to compile modules for your kernel.
* A gcc compiler, the one used to compile the kernel in use.
* A g++ compiler (g++-4.8/clang-3.4 or higher), required to compile user-space tools and libraries.
* GHC Glasgow Haskell Compiler 7.8 or higher.
* Alex and happy tool.
* CMake and make.

## Linux distributions (GHC notes)

PFQ is developed and tested on Linux Debian Jessie. 


### Debian Stable (Jessie)

Debian Jessie is currently the stable distribution. Accidentally GHC 7.8 is not yet available from the `stable` repository. 

Hence it is recommended to either manually compile GHC or install it from a different repository. At the time of writing GHC 7.8 is available from `jessie-backports` repository and GHC 7.10 is available from the `testing` one.  

To install a package from a different repository the APT pinning is required (which allows to install packages
from one version (stable, testing, unstable) without the necessity of upgrading the entire system). 
More information is available on [Debian](https://wiki.debian.org/AptPreferences) site.

### Ubuntu 14.04.1 LTS (Trusty Tahr)

Use Hebert's PPA to install GHC and cabal-install as described at [Stackage](http://www.stackage.org/install):

```
sudo apt-get update
sudo apt-get install -y software-properties-common
sudo add-apt-repository -y ppa:hvr/ghc
sudo apt-get update
sudo apt-get install -y cabal-install-1.20 ghc-7.8.4
cat >> ~/.bashrc <<EOF
export PATH=~/.cabal/bin:/opt/cabal/1.20/bin:/opt/ghc/7.8.4/bin:$PATH
EOF
export PATH=~/.cabal/bin:/opt/cabal/1.20/bin:/opt/ghc/7.8.4/bin:$PATH
cabal update
cabal install alex happy
```

### Other Linux distributions

Follow the instructions at [Stackage](http://www.stackage.org/install) site.

## Obtaining Source Codes

Clone the source codes from the GitHub repository with The following line:
 
 `git clone https://github.com/pfq/PFQ.git`

 _master_ is the stable branch, while _experimental_ is the branch of the most recent version (possibly untested).

## Satisfy Library Dependencies

Before building the framework, ensure the Haskell libraries on which it depends are installed.

You can use the cabal tool to install them. From the base directory launch the command:

`cabal install --only-dep pfq-framework.cabal`

Because of the dependencies of package imposed by authors, it could happen that this command fails.
Hence, to relax upper bounds limits you might want to use the --allow-newer option:

`cabal install --only-dep --allow-newer pfq-framework.cabal`


## Build the software

* To build and install the framework:

`runhaskell Build.hs install --buildType=Release`

The command configures, compiles and installs the framework satisfying the dependencies and the correct order to build of various components.

* Alternatively, you can specify the list of components you want to build from the command line. The following command shows the targets available:

`runhaskell Build.hs show`

Example:

`runhaskell Build.hs install pfq.ko pfqd --buildType=Release`

## Build the software in sandbox (to avoid Cabal Hell!)

To avoid Cabal Hell, the SimpleBuilder supports building Haskell packages in a shared cabal sandbox.

First create a cabal sandbox with the following commands:

`mkdir shared-sandbox`

`cabal sandbox init --sandbox=shared-sandbox`

Then, to satisfy the pfq-framework dependencies run:

`cabal install --only-dep -j4 pfq-frmework.cabal`

All the required libraries will be installed in the sandbox.

After this you can build the framework with:

`runhaskell Build.hs install --buildType=Release --sandbox=shared-sandbox`

The PFQ Haskell library and packages will be installed in isolation within the specified sandbox directory.

## Software Components

The following components are currently part of the framework:

* pfq.ko
* pfq-clib
* pfq-cpplib
* pfq-haskell-lib
* pfq-hcounters
* pfq-htest
* irq-affinity
* pfq-omatic
* pfq-load
* pfq-stress
* pfqd
* tests
* tools
