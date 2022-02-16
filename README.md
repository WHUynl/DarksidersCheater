# DarksidersCheater 学习笔记 1.无限灵魂 2.敌人爆炸
#### 1.懂得了使用多线程来检测控件的状态，防止程序阻塞，了解到重要的HANDLE 句柄
#### 2.学习到了使用VS的SPY++来获取窗体的名字以及其类名，从而获得窗体的HANDLE，更进一步通过窗体HANDLE获得窗体的使用OpenProcess获取整个进程内存的读写权限（此处也是进程间的通信）
#### 3.使用::ReadProcessMemory 和 ::WriteProcessMemory实现了对进程内存的修改，从而实现了游戏外挂。
#### 4.初步使用了反汇编软件 x64dbg 以及 查看内存的 CE
