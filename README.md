# Stompede - a STOMP Messaging Server

*Stompede* is written in C++ and aims for :

- High Scalability
- Low Latency
- High Throughput
- clean source code design
- STOMP 1.2 conformance (however, other network messaging protocols may be considered in the long term too)

## Software Composition Concept

The software is divided into:

- `stompeded`, the binary to run the STOMP broker daemon (located in `./src/*.*`)
- `libstompede`, a library that implements the STOMP API (located in `./lib/`)
  - Core Messaging API, (located in `./lib/stompede/*.*`)
  - STOMP Framing API, (located in `./lib/stompede/stomp/*.*`)

