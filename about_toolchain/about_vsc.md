查看默认终端：设置中搜索`terminal.integrated.defaultProfile.window`  ,,null的话默认是POWERSHELL



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