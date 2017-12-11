#!/bin/bash
./input.run test.txt /tmp/dirty_dishes.fifo &
./processor.run washer.conf /tmp/dirty_dishes.fifo /tmp/wet_dishes.fifo Washing 128 &
./processor.run dryer.conf /tmp/wet_dishes.fifo /tmp/dry_dishes.fifo Drying 2 &
./output.run /tmp/dry_dishes.fifo 8
