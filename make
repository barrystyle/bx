rm -f main

g++ -O2 -I. -I./crypto \
    entropy.cpp \
    main.cpp \
    bip32.cpp \
    bip39.cpp \
    crypto/sha256*.cpp \
    crypto/sha512.cpp \
    crypto/hmac_sha512.cpp \
    crypto/pkcs5_pbkdf2_hmac_sha512.cpp \
    db.cpp \
    simplelogger.cpp \
    util.cpp \
    -lboost_system -lboost_thread \
    -lpthread -lstdc++ -l:libbtc.a \
    -march=native
    -o main
