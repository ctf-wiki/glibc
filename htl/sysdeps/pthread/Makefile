ifeq ($(subdir),rt)
librt-sysdep_routines += timer_routines
endif
ifeq ($(subdir),posix)
  # FIXME: this is not getting $(pthread-version) from libpthread/Makefile!
CFLAGS-confstr.c += -DLIBPTHREAD_VERSION='"libpthread $(pthread-version)"'
endif
