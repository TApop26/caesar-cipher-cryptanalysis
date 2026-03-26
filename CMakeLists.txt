cmake_minimum_required(VERSION 4.1)
project(EXTRACREDIT C)

set(CMAKE_C_STANDARD 11)

add_executable(EXTRACREDIT main.c
        Metrics.h
        text_analysis.h
        Cipher.h
        text_analysis.c
        metrics.c
        cipher.c
)
