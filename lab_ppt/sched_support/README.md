# 调度实验配套文件

两个 C 文件根据本次会话中完整读取的“替换代码.zip”内容恢复，仅统一换行为 LF。
该目录保留备份，避免项目根目录的 `make clean` 删除 ZIP 后丢失资料。
原压缩包的 Makefile 未恢复，项目保留现有 Makefile 并加入等效的测试程序开关。

- 当前分支：sched，基于已完成的 lock 实验。
- 状态观察阶段：`make CPUS=1 qemu`。
- 自行实现 setpri() 的内核及用户态接口后：`make CPUS=1 SCHEDTEST=1 qemu`。
- 在默认 scheduler 下先运行 setpritest、schedtest 1 1、schedtest 1 2，保存基线。
- 然后修改 scheduler，再用相同命令和负载对比。
- SCHEDTEST 默认关闭，避免尚未实现 setpri() 时编译失败。

实验实现不包含在配套文件中。
