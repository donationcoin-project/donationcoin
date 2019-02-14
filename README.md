Build Instructions for QT Linux Wallet
======================================
Create a folder named Donationcoin in /home/ and unpack the contents of ~/donationcoin-master to that folder.

Install dependencies via Terminal:

$ sudo apt-get install build-essential libboost-all-dev libcurl4-openssl-dev libdb5.1-dev libdb5.1++-dev qt-sdk make qrencode libqrencode-dev

//In terminal navigate to the donationcoin folder.

$ cd /home/Donationcoin

//Enter into the terminal:

$ qmake -qt=qt4 "USE_QRCODE=1" "USE_UPNP=-"

//Then:

$ make

This will compile and build the QT Wallet which takes a little while, please be patient.

When finished you will have a file called Donationcoin-QT - Simply Double Click

//end of guide


Build Instructions for Terminal Based Linux Wallet
===================================================
//Create a folder named Donationcoin in /home/ and unpack the contents of ~/donationcoin-master to that folder.

//Install dependencies via Terminal:

$ sudo apt-get install build-essential libboost-all-dev libcurl4-openssl-dev libdb5.1-dev libdb5.1++-dev qt-sdk make 

//In terminal navigate to the donationcoin folder.

$ cd /home/Donationcoin

//Enter into the terminal:

$ make -f makefile.unix "USE_UPNP=-"

//This will produce a file named Donationcoind which is the command line instance of donationcoin-qt

//Now type:

$ strip Donationcoind

//When finished you will have a file called Donationcoind

//To run donationcoin

$ ./Donationcoind & 

//It will complain about having no Donationcoin.conf file, we'll edit the one provided and move it into place

$ nano ~/.donationcoin/donationcoin.conf

//Edit the Username and Password fields to anything you choose (but remember them) then save the file

$ cd src/
$ ./Donationcoind &

//The server will start. Here are a few commands, google for more.

$ ./Donationcoind getinfo
$ ./Donationcoind getmininginfo
$ ./Donationcoind getnewaddresss

//end of guide


Donationcoin integration/staging tree
================================

http://donationcoin.org

Copyright (c) 2013-2019 Donationcoin Developers

What is Donationcoin?
----------------

Donationcoin is a cryptocurrency for charity using scrypt as a proof-of-work algorithm.
 
- 4 minute block targets
 
- subsidy halves in 720k blocks (~4 years)
 
- ~90 million total coins
 
- 50 Coins per Block (with halving. Now at 25)

- Gravity Well Retargets Difficulty Every Block

- 20% Premine for Charity and Development

For more information, as well as an immediately useable, binary version of
the Donationcoin client sofware, see http://donationcoin.org

License
-------

Donationcoin is released under the terms of the MIT license. See `COPYING` for more
information or see http://opensource.org/licenses/MIT.

Development process
-------------------

Developers work in their own trees, then submit pull requests when they think
their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the Donationcoin
development team members simply pulls it.

The patch will be accepted if there is broad consensus that it is a good thing.
Developers should expect to rework and resubmit patches if the code doesn't
match the project's coding conventions (see `doc/coding.txt`) or are
controversial.

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/donationcoin-project/donationcoin) are created
regularly to indicate new official, stable release versions of Donationcoin.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test. Please be patient and help out, and
remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write unit tests for new code, and to
submit new unit tests for old code.

Unit tests for the core code are in `src/test/`. To compile and run them:

    cd src; make -f makefile.unix test

Unit tests for the GUI code are in `src/qt/test/`. To compile and run them:

    qmake DONATIONCOIN_QT_TEST=1 -o Makefile.test donationcoin-qt.pro
    make -f Makefile.test
    ./Donationcoin-qt_test

