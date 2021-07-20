#!/bin/bash

TIMEOUT=12h

timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property6a.elf 1 ./reluplex/logs/property6_summary.txt 2>&1 | tee ./reluplex/logs/property6_lower_stats_1.txt
timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property6a.elf 2 ./reluplex/logs/property6_summary.txt 2>&1 | tee ./reluplex/logs/property6_lower_stats_2.txt
timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property6a.elf 3 ./reluplex/logs/property6_summary.txt 2>&1 | tee ./reluplex/logs/property6_lower_stats_3.txt
timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property6a.elf 4 ./reluplex/logs/property6_summary.txt 2>&1 | tee ./reluplex/logs/property6_lower_stats_4.txt

timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property6b.elf 1 ./reluplex/logs/property6_summary.txt 2>&1 | tee ./reluplex/logs/property6_upper_stats_1.txt
timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property6b.elf 2 ./reluplex/logs/property6_summary.txt 2>&1 | tee ./reluplex/logs/property6_upper_stats_2.txt
timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property6b.elf 3 ./reluplex/logs/property6_summary.txt 2>&1 | tee ./reluplex/logs/property6_upper_stats_3.txt
timeout --foreground --signal=SIGQUIT $TIMEOUT ./reluplex/check_properties/bin/property6b.elf 4 ./reluplex/logs/property6_summary.txt 2>&1 | tee ./reluplex/logs/property6_upper_stats_4.txt
