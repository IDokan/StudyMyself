# Copyright 2018 Rudy Castan - All Rights Reserved

import os
import re
import subprocess
import sys
import threading
import traceback


class RunResult(object):
    def __init__(self, out, err, status, timed_out):
        # type: (str, str, int, bool) -> RunResult
        self.output = out
        self.error = err
        self.return_code = status
        self.did_timeout = timed_out


def run(cmd, echo=True, exit_on_error=False, cwd=None, out_file=None, error_file=None,
        in_file=None, timeout_in_seconds=None):
    command = TimeoutCommand(cmd, echo, cwd, in_file, exit_on_error, out_file, error_file)
    command.run(timeout_in_seconds)
    return RunResult(command.output, command.error, command.status, command.did_timeout)


def format_cmd(cmd):
    # Split string by spaces but keep spaces within quotes
    pattern = re.compile(r'''((?:[^\s"']|"[^"]*"|'[^']*')+)''')
    commands = pattern.split(cmd)[1::2]
    # remove outside double quotes. this makes commands like 'svn add "C:\temp\path name with space"' work
    commands = [w.strip().strip('"') for w in commands]
    return commands


def call(cmd, echo=True, cwd=None):
    formatted_cmd = format_cmd(cmd)
    if echo:
        print(formatted_cmd)
        return subprocess.call(formatted_cmd, cwd=cwd)

    proc = subprocess.Popen(formatted_cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    proc.wait()
    return proc.returncode


class TimeoutCommand(object):
    """
    Enables to run subprocess commands in a different thread with TIMEOUT option.
    Based on jcollado's solution:
    http://stackoverflow.com/questions/1191374/subprocess-with-timeout/4825933#4825933
    """
    process = None
    status = None
    output, error = '', ''
    did_timeout = False

    def __init__(self, cmd, echo=True, cwd=None, in_file=None, exit_on_error=False, out_file=None, error_file=None):
        formatted_cmd = format_cmd(cmd)
        if echo:
            print(formatted_cmd)

        if cwd:
            exe_path_in_cwd = os.path.realpath(os.path.join(cwd, formatted_cmd[0]))
            if os.path.isfile(exe_path_in_cwd):
                formatted_cmd[0] = exe_path_in_cwd
            else:
                exe_path_in_cwd += '.exe'
                if os.path.isfile(exe_path_in_cwd):
                    formatted_cmd[0] = exe_path_in_cwd

        self.cwd = cwd
        self.echo = echo
        self.exit_on_error = exit_on_error
        self.out_file = out_file
        self.error_file = error_file
        self.input_filehandle = None
        if in_file:
            self.input_filehandle = open(in_file)
        self.process = subprocess.Popen(formatted_cmd,
                                        cwd=cwd,
                                        stdin=self.input_filehandle,
                                        stdout=subprocess.PIPE,
                                        stderr=subprocess.PIPE)

    def run(self, timeout_seconds=None):
        """ Run a command then return: (status, output, error). """

        def target():
            try:
                self.output, self.error = self.process.communicate()
            except:
                self.error = traceback.format_exc()
                self.status = -1

        # thread
        thread = threading.Thread(target=target)
        thread.start()
        thread.join(timeout_seconds)
        if thread.is_alive():
            try:
                self.process.terminate()
                thread.join()
            except:
                pass
            self.did_timeout = True

        self.status = self.process.returncode

        if self.input_filehandle:
            self.input_filehandle.close()

        if self.echo:
            if self.output:
                print(self.output)
            if self.error:
                print(self.error)

        if self.out_file:
            with open(self.out_file, 'wt') as fh:
                fh.write(self.output)
        if self.error_file:
            with open(self.error_file, 'wt') as fh:
                fh.write(self.error)

        if self.exit_on_error and self.status != 0:
            sys.exit(self.status)
