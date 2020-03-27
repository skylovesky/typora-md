### Git 常用操作命令收集：

#### 1) 远程仓库相关命令

检出仓库：$ **git** clone **git**://github.com/jquery/jquery.**git**

查看远程仓库：$ **git** remote -v

添加远程仓库：$ **git** remote add [name] [url]

删除远程仓库：$ **git** remote rm [name]

修改远程仓库：$ **git** remote set-url --**push**[name][newUrl]

拉取远程仓库：$ **git** pull [remoteName] [localBranchName]

推送远程仓库：$ **git push** [remoteName] [localBranchName]

 

#### 2）分支(branch)操作相关命令

查看本地分支：$ **git** branch

查看远程分支：$ **git** branch -r

创建本地分支：$ **git** branch [name] ----注意新分支创建后不会自动切换为当前分支

切换分支：$ **git** checkout [name]

创建新分支并立即切换到新分支：$ **git** checkout -b [name]

删除分支：$ **git** branch -d [name] ---- -d选项只能删除已经参与了合并的分支，对于未有合并的分支是无法删除的。如果想强制删除一个分支，可以使用-D选项

合并分支：$ **git** merge [name] ----将名称为[name]的分支与当前分支合并

创建远程分支(本地分支**push**到远程)：$ **git push** origin [name]

删除远程分支：$ **git push** origin :heads/[name]

我从master分支创建了一个issue5560分支，做了一些修改后，使用**git push** origin master提交，但是显示的结果却是'Everything up-to-date'，发生问题的原因是**git push** origin master 在没有track远程分支的本地分支中默认提交的master分支，因为master分支默认指向了origin master 分支，这里要使用**git push** origin issue5560：master 就可以把issue5560推送到远程的master分支了。

   如果想把本地的某个分支test提交到远程仓库，并作为远程仓库的master分支，或者作为另外一个名叫test的分支，那么可以这么做。
$ origin test:master     // 提交本地test分支作为远程的master分支 //好像只写这一句，远程的github就会自动创建一个test分支$ origin test:test       // 提交本地test分支作为远程的test分支

$ origin :test       // 刚提交到远程的test将被删除，但是本地还会保存的，不用担心

#### 3）版本(tag)操作相关命令

查看版本：$ **git** tag

创建版本：$ **git** tag [name]

删除版本：$ **git** tag -d [name]

查看远程版本：$ **git** tag -r

创建远程版本(本地版本**push**到远程)：$ **git push** origin [name]

删除远程版本：$ **git push** origin :refs/tags/[name]

 

#### 4) 子模块(submodule)相关操作命令

添加子模块：$ **git** submodule add [url] [path]

如：$ **git** submodule add **git**://github.com/soberh/ui-libs.**git** src/main/webapp/ui-libs

初始化子模块：$ **git** submodule init ----只在首次检出仓库时运行一次就行

更新子模块：$ **git** submodule update ----每次更新或切换分支后都需要运行一下

删除子模块：（分4步走哦）

1)$ **git** rm --cached [path]

2) 编辑“.gitmodules”文件，将子模块的相关配置节点删除掉

3) 编辑“.**git**/config”文件，将子模块的相关配置节点删除掉

4) 手动删除子模块残留的目录

 

#### 5）忽略一些文件、文件夹不提交

在仓库根目录下创建名称为“.gitignore”的文件，写入不需要的文件夹名或文件，每个元素占一行即可，如

target

bin

*.db

### SSH秘钥连接Githug

#### 设置git的user name和email

```cmake
git config --global user.name "Luke.Deng"
git config --global user.email  "xiangshuo1992@gmail.com"
```

#### 检查是否存在SSH Key

```cmake
cd ~/.ssh
ls
或者
ll
看是否存在 id_rsa 和 id_rsa.pub文件，如果存在，说明已经有SSH Key
```

#### 创建一对新的SSH密钥

```cmake
$ssh-keygen -t rsa -C "your_email@example.com"
#这将按照你提供的邮箱地址，创建一对密钥
Generating public/private rsa key pair.
Enter file in which to save the key (/c/Users/you/.ssh/id_rsa): [Press enter]
```

> 直接回车，则将密钥按默认文件进行存储。此时也可以输入特定的文件名，比如/c/Users/you/.ssh/github_rsa;接着，根据提示，你需要输入密码和确认密码（说到这里，如果你很放心，其实可以不用密码，就是到输密码的地方，都直接回车，所以每次push就只管回车就行了。所谓的最安全的密码，就是没有密码  – –  哈哈）

#### 在GitHub账户中添加你的公钥

 id_rsa.pub 

#### 测试

> 为了确认我们可以通过SSH连接GitHub，我们输入下面命令。输入后，会要求我们提供验证密码，输入之前创建的密码就ok了。 

```cmake
$ssh -T git@github.com
```

 你可能会看到告警信息，如下： 

```cmake
The authenticity of host 'github.com (207.97.227.239)' can't be established.
RSA key fingerprint is 16:27:ac:a5:76:28:2d:36:63:1b:56:4d:eb:df:a6:48.
Are you sure you want to continue connecting (yes/no)?
```

不用担心，直接输入yes。

如果看到下面信息，就说明一切完美！

Hi username! You’ve successfully authenticated, but GitHub does not provide shell access.