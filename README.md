# EasyWoW/wotlk-core 

[![Build Status](https://travis-ci.org/EasyWoW/wotlk-core.svg?branch=master)](https://travis-ci.org/EasyWoW/wotlk-core) [![Coverity Scan](https://scan.coverity.com/projects/4872/badge.svg)](https://scan.coverity.com/projects/4872)

[![Issue Stats](http://www.issuestats.com/github/EasyWoW/wotlk-core/badge/pr)](http://www.issuestats.com/github/EasyWoW/wotlk-core) [![Issue Stats](http://www.issuestats.com/github/EasyWoW/wotlk-core/badge/issue)](http://www.issuestats.com/github/EasyWoW/wotlk-core)


[![License](https://www.gnu.org/graphics/agplv3-155x51.png "project is licensed under AGPL")](https://www.gnu.org/licenses/agpl-3.0.html)

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

    Linux/UNIX: GCC ( 4.6 or later) and Clang ( 3.2 or later)

    Windows: Visual Studio (Community 2013 or later), mingw currently has no support.

* data server (MySQL 5.4 or later, PostgreSQL will supported in near future).

* Git (source control)

* computer with 2-3 GB RAM minimally and also which CPU supports SSE2 technology.


## Is having WoW (World of Warcraft) server illegal?

Yes, It is because server core just manipulates and uses game content which was gathered from official this game servers and game client and also it breaks some WoW authorised EULA license paragraphs. 

## Is having core sources and using compiled binaries for personal (not for public) breaks any rules?

No, since Its open source and you use it only for personal. Personal means - you do not use server binaries for public connections (where possible players could connect to your server). 
We do not support any public live servers.
