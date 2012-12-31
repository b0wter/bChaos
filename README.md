bChaos - molecular dynamics somulator
=====================================

What is is?
-----------
bChaos is a molecular dynamics simulator that implements a couple of basic potentials (Coulomb, gravitational) and a very special harmonic potential that was used to evaluate a certain behaviour of particle groups (details can be found [here][1]).

Compiling
=========
The repository contains a makefile. In order to use it you need to have some basic development tool installed.
To install these tools on Debian based distributions run the command:

    $ [sudo] apt-get install build-essentials'

On Fedora based distributions you have to run the following commands as root:

    # yum groupinstall 'Development Tools''
    # yum groupinstall 'Development Libraries''

Afterwards (and after checking you have install eigen2, see below) you simply have to enter the make command in the project directory:

    $ make

Dependencies
============
The only depency is the development version of the eigen2 library. You can install them with the following commands:
Debian:
    $ [sudo] apt-get install libeigen2-dev
Fedora:
    # yum install eigen2-devel.noarch
Be careful to not install version 3 of eigen.

Links
=====
[1]: 'https://github.com/b0wter/DA'  "Spreading in integrable and non-integrable multi body systems"
