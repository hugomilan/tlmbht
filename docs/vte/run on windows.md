---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: page_eqAMS_Disqus
title: Running TLMBHT on Windows
permalink: conf/tlmbht-windows.html
---

<span style="color:#697473">Jan 31, 2017</span> by [**Hugo Milan**](https://hugomilan.github.io/)

The tests made here were performed on Win 10 64bits PCs. The procedure should be similar for other versions of Windows.

First, you need to know if your version of Windows have **Command Prompt** or **PowerShell**. 

### Which one do I have?

You may type **cmd** in the Windows search and see if it shows up. If it does not shows up, you may type **PowerShell**.

Here is a PC with **Command Prompt**.
![PC with Command Prompt]({{ site.baseurl }}/assets/images/vte/Win_cmd_search.png "PC with Command Prompt")

Here is a PC with **PowerShell**.
![PC with Command Prompt]({{ site.baseurl }}/assets/images/vte/Win_PS_search.png "PC with PowerShell")

Alternatively, you may use the shortcut *Windows + R* to open the Run and try to execute the Command Prompt with **cmd**. If it does not shows up, you may try in the Run **PowerShell**.

Here is a PC with **Command Prompt**.
![PC with Command Prompt]({{ site.baseurl }}/assets/images/vte/Win_cmd_run.png "PC with Command Prompt")

Here is a PC with **PowerShell**.
![PC with Command Prompt]({{ site.baseurl }}/assets/images/vte/Win_PS_run.png "PC with PowerShell")

Lastly, you may try the shortcuts *Windows + X* then *C* to run the **Command Prompt**. Or, for **PowerShell**, *Windows + X* then *I*.

### How to execute tlmbht from Command Prompt/PowerShell

Here, I assume that you already know if you have **Command Prompt** or **PowerShell** and knows how to open it. To our purposes, the command we will use are similar in **Command Prompt** and **PowerShell**, then, I will refer to them as **terminal** and will show basic navigation commands.

Let's suppose that you downloaded the binary (executable) file for Windows 64bits ([you can get it here](https://github.com/hugomilan/tlmbht/releases)), you are going through the [validation of the Pennes equation in 1D]({{ site.baseurl }}{% link vte/validation/pennes 1D line.md %}) and downloaded the case file and the mesh file ([you can get them here](https://github.com/hugomilan/tlmbht/tree/master/vte/generalDiffusion/1D/LineNode)) in your Download folder. Simply type `cd downloads` in the terminal to get to the downloads folder.

![PC with Command Prompt]({{ site.baseurl }}/assets/images/vte/Win_PS_cd.png "Going to the Downloads folder using PowerShell")

Inside the folder, simply type `tlmbhtWin64.exe cpennes1Li.tlm` to execute the software.
![PC with Command Prompt]({{ site.baseurl }}/assets/images/vte/Win_cmd_running.png "Running TLMBHT With Command Prompt")

And this is the final output of the software.
![PC with Command Prompt]({{ site.baseurl }}/assets/images/vte/Win_cmd_have_run.png "TLMBHT outputs in the Command Prompt")

The data calculated by tlmbht, in this case, was saved in the file *cpennes1Li.m* that you can see in the Download folder.


### Summary of useful commands

* `cd folderName` go to the folder *folderName*.
* `cd ..` go to the parent directory. Example: type `cd` go from the directory `C:\User\YourUserName\Downloads` to `C:\User\YourUserName`.
* Windows names are case-insensitivity. `cd Downloads` and `cd downloads` you take you to the same folder.
* You can use the key Tab to get auto-completion of the folder name, file, or software. Example: `cd Down` then press Tab will yield `cd Downloads`.
* Type `help` to see other possible commands.


Now, you can go to:

* [Tutorials, examples, and validations]({{ site.baseurl }}{% link vte/index.md %})
