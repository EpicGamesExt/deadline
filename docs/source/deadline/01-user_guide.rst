.. Copyright Epic Games, Inc. All Rights Reserved.

==========
User Guide
==========

This guide discusses the plugin's expectations and how to submit jobs.

The following section outlines the available options for the plugin:

.. _executable:

* **Executable**:
  This option specifies the full path to the Editor executable, typically named **UnrealEditor-Cmd**.  The *Cmd* executable allows attaching and piping
  the log outputs to the current spawned process. The plugin resolves any Deadline Path Mappings and a custom key ``{ProjectRoot}`` in the executable path.
  ``{ProjectRoot}`` allows users to specify implicit project root locations provided in an environment variable.
  This option enables a preprocessing step like a source control sync/build system to specify the current project root for the executable.

* **ProjectFile**:
   This option specifies the full path to the Unreal *.uproject* file. The *.uproject* file specifies the project to load when the editor is launched.
   Similar to the executable option, the plugin resolves any ``{ProjectRoot}`` and Deadline Path Mappings in the *.uproject* path.

* **CommandLineArguments**:
   This option specifies the startup/launch command-line arguments, which determine the automated job/task that the editor is expected to execute.
   The plugin appends the following arguments to the supplied arguments:

        * ``-log``: Outputs the editor logs in the current process.
        * ``-unattended``: Disables anything requiring user feedback.

.. seealso::

    `Unreal Engine Command-Line Arguments <https://docs.unrealengine.com/5.1/en-US/command-line-arguments-in-unreal-engine/>`_

.. _commandline_mode:

* **CommandLineMode**:
    This option determines whether a new session of the Editor should be launched for every task or if one editor session should be used for all tasks.
    The former behaves similarly to a classical command-line process where the Editor's exit error code determines the success of the task.
    The latter relies on an internal Editor process to communicate the task success via an `RPC <https://docs.python.org/3/library/xmlrpc.html>`_ protocol.
    The default option is set to ``true``.

The following is an example snippet of the plugin options, which launches the editor and executes a custom Python script:

.. code-block:: python

    {
        "Executable": "{ProjectRoot}/Engine/Binaries/Win64/UnrealEditor-Cmd.exe",
        "ProjectFile": "{ProjectRoot}/MyCustomProject/MyCustomProject.uproject",
        "CommandLineArguments": '-execcmds="py mycustomscript.py arg0 arg1 --key=value"',
        "CommandLineMode": "true"
    }

|

Plugin options
++++++++++++++

All the available options on the Unreal Engine plugin are listed below:


.. code-block::

    [Executable]
    Type=String
    Label=Unreal Executable
    Category=Executable
    Index=0
    Description=Unreal Executable
    Required=true
    DisableIfBlank=false

    [LoggingDirectory]
    Type=string
    Label=Command Line Arguments
    Category=Executable
    Index=2
    Description=What command line arguments should be passed to the executable?
    Required=false
    DisableIfBlank=false

    [ProjectFile]
    Type=String
    Label=Project File
    Category=Project
    Index=0
    Description=The name of the .uproject file ("MyGame.uproject") within the Game folder.
    Required=true
    DisableIfBlank=false

    [CommandLineArguments]
    Type=string
    Label=Command Line Arguments
    Category=Project
    Index=1
    Description=What command line arguments should be passed to the executable?
    Required=false
    DisableIfBlank=false

    [CommandLineMode]
    Type=boolean
    Label=Command Line Mode
    Category=Project
    Index=1
    Description=Should the Editor process commands as commandline arguments
    Required=false
    DisableIfBlank=false
    Default=true

    [StartupDirectory]
    Type=string
    Label=Startup Directory
    Category=Command Line Options
    CategoryOrder=0
    Index=3
    Description=The directory to start the command line in (leave blank to use default)
    Required=false
    DisableIfBlank=true

