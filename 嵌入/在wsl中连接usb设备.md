# 连接 USB 设备

本指南将演练使用 USB/IP 开源项目 [usbipd-win](https://github.com/dorssel/usbipd-win) 将 USB 设备连接到在 WSL 2 上运行的 Linux 发行版所必要的步骤。

在 Windows 计算机上设置 USB/IP 项目将启用常见的开发人员 USB 场景，例如刷写 Arduino 或访问智能卡读取器。

## 先决条件
- 运行Windows 11 (版本 22000 或更高版本) 。 （可提供 Windows 10 支持，请参见下面的注释）
- 需要具有 x64/x86 处理器的计算机。 usbipd-win) 目前不支持 (Arm64。
- Linux 发行版已安装并设置为 WSL 2。
- 运行 Linux 内核 5.10.60.1 或更高版本。  

    备注  
    若要检查 Windows 版本及内部版本号，选择 Windows 徽标键 + R，然后键入“winver”，选择“确定” 。 可通过选择“开始”>“设置”>“Windows 更新”>“检查更新”来更新到最新的 Windows 版本。 若要检查 Linux 内核版本，请打开 Linux 发行版并输入命令：`uname -a`。 若要手动更新到最新内核，请打开 PowerShell 并输入命令：“`wsl --update`”。

    重要  
    建议使用 Windows 11 将 USB 设备连接到在 WSL 2 上运行的 Linux 发行版。 但是，按照 USBIPD-WIN 项目报告中的说明，通过构建你自己的支持 USBIP 的 WSL 2 内核，可使用 Windows 10 连接 USB 设备。

## 安装 USBIPD-WIN 项目
WSL 本身并不支持连接 USB 设备，因此你需要安装开源 usbipd-win 项目。

1. 转到 [usbipd-win 项目的最新发布页。](https://github.com/dorssel/usbipd-win/releases)
2. 选择 .msi 文件，该文件将下载安装程序。 （你可能会收到一条警告，要求你确认你信任此下载）。
3. 运行下载的 usbipd-win_x.msi 安装程序文件。  
   
    备注  
    或者，也可使用 Windows 程序包管理器程序 (winget) 来安装 usbipd-win 项目。 如果已安装 winget，只需使用命令 `winget install --interactive --exact dorssel`.usbipd-win 安装 usbipd-win 即可。 如果你省略了 --interactive，winget 可能会立即重启计算机（如果这是安装驱动程序所必需的操作）。

这将安装：  
名为 `usbipd` 的服务（显示名称：USBIP 设备主机）。 可使用 Windows 中的“服务”应用检查此服务的状态。
命令行工具 `usbipd`。 此工具的位置将添加到 PATH 环境变量。
名为 `usbipd` 的防火墙规则，用于允许所有本地子网连接到服务。 可修改此防火墙规则以微调访问控制。

## 在 Linux 中安装 USBIP 工具和硬件数据库

USB/IP 项目完成安装后，需要安装用户空间工具和 USB 硬件标识符数据库。 这些说明适用于 Ubuntu ， [其他分发版可能需要不同的 usbip 客户端包。](https://github.com/dorssel/usbipd-win/wiki/WSL-support#usbip-client-tools)

在 Ubuntu 上运行以下命令：
```dotnetcli
<!-- Bash -->

sudo apt install linux-tools-generic hwdata

sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/*-generic/usbip 20
```

此时，一个服务正在 Windows 上运行以共享 USB 设备，并且在 WSL 中安装了必要的工具来连接到共享设备。

### 附加 USB 设备
在附加 USB 设备之前，请确保 WSL 命令行已打开。 这将使 WSL 2 轻型 VM 保持活动状态。

1. 通过以管理员模式打开 PowerShell 并输入以下命令，列出所有连接到 Windows 的 USB 设备：
```dotnetcli
<!-- PowerShell -->

usbipd wsl list
```
2. 选择要附加到 WSL 的设备总线 ID，然后运行此命令。 WSL 会提示你输入密码以运行 sudo 命令。 要附加的 Linux 分发版必须是默认分发版。 (请参阅 WSL 的基本文档 ，更改默认分发) 。

```dotnetcli
<!-- PowerShell -->

usbipd wsl attach --busid <busid>
```
3. 打开 Ubuntu（或首选的 WSL 命令行），使用以下命令列出附加的 USB 设备：
```dotnetcli
<!-- Bash -->
lsusb
``````
你应会看到刚刚附加的设备，并且能够使用常规 Linux 工具与之交互。 根据你的应用程序，你可能需要配置 udev 规则以允许非根用户访问设备。

4. 在 WSL 中完成设备使用后，可物理断开 USB 设备，或者在管理员模式下从 PowerShell 运行此命令：
```dotnetcli
<!-- PowerShell -->

usbipd wsl detach --busid <busid>
``````
若要详细了解此操作的工作原理，请参阅 [Windows 命令行博客](https://devblogs.microsoft.com/commandline/connecting-usb-devices-to-wsl/#how-it-works)和 [GitHub 上的 usbipd-win 存储库。](https://devblogs.microsoft.com/commandline/connecting-usb-devices-to-wsl/#how-it-works)
