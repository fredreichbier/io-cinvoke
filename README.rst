CInvoke for Io
==============

That is a C/Invoke addon for Io, based on Trevor Fancher's CFFI addon.

It does support (in theory):

 * dynamic library loading on all systems supported by C/Invoke
 * dynamic function calling
 * C structure access 
 * Io callback functions

Its dependencies:
 * C/Invoke >= 1.0 (http://www.nongnu.org/cinvoke/)
 * Io, tested with latest git (around October 2008)

CInvoke is licensed under the revised BSD license.

Unfortunately, CInvoke for Io is only tested with the included samples. 
A test gtk binding does not work. So, it is probably not usable for everyone
yet, but hopefully in the future.
Have fun. Or anything else.

