rm -f main
rm -f test

g++ -O2 -I. -I./crypto \
    crypto/hmac_sha512.cpp \
    crypto/sha512.cpp \
    seed.cpp \
    main.cpp \
    base58.cpp \
    bip32.cpp \
    db.cpp \
    -lboost_system -lboost_thread \
    -lpthread -lstdc++ -lcrypto -lssl \
    -l:libbtc.a \
    -o main

g++ -O2 -I. -I./crypto \
    crypto/hmac_sha512.cpp \
    crypto/sha512.cpp \
    seed.cpp \
    test.cpp \
    base58.cpp \
    db.cpp \
    -lboost_system -lboost_thread \
    -lpthread -lstdc++ -lcrypto -lssl \
    -o test

