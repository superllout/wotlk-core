# EasyWoW/wotlk-core 

[![Build Status](https://travis-ci.org/EasyWoW/wotlk-core.svg?branch=master)](https://travis-ci.org/EasyWoW/wotlk-core)

Core wiki: https://github.com/EasyWoW/wiki/wiki

## What is EasyWoW

EasyWoW is World of Warcraft server engine based on arcemu (http://github.com/arcemu/arcemu, http://www.arcemu.org). This fork was made due original development has been ceased (they call it hibernation) and by wiling to 
add new features. 
This framework is ready to use under all currently popular operation systems - UNIX, OS X, Linux, Windows.
Its free to use, fork or use core commits (with authority information) and do any other things which do not breaks GNU AFFERO GENERAL PUBLIC LICENSE Version 3 license.

Main goals for this core is simplicity, performance and functionality - we and our contributors work very hard to combine all of them into modern code. We, EasyWoW aim as friendly community, so feel free to give 
questions or documentation requests (NOTE: do not forget to add correct label - "Question" or "Suggestion for wiki" ).



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

No, since Its open source and you use it only for personal. Personal means - you do not use server binaries for public connections (where possible players could connect to your server). 
We do not support any public live servers.

## How to contribute


### Generally:

* Respect current developers, contributors - they do hard work and shares it for free.

* Use English - we want hear you but we may won't understand your custom language.

* We do not promise anything, when its done - its done.

* Do not impale community with any products, advertisements, commercial activity or similar - do it personally, this community was not created for that purpose.


### For simple user:

* Report any issues with proper information (last used core commit hash, git branch, compiler, platform, mysql server version, add-ons, which script engines are enabled and similar.).

* Add information when creating issue how that functionality should work (for example: creature should cast XX spell XXX times) and how this works in core currently.

* Check your information and for duplicate issues before submitting.

* By testing new patches (major implementation will published in branches) and reporting implementation issues.

* Helping to keep it well documented.

* By making guides.


### For developer:

* Improve performance and compatibility where It is possible (major).

* Make pull requests with code improvements. Do not make pull requests with fun content to core repository.

* Make code understandable, do not use just letters or non understandable, long variable titles, use spaces instead of tabs and try to use much latest supportable (currently c11/c++11) standards, add comments which could
 help to understand code easer

* Implement features, script missing contents, improve functionality where is possible.


