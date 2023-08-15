rm -f main

g++ -O2 -I. \
    hmac_sha512.cpp \
    sha512.cpp \
    seed.cpp \
    main.cpp \
    base58.cpp \
    db.cpp \
    ./secp256k1/.libs/libsecp256k1.a \
    -lboost_system -lboost_thread \
    -lpthread -lstdc++ -lcrypto -lssl \
    -o main
