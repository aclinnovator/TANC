require 'mkmf'

LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']

HEADER_DIRS = [INCLUDEDIR]

LIB_DIRS = [LIBDIR]

dir_config('TCPathfind', HEADER_DIRS, LIB_DIRS)

unless find_header('TCPathfind/tcpathfind.h')
  abort "libpolarssl is missing. please install libpolarssl"
end

create_makefile('TCPathfind/Makefile')
