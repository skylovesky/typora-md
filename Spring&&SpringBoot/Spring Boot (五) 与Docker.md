# Docker

[docker中文]: http://www.docker.org.cn/page/resources.html



Docker是一个开源的应用容器引擎，基于Go语言并遵从Apache2.0协议开源。



Docker支持将软件编译成一个镜像；然后在镜像中各种软件做好配置，发布出去；

运行中的镜像称之为容器。



## Docker的核心概念

- docker镜像（Images）：Docker镜像是用于创建Docker容器的模板。

- docker容器（Container）：容器是独立运行的一个或一组应用。

- docker客户端（Client）:客户端通过命令行或者其他工具使用Docker与Docker的守护进程通信。

- docker主机（Host）：一个物理或者虚拟的机器用于执行Docker守护进程和容器。

- docker仓库（Registry）：Docker仓库用来保存镜像。

  [Docker hub]: https://hub.docker.com）	"提供了庞大镜像集合"

  