opts = Options()
opts.Add('DESTDIR', 'Directory to install', '/')
opts.Add('DEBUG', 'Set DEBUG=yes for building with enabled debuging', 'no')
opts.Add('NLS_SUPPORT', 'Set NLS_SUPPORT=no for building with disable gettext i18n', 'yes')
opts.Add('LIBDIR', 'Set LIBDIR=/lib/dir/path for change default instalation path', '/usr/lib')
opts.Add('LIBSYSDIR', 'Set LIBSYSDIR=/lib/system/dir/path for change default instalation path', '/lib')
env = Environment(options=opts)
Help(opts.GenerateHelpText(env))

LIBDEVSUFFIX = ".so"

NSS_NAME = "libnss_role"
NSS_LIBBASEVERSION = "2"
NSS_LIBSUBVERSION = "0.0"
NSS_LIBFULLVERSION = NSS_LIBBASEVERSION + '.' + NSS_LIBSUBVERSION
NSS_LIBBASESUFFIX = LIBDEVSUFFIX + '.' + NSS_LIBBASEVERSION
NSS_LIBFULLSUFFIX = LIBDEVSUFFIX + '.' + NSS_LIBFULLVERSION
NSS_SONAME = NSS_NAME + NSS_LIBBASESUFFIX
NSS_FULLNAME = NSS_NAME + NSS_LIBFULLSUFFIX

COMMON_NAME = "librole"
COMMON_LIBBASEVERSION = "0"
COMMON_LIBSUBVERSION = "0.0"
COMMON_LIBFULLVERSION = COMMON_LIBBASEVERSION + '.' + COMMON_LIBSUBVERSION
COMMON_LIBBASESUFFIX = LIBDEVSUFFIX + '.' + COMMON_LIBBASEVERSION
COMMON_LIBFULLSUFFIX = LIBDEVSUFFIX + '.' + COMMON_LIBFULLVERSION
COMMON_SONAME = COMMON_NAME + COMMON_LIBBASESUFFIX
COMMON_FULLNAME = COMMON_NAME + COMMON_LIBFULLSUFFIX
COMMON_DEVNAME = COMMON_NAME + LIBDEVSUFFIX

env['CCFLAGS'] = ['-O2', '-Iinclude', '-DPACKAGE=\\"nss_role\\"']
if 'DEBUG' in ARGUMENTS and ARGUMENTS['DEBUG'] == 'yes':
    env['CCFLAGS'] += ['-DDEBUG']
if 'NLS_SUPPORT' not in ARGUMENTS or ARGUMENTS['NLS_SUPPORT'] != 'no':
    env['CCFLAGS'] += ['-DENABLE_NLS', '-DLOCALEDIR=\\"/usr/share/locale\\"']

libenv = env.Clone()
libenv["SHLIBSUFFIX"] = [NSS_LIBFULLSUFFIX]
libenv["LINKFLAGS"] = ['-Wl,-soname,' + NSS_SONAME]
parser = libenv.SharedObject('RoleParserSimple', 'RoleParserSimple.cpp')
so = libenv.SharedLibrary(NSS_NAME, ['nss_role.cpp', parser])
solink = libenv.Command(NSS_SONAME, so[0], 'ln -sf %s %s' % (NSS_FULLNAME, NSS_SONAME))

commonenv = libenv.Clone()
commonenv["SHLIBSUFFIX"] = [COMMON_LIBFULLSUFFIX]
commonenv["LINKFLAGS"] = ['-Wl,-soname,' + COMMON_SONAME]
commonenv["LIBS"] = ['boost_iostreams', 'pam', 'pam_misc']
commonfiles = ['PamCheck.cpp', 'GetText.cpp', 'LockFile.cpp', 'RoleCommon.cpp', 'RoleManager.cpp', parser, 'UserReader.cpp', 'GroupReader.cpp', 'RoleParser.cpp', 'RoleStorage.cpp']
common = commonenv.SharedLibrary(COMMON_NAME, commonfiles)
commonlink = commonenv.Command(COMMON_SONAME, common[0], 'ln -sf %s %s' % (COMMON_FULLNAME, COMMON_SONAME))
commondevlink = commonenv.Command(COMMON_DEVNAME, common[0], 'ln -sf %s %s' % (COMMON_FULLNAME, COMMON_DEVNAME))
commonheaders = Glob('include/Role/*.h')

utilenv = env.Clone()
utilenv["LIBS"] = ['role','boost_program_options']
utilenv["LIBPATH"] = '.'

roleadd = utilenv.Program('roleadd', 'roleadd.cpp')
roledel = utilenv.Program('roledel', 'roledel.cpp')
rolelst = utilenv.Program('rolelst', 'rolelst.cpp')

commonenv.Install('$DESTDIR/$LIBDIR/', common)
commonenv.Install('$DESTDIR/usr/include/Role', commonheaders)
i = commonenv.Command('$DESTDIR/$LIBDIR/' + COMMON_SONAME, commonlink[0], 'cp -P %s /$DESTDIR/$LIBDIR/%s' % (COMMON_SONAME, COMMON_SONAME))
commonenv.Alias('install', i)
i = commonenv.Command('$DESTDIR/$LIBDIR/' + COMMON_DEVNAME, commondevlink[0], 'cp -P %s /$DESTDIR/$LIBDIR/%s' % (COMMON_DEVNAME, COMMON_DEVNAME))
commonenv.Alias('install', i)

utilenv.Install('$DESTDIR/usr/sbin', [roleadd, roledel])
utilenv.Install('$DESTDIR/usr/bin', [rolelst])
utilenv.InstallAs('$DESTDIR/etc/pam.d/roleadd', 'role.pamd')
utilenv.InstallAs('$DESTDIR/etc/pam.d/roledel', 'role.pamd')
utilenv.Install('$DESTDIR/usr/share/man/man8', ['roleadd.8', 'roledel.8', 'rolelst.8'])

libenv.Install('$DESTDIR/$LIBSYSDIR', so)
i = libenv.Command('$DESTDIR/$LIBSYSDIR/' + NSS_SONAME, solink[0], 'cp -P %s /$DESTDIR/$LIBSYSDIR/%s' % (NSS_SONAME, NSS_SONAME))
libenv.Alias('install', i)

Alias('install', FindInstalledFiles())
