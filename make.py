#!/usr/bin/env python
# coding=utf-8
# PYTHON_ARGCOMPLETE_OK


from datetime import datetime
import re
import platform
import sys
import shlex
import shutil
import subprocess
import pty
import os
import glob
import errno
import logging


__author__ = 'andrew@azaporozhtsev.grad.rnd'
__date__ = "26.03.2019"
__license__ = "GPL"
__version__ = "0.1.2"

EXEC_FORMAT = '====\n  {{{0}}} # {1} {2} \n===='
PRO_FILE = '_.pro'
PROJ_LIST = []


def main():
    args = init_args()

    mode = logging.INFO

    # if is_python3():
    #    args['verbose'] = True

    if args.get('verbose'):
        mode = logging.DEBUG

    log = init_log(mode)
    build_dir = os.path.realpath(args.get('build_dir'))

    log.warning("BUILD DIRECTORY: {0}".format(build_dir))

    project = QtProBuilder(PRO_FILE, log, build_dir=build_dir, pro_list=PROJ_LIST)

    project.init_build_dir()

    if isinstance(args.get('delete'), list):
        if not args.get('delete'):
            project.delete_build_dir()
        else:
            for pro_name in args.get('delete'):
                project.delete_build_dir(pro_name)

    if isinstance(args.get('clean'), list):
        if not args.get('clean'):
            project.clean()
        else:
            for pro_name in args.get('clean'):
                project.clean(pro_name)


    if isinstance(args.get('defines'), list):
        if not args.get('qmake_param'):
            args['qmake_param'] = []

        for param in args.get('defines'):
            args['qmake_param'].append("DEFINES+=\"" + param + "\"")


    if isinstance(args.get('build'), list):
        if not args.get('build'):
            if not project.build(args.get('qmake'), verbose=args.get('verbose'), conf_debug=args.get('release'), 
                             qmake_param=args.get('qmake_param'), custom_make=args.get('custom_make')):
                sys.exit(1)
        else:
            pro_list = args.get('build')
            start_time = datetime.now()
            for i, pro_name in enumerate(pro_list, 1):
                delta = datetime.now() - start_time
                share = i / float(len(pro_list))
                eta = (1 - share) * delta.seconds
                if not project.build(args.get('qmake'), pro_name, verbose=args.get('verbose'), conf_debug=args.get('release'),
                                 qmake_param=args.get('qmake_param'), custom_make=args.get('custom_make'), 
                                 progress='({0}/{1}, {2}%, {3}, ETA={4})'.format(i, len(pro_list), round(share * 100, 2),
                                            '[{0}:{1} sec]'.format(delta.seconds, str(delta.microseconds)[4:]),
                                            "{0:.2f} sec".format(eta))):
                    sys.exit(1)

    if isinstance(args.get('build_fast'), list):
        if not args.get('build_fast'):
            if not project.build_fast():
                sys.exit(1)
        else:
            for pro_name in args.get('build_fast'):
                if not project.build_fast(pro_name):
                    sys.exit(1)

    if isinstance(args.get('install'), list):
        if not args.get('install'):
            project.install()
        else:
            for pro_name in args.get('install'):
                project.install(pro_name)

    if isinstance(args.get('uninstall'), list):
        if not args.get('uninstall'):
            project.uninstall()
        else:
            for pro_name in args.get('uninstall'):
                project.uninstall(pro_name)


def init_args():
    """
    Gets dictionary arguments of command line
    :return: dict args
    """
    from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter
    description = 'qt project\'s builder'
    epilog = '''(c) Andrew 2019. Copyright and Related Rights Regulations (2019 No. 3)'''
    parser = ArgumentParser(formatter_class=ArgumentDefaultsHelpFormatter, description=description, epilog=epilog)

    global PRO_FILE
    PRO_FILE = glob.glob('*.pro')
    if PRO_FILE and os.path.exists(os.path.realpath(PRO_FILE[0])):
        PRO_FILE = os.path.realpath(PRO_FILE[0])
        print("Found project file: " + PRO_FILE)
    global PROJ_LIST
    PROJ_LIST = list(set(QtProBuilder.get_list_subprojects(os.path.join(os.path.abspath(os.curdir), PRO_FILE))))
    proj_list = [i[0] for i in PROJ_LIST]

    build_dir_pro = os.path.abspath(os.path.join('..', 'build_' + os.path.basename(os.path.realpath('.'))))

    parser.add_argument('--build-dir', dest='build_dir', default=build_dir_pro , help='show the process in details')
    parser.add_argument('--add', dest='qmake_param', nargs='*', help='additional qmake parameters')
    parser.add_argument('--release', dest='release', action='store_false', help='configure the qt project as release')
    parser.add_argument('--defines', dest='defines', nargs='*', help='build the qt project')
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_true', help='show the process in details')
    parser.add_argument('-b', dest='build', nargs='*', choices=proj_list, help='build the qt project')
    parser.add_argument('-bf', dest='build_fast', nargs='*', choices=proj_list, help='build the qt project')
    parser.add_argument('-i', dest='install', nargs='*', choices=proj_list,
                        help='install the qt project on system')
    parser.add_argument('-u', dest='uninstall', nargs='*', choices=proj_list,
                        help='uninstall the qt project on system')
    parser.add_argument('-c', dest='clean', nargs='*', choices=proj_list, help='clean the qt project')
    parser.add_argument('-r', '--rebuild', dest='rebuild', nargs='*', choices=proj_list, help='rebuild the qt project')
    parser.add_argument('-d', '--delete-before', dest='delete', nargs='*', choices=proj_list, help='remove build directory')

    def get_qmake_path():
        if is_python2():
            return str(subprocess.Popen('which qmake-qt4', shell=True, stdout=subprocess.PIPE).stdout.read().strip())
        else:
            return subprocess.Popen('which qmake-qt4', shell=True, stdout=subprocess.PIPE).stdout.read().strip().decode()

    parser.add_argument('--qmake_path', dest='qmake', default=get_qmake_path(), help='set qmake path')
    parser.add_argument('--custom-make', dest='custom_make', default='', help='set target make command')

    args = sys.argv[1:]
    if not args:
        args = shlex.split('-c -b -i')

    args = parser.parse_args(args)
    args = vars(args)

    if args and args['clean'] is None and args['build'] is None and args['install'] is None:
        args['clean'] = args['build'] = args['install'] = []
        print('args were changed by default')

    if args.get('rebuild'):
        args['clean'] = args.get('rebuild')
        args['build'] = args.get('rebuild')
        args['install'] = args.get('rebuild')
    return args


def init_log(mode):
    log = logging.Logger('qt_builder')

    file_handler = logging.FileHandler(os.path.join(os.getcwd(), 'log.txt'), encoding=sys.getfilesystemencoding(),
                                       mode='w')
    # file_handler.setFormatter(logging.Formatter(u'%(asctime)-15s  [FILE: %(filename)s LINE:%(lineno)d] <%(levelname)s> %(message)s'))
    file_handler.setFormatter(logging.Formatter(u'%(message)s'))
    file_handler.setLevel(logging.DEBUG)
    log.addHandler(file_handler)

    console_handler = logging.StreamHandler(sys.stdout)
    console_handler.setFormatter(logging.Formatter(u'%(message)s'))
    console_handler.setLevel(mode)
    log.addHandler(console_handler)

    print('LOG mode console = ' + logging.getLevelName(console_handler.level))
    print('LOG mode file_handler = ' + logging.getLevelName(file_handler.level))
    return log


class QtProBuilder(object):
    def __init__(self, pro, log, build_dir, pro_list):
        self.__log = log
        self.__count_log = 0
        self.__build_dir = build_dir
        self.__find_pro = re.compile(r'^cd src/(?P<pro>[a-z0-9-_]+)/ && .*make -f Makefile')
        self.__pro = pro
        self.__num_pro = 0
        self.__start_time = datetime.now()
        self.__amount_pro = len(pro_list)
        

    def make(self, qmake):
        # smart building
        self.clean()
        self.build(qmake)
        self.install()

    def build_fast(self, project_name=''):
        build_dir, project_name = self._set_project_name(project_name)

        self.__log.info(Colors.underline + Colors.bold +'building ' +
                         Colors.fg.lightgreen + Colors.bg.black + '{0} ...'.format(project_name) + Colors.reset)
        
        if run_inter(self.__log, '/usr/bin/make -j {0}'.format(self.cpu_count()), path=build_dir,
                     cwd=build_dir, repl=self.__building):
            self.__log.info('OK')
            return True
        self.__log.critical(Colors.underline + 'the building has ' + Colors.fg.red + 'failed' + Colors.reset)
        return False

    def build(self, qmake, project_name='', verbose=None, conf_debug=True, qmake_param='', custom_make='', progress=''):
        build_dir, project_name = self._set_project_name(project_name)

        self.__log.info(Colors.bold + 'building ' + Colors.underline + Colors.fg.lightgreen + Colors.bg.black + \
                        '{0}{1}\t{2}'.format(project_name, Colors.reset, progress))

        cmd = '{0} {1} -r '.format(qmake, project_name)

        if conf_debug:
            cmd += 'CONFIG+=debug'
        if isinstance(qmake_param, list):
            params = ''
            for param in qmake_param:
                params += ' ' + param
            cmd += params
            print('PARAMS = ' + params)

        if run_inter(self.__log, cmd, path=build_dir, cwd=build_dir):
            if run_inter(self.__log, '/usr/bin/make -j{0} {1}'.format(self.cpu_count(), custom_make), 
                         path=build_dir, cwd=build_dir, repl=self.__building, verbose=verbose):
                if not verbose:
                    self.__log.info('')
                self.__log.info('OK')
                return True
        self.__log.critical(Colors.underline + 'the building has ' + Colors.fg.red + 'failed' + Colors.reset)
        return False

    def clean(self, project_name=''):
        build_dir, project_name = self._set_project_name(project_name)
        self.__log.info(Colors.bold + Colors.underline + 'cleaning {0} ...'.format(build_dir) + Colors.reset)
        run_inter(self.__log, '/usr/bin/make clean', path=build_dir, cwd=build_dir)

    def install(self, project_name=''):
        build_dir, project_name = self._set_project_name(project_name)

        self.__log.info(Colors.bold + Colors.underline + 'installing {0} ...'.format(build_dir) + Colors.reset)
        if not run_inter(self.__log, '/usr/bin/sudo /usr/bin/make install', path=build_dir,
                         cwd=build_dir):
            self.__log.critical(Colors.underline + 'the installing has ' + Colors.fg.red + 'failed' + Colors.reset)
            return False
        self.__log.info('OK')
        return True

    def uninstall(self, project_name=''):
        build_dir, project_name = self._set_project_name(project_name)

        self.__log.info(Colors.bold + Colors.underline + 'uninstalling {0} ...'.format(build_dir) + Colors.reset)
        if not run_inter(self.__log, '/usr/bin/sudo /usr/bin/make uninstall -j{0}'.format(self.cpu_count()),
                         path=build_dir,
                         cwd=build_dir):
            self.__log.critical(Colors.underline + 'the uninstalling has ' + Colors.fg.red + 'failed' + Colors.reset)
            return False
        self.__log.info('OK')
        return True

    def _set_project_name(self, project_name):
        if project_name:
            d_pro = dict(PROJ_LIST)
            dir_pro = d_pro.get(project_name, '')
            build_dir = os.path.join(self.__build_dir, dir_pro, os.path.basename(project_name))
            project_dir = os.path.join(os.path.abspath('.'), dir_pro, project_name)
            project_name = glob.glob(os.path.join(project_dir, '*.pro'))[0]
            if not os.path.exists(build_dir):
                os.makedirs(build_dir)
        else:
            project_name = self.__pro
            build_dir = self.__build_dir
        return build_dir, project_name

    def __building(self, buffer):
        res = self.__find_pro.match(buffer)
        if res:
            delta = datetime.now() - self.__start_time
            self.__num_pro += 1
            share = self.__num_pro / float(self.__amount_pro)
            eta = (1 - share) * delta.seconds
            return os.linesep + Colors.bold + Colors.fg.green + '>>> {0} '.format(res.group('pro')) + Colors.reset + \
                '({0}/{1}, {2}%, {3}, ETA={4})'.format(self.__num_pro, self.__amount_pro, round(share * 100, 2),
                                            '[{0}:{1} sec]'.format(delta.seconds, str(delta.microseconds)[4:]),
                                            "{0:.2f} sec".format(eta))

        self.__log.debug(buffer)
        self.__count_log = self._show_progress(self.__count_log)

    def delete_build_dir(self, project_name=''):
        build_dir, project_name = self._set_project_name(project_name)
        self.__log.info(
            Colors.bold + Colors.underline + 'removing build dir {0} ...'.format(build_dir) + Colors.reset)
        shutil.rmtree(build_dir, True)
        self.init_build_dir()

    def init_build_dir(self):
        try:
            if not os.path.exists(self.__build_dir):
                os.makedirs(self.__build_dir)
        except OSError as e:
            if e.errno == errno.EEXIST and os.path.isdir(self.__build_dir):
                pass
            else:
                raise OSError

    @staticmethod
    def get_list_subprojects(top_pro_file):
        subpro_str = r'\n\s*([a-z0-9-_/]+)[/]([a-z0-9-_]+)'
        r_sub_proj = re.compile(subpro_str)
        if os.path.exists(top_pro_file):
            text = open(top_pro_file, 'rt').read()

            # if there is a composited project
            if re.search(r'TEMPLATE\s*=\s*subdirs', text):
                m = re.search(r'SUBDIRS\s+[+][=].*([a-z0-9-_/]+)([a-z0-9-_]+)', text, re.DOTALL)
                if m:
                    text = m.group(0)
                    l = r_sub_proj.findall(text)
                    res = []
                    for dir, name_pro in l:
                        if not find(name_pro + '.pro', os.path.join(os.path.abspath(os.curdir), dir)):
                            return []
                        res.append((name_pro, dir))
                    return res
            return []
        raise ValueError('It hasn\'t found the project "{0}"'.format(top_pro_file))

    @staticmethod
    def _show_progress(counter, width=80):
        if counter >= width:
            counter = 0
            # sys.stdout.write('\r' + ' ' * width + '\r')
            # sys.stdout.flush()
        sys.stdout.write('*')
        sys.stdout.flush()
        counter += 1
        return counter

    @staticmethod
    def cpu_count():
        try:
            import multiprocessing
            return multiprocessing.cpu_count()
        except (ImportError, NotImplementedError):
            return 1


def is_on_linux():
    return re.match('linux', platform.system(), re.IGNORECASE)


def is_on_windows():
    return re.match('windows', platform.system(), re.IGNORECASE)


def run_inter(log, cmd, input_list=None, path=None, cwd='.', repl=None, verbose=False):
    if path is None:
        if is_on_linux():
            path = os.getenv("HOME")
        elif is_on_windows():
            path = os.getenv("USERPROFILE")
    curwd = os.getcwd()
    os.chdir(cwd)

    if not input_list:
        input_list = list()

    log.debug(EXEC_FORMAT.format(path, cmd, input_list if input_list else ''))
    cmd = os.path.join(path, cmd)
    if verbose:
        process = subprocess.Popen(cmd, shell=True)

    else:
        process = subprocess.Popen(cmd, shell=True, universal_newlines=True, stdout=subprocess.PIPE,
                                   stderr=subprocess.PIPE, stdin=subprocess.PIPE, bufsize=1)

    if input_list:
        process.stdin.write(os.linesep.join(input_list))

    if not verbose:
        while process.poll() is None:
            if not process.stdout:
                break
            process.stdout.flush()
            output = process.stdout.readline()
            if not output:
                break

            # output = os.linesep.join(output)
            # output = output.decode(sys.getfilesystemencoding())
            output = output.rstrip()

            if output and repl:
                output = repl(output)
            if output:
                log.info(output)

    status = process.wait() == 0
    os.chdir(curwd)

    if is_on_linux() and not status and process.stderr:
        def repl_error_func(m):
            key_line = m.group('line')
            key_error = m.group('error')
            key_file_name = m.group('file')
            return '{0}{1}{2}'.format('{0}: '.format(
                Colors.underline + Colors.reverse + key_file_name + Colors.reset) if key_file_name else '',
                                      '[{0}]'.format(
                                          Colors.underline + Colors.bold + key_line + Colors.reset) if key_line else '',
                                      ' - {0}'.format(
                                          Colors.bold + Colors.fg.red + key_error.upper() + Colors.reset) if key_error else '')

        re_error = re.compile(r'(?:(?P<file>\w+[.]\w+):(?P<line>(?:[0-9]+:)?[0-9]+):)? (?P<error>error:?\s)', re.I)

        for line in process.stderr.readlines():
            line = re_error.sub(repl=repl_error_func, string=line)
            if is_python2():
                log.critical(line.decode(sys.getfilesystemencoding()).rstrip())
            elif is_python3():
                log.critical(line.rstrip())

    return status


class Colors:
    reset = '\033[0m'
    bold = '\033[01m'
    disable = '\033[02m'
    underline = '\033[04m'
    reverse = '\033[07m'
    strikethrough = '\033[09m'
    invisible = '\033[08m'

    class fg:
        black = '\033[30m'
        red = '\033[31m'
        green = '\033[32m'
        orange = '\033[33m'
        blue = '\033[34m'
        purple = '\033[35m'
        cyan = '\033[36m'
        lightgrey = '\033[37m'
        darkgrey = '\033[90m'
        lightred = '\033[91m'
        lightgreen = '\033[92m'
        yellow = '\033[93m'
        lightblue = '\033[94m'
        pink = '\033[95m'
        lightcyan = '\033[96m'

    class bg:
        black = '\033[40m'
        red = '\033[41m'
        green = '\033[42m'
        orange = '\033[43m'
        blue = '\033[44m'
        purple = '\033[45m'
        cyan = '\033[46m'
        lightgrey = '\033[47m'


def find(pattern, path):
    if path and pattern:
        p = re.compile(pattern)
        for root, dirs, files in os.walk(path):
            for name in files:
                if p.match(name):
                    return root, name


def is_python2():
    return sys.version[0] == '2'


def is_python3():
    return sys.version[0] == '3'
    

if __name__ == '__main__':
    main()
