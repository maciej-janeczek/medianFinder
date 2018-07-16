#!/usr/bin/python2.7

import argparse
import os, sys
import subprocess
import time
import shutil
import traceback


AVAILABLE_PROFILES = [ 'debug', 'release' ]
DEFAULT_PROFILE = 'release'


def parseArgs(sysargs):
    parser = argparse.ArgumentParser(description="Build UIALogsParser")

    parser.add_argument(
        '-p',
        '--profile',
        default = DEFAULT_PROFILE,
        choices = AVAILABLE_PROFILES,
        help = "Select the build profile (Default: %(default)s)"
    )

    parser.add_argument(
        '-c',
        '--clean',
        help = "Clean built targets",
        action = "store_true"
    )

    parser.add_argument(
        '-v',
        '--verbose',
        help = "Enables verbose mode at build",
        action = "store_true"
    )
    args = parser.parse_args(sysargs)
    printParameters(args)
    
    return args
    
    
def printParameters(args):
    log("\nUsed parameters:\n")
    
    for attr, value in args.__dict__.iteritems():
        log("%15s: %s" % (attr, value))
        
    log("")

def executeSystemCmd(command):
    startTime = time.time()
    
    try:
        log('\t[Executing command] ' + command)
        process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        
        while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                break
            output = output.strip()   
            
            if output:
                log(output)
        exitCode = process.poll()
    
        if exitCode != 0:
            log(' ### Error: Command returned non zero exit code: ' + str(exitCode))
            sys.exit(exitCode)
    
    except Exception as error:
        log (' ### Error: ' + repr(error) + ': ' + str(error))
        traceback.log_exc()
        sys.exit(1)

    finally:
        endTime = time.time()
        hours, rem = divmod(endTime - startTime, 3600)
        minutes, seconds = divmod(rem, 60)
        log('\t[Command time: {:0>2}:{:0>2}:{:05.2f}]'.format(int(hours), int(minutes), seconds))
        

def log(msg):
    sys.stdout.write(msg + '\n')
    sys.stdout.flush()
        
        
def mkCmd(argsList):
    return " ".join(argsList)
    
def copyFile(src, dest):
    log("\t[Copying file] '" + src + "' --> '" + dest + "'")
    shutil.copy2(src, dest) 

def main():
    args = parseArgs(sys.argv[1:])

    currentDir = os.path.abspath(os.path.dirname(__file__))
    
    # Repo path:
    basedir = os.environ['cagr']

    mingwDir = os.path.join(basedir, 'partners', 'open_source', 'mingw32', 'bin') 
    os.environ['PATH'] = os.path.join(basedir, 'partners', 'open_source', 'cmake', 'cmake-3.5.0-win32-x86', 'bin') + os.pathsep + os.environ['PATH']
    os.environ['PATH'] = mingwDir + os.pathsep + os.environ['PATH']
    
    cmake_outdir = os.path.join(currentDir, 'build')
    
    if not os.path.exists(cmake_outdir):
        log("\t[Creating directory: " + cmake_outdir + "]")
        os.makedirs(cmake_outdir)
            
    log("\t[Changing directory to: " + cmake_outdir + "]")
    os.chdir(cmake_outdir)
    
    cmd = mkCmd([ 
        "cmake", 
        "..", 
        "-G",
        "Ninja", 
        "-DCMAKE_BUILD_TYPE=" + args.profile,
    ])
    executeSystemCmd(cmd)
    
    cmd = mkCmd([ 
        "ninja",
        ("-v" if args.verbose else ""),
        ("-t clean" if args.clean else "")
    ])
    executeSystemCmd(cmd)
    
    copyFile(os.path.join(mingwDir, 'libgcc_s_dw2-1.dll'), cmake_outdir)
    copyFile(os.path.join(mingwDir, 'libstdc++-6.dll'), cmake_outdir)
    copyFile(os.path.join(mingwDir, 'libwinpthread-1.dll'), cmake_outdir)
    
    
if __name__ == "__main__":
    main()