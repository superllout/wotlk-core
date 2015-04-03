# EasyWoW/wotlk-core

Core wiki: https://github.com/EasyWoW/wiki/wiki

## What is EasyWoW

EasyWoW is World of Warcraft server engine based on arcemu (http://github.com/arcemu/arcemu, http://www.arcemu.org). This fork was made due original development has been ceased (they call it hibernation) and by wiling to 
add new features. 
This framework is ready to use under all currently popular operation systems - UNIX, OS X, Linux, Windows.
Its free to use, fork or use core commits (with authority information) and do any other things which do not breaks GNU GPL v3 license.

Main goals for this core is simplicity, performance and functionality - we and our contributors works very hard to combine all of them into modern code.



## Required tools to run this server

* Game client (this repository supports only 3.3.5a 12340 build).

* Compiler:

    Linux/UNIX: GCC ( 4.7 or later) and Clang ( 3.6 or later)

    Windows: Visual Studio (Community 2013 or later), mingw currently has no support.

* data server (Mysql 5.4 or later, PostgreSQL will supported in near future).

* Git (source control)

* computer with 2-3 GB RAM minimally and also which CPU supports SSE2 technology.


## Is having WoW (World of Warcraft) server illegal?

Yes, It is because server core just manipulates and uses game content which was gathered from official this game servers and game client and also it breaks some WoW authorised EULA license paragraphs. 

## Is having core sources and using compiled binaries for personal (not for public) breaks any rules?

No, since Its open source.

## How to contribute

### For simple user:

* Report any issues with proper information (last used core commit has, compiler, platform, mysql server version, add-ons, which script engines are enabled and similar.).

* Add information how that functionality should work (for example: creature should cast XX spell XXX times) and how this works in core currently.

* Check your information before submitting.

* By testing new patches (major implementation will published in branches).

* Helping to keep it well documented.

* By making guides.


### For developer:

* Improve performance where is possible (major).

* Do not make pull requests with fun content to core repository.

* Make code understandable, do not use just letters or non understandable, long variable titles, use spaces instead of tabs and try to use much latest supportable (currently c11/c++11) standards, add comments which could
 help to understand code easer


