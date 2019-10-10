# Copyright 2018 Rudy Castan - All Rights Reserved

import sys
import terminal

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print 'Usage: watchdog.py timeout_in_milliseconds exe_to_run [exe options]'
        exit(1)

    to_seconds = float(sys.argv[1]) / 1000.0
    command = ' '.join(sys.argv[2:])
    results = terminal.run(command, echo=False, timeout_in_seconds=to_seconds)
    if results.output:
        sys.stdout.write(results.output)
    if results.error:
        sys.stderr.write(results.error)
    exit(results.return_code)

