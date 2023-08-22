rm -f main
rm -f test

g++ -O2 -I. -I./crypto \
    seed.cpp \
    main.cpp \
    bip32.cpp \
    db.cpp \
    simplelogger.cpp \
    -lboost_system -lboost_thread \
    -lpthread -lstdc++ -l:libbtc.a \
    -o main

g++ -O2 -I. -I./crypto \
    seed.cpp \
    test.cpp \
    db.cpp \
    -lboost_system -lboost_thread \
    -lpthread -lstdc++ -l:libbtc.a \
    -o test

