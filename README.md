# EasyWoW/wotlk-core ![alt text](https://www.gnu.org/graphics/agplv3-88x31.png "AGPL v3 license")

## Important links:

Core wiki: https://github.com/EasyWoW/wiki/wiki
[![Issue Stats](http://www.issuestats.com/github/EasyWoW/wotlk-core/badge/pr)](http://www.issuestats.com/github/EasyWoW/wotlk-core) [![Issue Stats](http://www.issuestats.com/github/EasyWoW/wotlk-core/badge/issue)](http://www.issuestats.com/github/EasyWoW/wotlk-core)

## Build Status:
| Compiler | Status |
|-----------|------------------|
| GCC/Clang | [![GCC/Clang build Status](https://travis-ci.org/EasyWoW/wotlk-core.svg?branch=master)](https://travis-ci.org/EasyWoW/wotlk-core) |
| MSVC 2013 | [![MSVC 2013 build status](https://ci.appveyor.com/api/projects/status/c3529dgugyyqildt?svg=true)](https://ci.appveyor.com/project/sanctum32/wotlk-core) |

[![Coverity static code analysis scan](https://scan.coverity.com/projects/4872/badge.svg)](https://scan.coverity.com/projects/4872) |



## What is EasyWoW

EasyWoW is World of Warcraft server engine based on arcemu (http://github.com/arcemu/arcemu, http://www.arcemu.org). This fork was made due original development has been ceased (they call it hibernation) and by wiling to 
add new features. 
This framework is ready to use under all currently popular operation systems - UNIX, OS X, Linux, Windows.
Its free to use, fork or use core commits (with authority information) and do any other things which do not breaks GNU AFFERO GENERAL PUBLIC LICENSE Version 3 license.

Main goals for this core is simplicity, performance and functionality - we and our contributors work very hard to combine all of them into modern code. We, EasyWoW aim as friendly community, so feel free to give 
questions or documentation requests (NOTE: do not forget to add correct label - "Question" or "Suggestion for wiki" ).

## Required tools to run this server

+ Game client (this repository supports only 3.3.5a 12340 build).
+ Compiler:
    + Linux/UNIX: GCC ( 4.6 or later) and Clang ( 3.2 or later)
    + Windows: Visual Studio (Community 2013 or later), mingw currently has no support.
+ SQL data server (MySQL 5.4 or later, PostgreSQL will supported in near future).
+ Git (source control)
+ Computer with 2-3 GB RAM minimally and also which CPU supports SSE2 technology.
