#!/bin/bash

TIMEOUT=12h

timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property8.elf 3 ./reluplex/logs/property8_summary.txt 2>&1 | tee ./reluplex/logs/property8_stats_3.txt
