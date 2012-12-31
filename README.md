bChaos - molecular dynamics somulator
=====================================

COMPILING
=========
The repository contains a makefile. In order to use it you need to have some basic development tool installed.
To install these tools on Debian based distributions run the command:

  $ [sudo] apt-get install build-essentials

On Fedora based distributions you have to run the following commands as root:

  # yum groupinstall 'Development Tools'
  # yum groupinstall 'Development Libraries'

Afterwards (and after checking you have install eigen2, see below) you simply have to enter the make command in the project directory:

  $ make

DEPENDENCIES
============
The only depency is the development version of the eigen2 library. You can install them with the following commands:

Debian:
  $ [sudo] apt-get install libeigen2-dev

Fedora:
  # yum install eigen2-devel.noarch
