#!/bin/bash

TIMEOUT=12h

timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property7.elf 3 ./reluplex/logs/property7_summary.txt 2>&1 | tee ./reluplex/logs/property7_stats_3.txt

timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property7.elf 4 ./reluplex/logs/property7_summary.txt 2>&1 | tee ./reluplex/logs/property7_stats_4.txt
