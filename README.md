IUP all in one for mingw
=====

IUP http://webserver2.tecgraf.puc-rio.br/iup/ 是一个跨平台的 GUI 库，对 Lua 支持非常好。

不过它编译起来很麻烦，编译好的版本文件很多，为了自己方便，我创建了这个适用于 mingw 的 Makefile 。

先从 svn 仓库获取 IUP 源码（我使用的 Revision: 4360 ）：

```
svn co https://svn.code.sf.net/p/iup/iup/trunk/iup iup
```

然后在 iup 目录下 clone 本仓库

```
git clone https://github.com/cloudwu/iupmingw.git mingw
```

保证安装了 lua ，确认 Makefile 中设置了正确 lua 的 include lib bin 路径，最后 make 。

你也可以在 https://github.com/cloudwu/iupmingw/releases 下载我编译好的版本。

生成 iup.exe 后，就可以玩了：

```
iup simplenotepad.lua
```
