/*
	Kai Lüke <kailueke@riseup.net>: changed to modify what
	                      you want instead of just machine

	Use the provided Makefile to simply run
	  make KRELEASE=2.5.2-0 KSYSNAME=Minix		\
	   KVERSION="Debian 2.5 (2012-03-04)" KMACHINE=i386 test
	and see what happens by using insmod to load the module.
	For using it longer, try
	  make KMACHINE=i386 insertunamemodule
	until you are done and can revert via
	  make removeunamemodule

	If you are confused by temp files, do "make clean"

	Specification of the modified struct:
	struct new_utsname {
	        char sysname[__NEW_UTS_LEN + 1];
	        char nodename[__NEW_UTS_LEN + 1];
	        char release[__NEW_UTS_LEN + 1];
	        char version[__NEW_UTS_LEN + 1];
	        char machine[__NEW_UTS_LEN + 1];
	        char domainname[__NEW_UTS_LEN + 1];
	};

	Former changelog:

	This simple piece of code simply turns your ix86 into an i386 -
	useful if you're cross-compiling for a weaker platform.

	Based on the program contained in the cross compiling hint by
	Daniel Baumann <danielbaumann@linuxmail.org>

	Originally Updated to 2.6.x series kernel by Roel Neefs.

	Updated to work with the 2.6.x series kernel driver standards
	by Jim Gifford.

	Updated to work with 2.6.16 and greater by Joe Ciccone.

	Updated to use i386 for IA32 compatibility, required to 
	eliminate cpuid

	You will need to create a small Makefile for this module to 
	work.

	cat > Makefile << "EOF"
	obj-m += uname_hack.o
	EOF

	To compile as a module use the following command:

	make -C /usr/src/linux-{version} SUBDIRS=$PWD

	As stated on http://sourceforge.net/projects/uname-hack/
	this work is under
	GNU General Public License version 2.0 (GPLv2)
	see: http://www.gnu.org/licenses/gpl-2.0-standalone.html
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/utsname.h>
#include <linux/version.h>


#if LINUX_VERSION_CODE >= 132630
# define uname_uts_attrib(attrib) utsname()->attrib
#else
# define uname_uts_attrib(attrib) system_utsname.attrib
#endif


#ifdef hack_sysname
static char uname_save_sysname[sizeof(uname_uts_attrib(sysname))];
#endif
#ifdef hack_nodename
static char uname_save_nodename[sizeof(uname_uts_attrib(nodename))];
#endif
#ifdef hack_release
static char uname_save_release[sizeof(uname_uts_attrib(release))];
#endif
#ifdef hack_version
static char uname_save_version[sizeof(uname_uts_attrib(version))];
#endif
#ifdef hack_machine
static char uname_save_machine[sizeof(uname_uts_attrib(machine))];
#endif
#ifdef hack_domainname
static char uname_save_domainname[sizeof(uname_uts_attrib(domainname))];
#endif

#define savename(attrib) uname_save_ ## attrib

#define set(attrib, value) memcpy(savename(attrib), uname_uts_attrib(attrib), sizeof(uname_uts_attrib(attrib))); \
	memset(uname_uts_attrib(attrib), sizeof(uname_uts_attrib(attrib)), '\0'); \
	memcpy(uname_uts_attrib(attrib), value, sizeof(value));

#define unset(attrib) memcpy(uname_uts_attrib(attrib), savename(attrib), sizeof(uname_uts_attrib(attrib)));

static int __init uname_hack_init_module( void )
{
#ifdef hack_sysname
	set(sysname, hack_sysname)
#endif
#ifdef hack_nodename
	set(nodename, hack_nodename)
#endif
#ifdef hack_release
	set(release, hack_release)
#endif
#ifdef hack_version
	set(version, hack_version)
#endif
#ifdef hack_machine
	set(machine, hack_machine)
#endif
#ifdef hack_domainname
	set(domainname, hack_domainname)
#endif
	return (0);
}

static void __exit uname_hack_cleanup_module( void )
{
#ifdef hack_sysname
	unset(sysname)
#endif
#ifdef hack_nodename
	unset(nodename)
#endif
#ifdef hack_release
	unset(release)
#endif
#ifdef hack_version
	unset(version)
#endif
#ifdef hack_machine
	unset(machine)
#endif
#ifdef hack_domainname
	unset(domainname)
#endif
}

module_init(uname_hack_init_module);
module_exit(uname_hack_cleanup_module);

