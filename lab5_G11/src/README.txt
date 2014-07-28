{\rtf1\ansi\ansicpg1252\cocoartf1265\cocoasubrtf210
{\fonttbl\f0\froman\fcharset0 TimesNewRomanPSMT;}
{\colortbl;\red255\green255\blue255;}
\paperw11900\paperh16840\margl1440\margr1440\vieww17420\viewh10800\viewkind0
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural

\f0\fs28 \cf0 To run the 
\b PROCESS
\b0  solution to the producer-consumer problem with a bounded buffer, do the following:\

\b 1. 
\b0 gcc produce.c -o produce.out -lrt\
    produce.c is the process that generates (through fork/spawn) the other processes (producers and consumers)\

\b 2. 
\b0 gcc producer.c -o producer.out -lrtkk\

\b 3. 
\b0 gcc consumer.c -o consumer.out -lrt -lm\
   -lrt is used to link against the real-time library\
   -lm is used because of the math operation inside consumer (square root)\

\b 4. 
\b0 Finally, execute the program\
   ./produce.out <N> <B> <P> <C>\
   N - quantity of items to be produced and consumed\
   B - the size of the buffer\
   P - number of producers\
   C - number of consumers}