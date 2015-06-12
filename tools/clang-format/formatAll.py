'''
Python script to format code using clang-format tool.

The script must be run using Python 3.x (not compatible with Python2) and it support Windows and Linux.

The script receive a list of directories and search source files in all sub folders.

Options:
    -d <coma separated list of directories>                 To specify what are the directory to search for source file to format.          Default="." (currrent directory)
    Ex: -d "dir1,dir2,dir3"
    
    -b <clang-format binary>                                To specify the path and filename to the clang-format-executable                 Default="./clang-format.exe"
    Ex: -b "./some_dir/clang-format"
    
    -e <coma separated list of files extension to format>   To specify the file to format                                                   Default=".c,.h,.cpp"
    Ex: -e ".c,.h,.cpp,.js"
'''
import os
import subprocess
import shlex
import sys
import getopt
import errno

MAX_FILES_PER_COMMAND = 100;

def main(argv):
    
    #DEFAULT OPTIONS VALUES
    clangBinary = './clang-format.exe'
    dirs = ('.')
    includes = ('.c', '.h', '.cpp')
    
    optlist, args = getopt.getopt(argv, 'd:b:e:')
    
    for opt, arg in optlist:
        if opt in ('-d'):
            dirs = arg.split(',')
        elif opt in ('-b'):
            clangBinary=arg
        elif opt in ('-e'):
            includes = tuple(arg.split(','))
    
    for dir in dirs:
        formatRootDir(dir, clangBinary, includes)            
            
def formatRootDir(directory, clangBinary, includes):
    #Format files in current directory
    formatDir(directory, clangBinary, includes)
    
    #Format subdirectories
    for root, dirnames, filenames in os.walk(directory):
        # Format subdirectories
        for subdirname in dirnames:
            formatDir(os.path.join(root, subdirname), clangBinary, includes)
            
def formatDir(directory, clangBinary, includes):
    count = 0;
    files = '';
    for file in os.listdir(directory):
        if file.endswith(includes):
            if(count>0):
                files += ';'
            files += os.path.join(directory, file)
            count = count + 1
            if(count >= MAX_FILES_PER_COMMAND):
                formatFile(files, clangBinary)
                files=''
                count = 0
                
    if(count > 0):
        formatFile(files, clangBinary)
        files=''
        count = 0;
                   
def die(message):
  print('error:', message, file=sys.stderr)
  sys.exit(2)
  
def formatFile(files, clangBinary):
    fileList = files.split(';')
    for file in fileList:
        print('Formatting "%s"' % file)
    try:
        #print 'Executing clang-format command: %s' % (([clangBinary, '-i'] + fileList))
        clang_format = subprocess.Popen([clangBinary, '-i'] + fileList, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        for line in clang_format.stdout.readlines():
            print(line)
    except OSError as e:
        if e.errno == errno.ENOENT:
            die('Cannot find executable "%s"' % clangBinary)
        else:
            raise
    clang_format.stdin.close()
    clang_format.stdout.close()
  
if __name__ == '__main__':
  main(sys.argv[1:])