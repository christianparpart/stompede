#pragma once
/* <stompede/sysconfig.h.cmake>
 *
 * This file is part of the x0 web server project and is released under LGPL-3.
 *
 * (c) 2013 Chrisitan Parpart <trapni@gmail.com>
 */

#cmakedefine HAVE_SYS_INOTIFY_H
#cmakedefine HAVE_SYS_SENDFILE_H
#cmakedefine HAVE_SYS_RESOURCE_H
#cmakedefine HAVE_SYS_LIMITS_H
#cmakedefine HAVE_SYS_MMAN_H
#cmakedefine HAVE_SYSLOG_H
#cmakedefine HAVE_PWD_H

#cmakedefine HAVE_NETDB_H
#cmakedefine HAVE_AIO_H
#cmakedefine HAVE_LIBAIO_H
#cmakedefine HAVE_ZLIB_H
#cmakedefine HAVE_BZLIB_H
#cmakedefine HAVE_GNUTLS_H
#cmakedefine HAVE_LUA_H
#cmakedefine HAVE_PCRE_H
#cmakedefine HAVE_SYS_UTSNAME_H

#cmakedefine HAVE_LDAP_H

#cmakedefine WITH_SSL
#cmakedefine WITH_ACCEPT4
#cmakedefine WITH_TCP_DEFER_ACCEPT

#cmakedefine HAVE_INOTIFY_CREATE1
#cmakedefine HAVE_FORK
#cmakedefine HAVE_CHROOT
#cmakedefine HAVE_PATHCONF
#cmakedefine HAVE_SENDFILE
#cmakedefine HAVE_POSIX_FADVISE
#cmakedefine HAVE_READAHEAD
#cmakedefine HAVE_ACCEPT4

#cmakedefine SYSCONFDIR "@SYSCONFDIR@"
#cmakedefine PLUGINDIR "@PLUGINDIR@"
