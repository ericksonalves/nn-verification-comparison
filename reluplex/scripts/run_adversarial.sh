#!/bin/bash

./reluplex/check_properties/bin/adversarial.elf ./reluplex/logs/adversarial_summary.txt 2>&1 | tee ./reluplex/logs/adversarial_stats.txt
