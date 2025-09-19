### 终端设置

查看默认终端：设置中搜索`terminal.integrated.defaultProfile.window`  ,,null的话默认是POWERSHELL



### 预设变量

## [Predefined variables](https://code.visualstudio.com/docs/reference/variables-reference#_predefined-variables)

The following predefined variables are supported:

| Variable                       | Description                                                  |
| :----------------------------- | :----------------------------------------------------------- |
| **${userHome}**                | Path of the user's home folder                               |
| **${workspaceFolder}**         | Path of the folder opened in VS Code                         |
| **${workspaceFolderBasename}** | Name of the folder opened in VS Code without any slashes (/) |
| **${file}**                    | Currently opened file                                        |
| **${fileWorkspaceFolder}**     | Currently opened file's workspace folder                     |
| **${relativeFile}**            | Currently opened file relative to `workspaceFolder`          |
| **${relativeFileDirname}**     | Currently opened file's dirname relative to `workspaceFolder` |
| **${fileBasename}**            | Currently opened file's basename                             |
| **${fileBasenameNoExtension}** | Currently opened file's basename with no file extension      |
| **${fileExtname}**             | Currently opened file's extension                            |
| **${fileDirname}**             | Currently opened file's folder path                          |
| **${fileDirnameBasename}**     | Currently opened file's folder name                          |
| **${cwd}**                     | Task runner's current working directory upon the startup of VS Code |
| **${lineNumber}**              | Currently selected line number in the active file            |
| **${columnNumber}**            | Currently selected column number in the active file          |
| **${selectedText}**            | Currently selected text in the active file                   |
| **${execPath}**                | Path to the running VS Code executable                       |
| **${defaultBuildTask}**        | Name of the default build task                               |
| **${pathSeparator}**           | Character used by the operating system to separate components in file paths |
| **${/}**                       | Shorthand for **${pathSeparator}**                           |

Suppose that you have the following conditions:

1. A file located at `/home/your-username/your-project/folder/file.ext` opened in your editor;
2. The directory `/home/your-username/your-project` opened as your root workspace.

This leads to the following values for each of the variables:

- **${userHome}**: `/home/your-username`
- **${workspaceFolder}**: `/home/your-username/your-project`
- **${workspaceFolderBasename}**: `your-project`
- **${file}**: `/home/your-username/your-project/folder/file.ext`
- **${fileWorkspaceFolder}**: `/home/your-username/your-project`
- **${relativeFile}**: `folder/file.ext`
- **${relativeFileDirname}**: `folder`
- **${fileBasename}**: `file.ext`
- **${fileBasenameNoExtension}**: `file`
- **${fileExtname}**: `.ext`
- **${fileDirname}**: `/home/your-username/your-project/folder`
- **${fileDirnameBasename}**: `folder`
- **${lineNumber}**: line number of the cursor
- **${columnNumber}**: column number of the cursor
- **${selectedText}**: text selected in your code editor
- **${execPath}**: location of Code.exe
- **${pathSeparator}**: `/` on macOS or linux, `\` on Windows





### Intellisense设置

主要是应对intellisense会在一些路径之类的地方上画波浪线表示找不到该头文件或变量，但是实际上编译是能过的，这说明只是intellisense自己找不到路径

更改intellisense设置：

![image-20250917133739457](assets\image-20250917133739457.png)

根据自己的语言选择intellisense的编译器

![image-20250917133814227](assets\image-20250917133814227.png)

或查看`.vscode\c_cpp_properties.json`的有关配置，因为这里的intellisense其实是C/C++扩展里面的功能，由其相关的json文件配置