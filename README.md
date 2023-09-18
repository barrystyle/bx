# bx/milksad scanner



### recommended reading

https://milksad.info/

https://milksad.info/disclosure.html

https://blog.ledger.com/Funds-of-every-wallet-created-with-the-Trust-Wallet-browser-extension-could-have-been-stolen/


### performance

debug (in nanoseconds) in individual functions:

* main.cpp (line 20)
* db.cpp (line 12)
* bip32.cpp (line 10)

can be made by setting DEBUG to 1 in the above functions,
providing nanosecond accuration timing for the iterations
performed in each.


### entropy

entropy length can be set for following bitwidth:

* 128 bits = 12 words
* 192 bits = 18 words (default for libbitcoin)
* 256 bits = 24 words

this is set on line 47 of main.cpp


### address balance db

the program attempts to match generated addresses from those in a
database read into ram (https://bitcointalk.org/index.php?topic=5254914.0).
format is one address per line, without balance.

the filename is set at line 64 of db.cpp


### concurrent cores

the amount of threads launched/work divided is set at line 21 of main.cpp.
the work range (4.3bil addresses) is evenly divided amongst the cores.


### found matches

are logged by simplelogger.cpp, to filename set at line 9 ('output.log').
this is always set to append, so wont overwrite results found on previous runs.


### getting funds from found matches

if a match is found, the 'entropy' bytes are written to the log file. from there
they can be input into https://iancoleman.io/bip39/ (set 'show entropy details'),
the actual derived keys will then be displayed underneath (public/private keys).

alternatively, you can paste the bip39 mnemonic directly into electrum during
wallet import to get at the funds.


### compilation

once libbtc has been installed to system, simply:

./make

to run:

./main
