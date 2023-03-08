.. Copyright Epic Games, Inc. All Rights Reserved.

====================
Unreal Engine Plugin
====================

In this section, we will discuss the main behaviors of the plugin and how it achieves what is listed in the :ref:`Design philosophy <design>`.

* Unreal Engine job type:
   There is only one Unreal Engine plugin to serve different job types. We designed it this way for maintainability reasons.
   We didn't want to write multiple different plugins for the nearly infinite job types the editor could execute.
   To accomplish this, the plugin assumes the command-line interface and the process that's executed on startup knows how to start, execute, and shut down the editor.
   This concept is based on the automation framework already built into the editor.
   See :ref:`Commandline Mode <commandline_mode>` to learn how the editor determines the success or failure of a task.
   This behavior allows us to have a plugin that is very lean, easy to maintain, and supports almost all Unreal jobs.

* Reproducibility:
   In production, errors can occur while processing a remote job. It is essential for engineers and QA to replicate errors locally.
   This allows them to test and apply fixes to the project. To accomplish this, the plugin needs to have a simple interface for executing jobs.
   This interface should allow an engineer to copy and paste the command locally and reproduce the same results effectively.
   Allowing the command-line arguments to handle the heavy lifting enables this behavior.

* Explicit binary location:
   Most studios have a build system that compiles the editor binaries and makes them available for use through means like a network storage.
   To make the plugin agnostic to build systems and launcher binaries, the plugin expects the full path to the binary location.
   This removes any implicit assumptions on where a binary is located from the plugin. However, to support dynamic build system deployments without
   a user explicitly knowing where the project lives on the remote machine, it assumes a pre-process step sets a custom environment variable called ``ProjectRoot``.
   If the user adds this in the executable path, the remote system will resolve the environment variable and return the full path to the executable.
   This gives projects the option of specifying stable build binaries as well as hooking into a pre-process build step.

* Multiple tasks per Editor instance or one task per Editor Instance:
   Depending on the size of a project, you can experience Editor startup times between a couple of minutes to several minutes (we have seen a 5-minute startup time).
   Instead of launching the Editor during this time, it could be used to process a job. To enable the stated options, the plugin offers two modes: classical
   command-line mode, where task success is determined by the exit code, and `RPC <https://docs.python.org/3/library/xmlrpc.html>`_ protocol mode, where
   a single Deadline process can execute multiple tasks without shutting down between them.

    * Classical mode:
       The plugin relies on the Editor exiting with exit code 0 to determine success and other codes for errors.

       .. warning::

            If your automation does not close the editor after processing, the deadline process has no way of knowing if the task is complete.
            You have to make sure you exit the editor with the appropriate code for Deadline to respond accordingly.

    * RPC mode:
       The Deadline process is the server, and the Editor instance is the client. The server exposes certain basic Deadline functionalities to
       the clients. For example, you can get the current user, task ID, environment variables, job details, and other repository options through the RPC protocol
       (See :ref:`RPC API <deadline_rpc_api>` for the options exposed on the RPC proxy).When a job is picked up by a worker, Deadline creates a random port and stores
       this port in an environment variable called ``DEADLINE_RPC_PORT``. The client is expected to "connect" to the process via the proxy. This notifies Deadline about a
       client connection and starts an infinite loop that only exits when the client tells the server a task is completed. The Deadline process will only shut down when
       all tasks in a job have been satisfied. This is a general behavior of Deadline, and the plugin tries not to manage it.

       .. note::

         This mode expects the client to notify the server when a task has completed or failed, and Deadline will respond to the behavior accordingly


The plugin's design has enabled us to achieve backward and forward compatibility across Unreal versions while also being maintainable.

.. seealso::

    :ref:`Deadline Unreal Python API <deadline_python_api>`

