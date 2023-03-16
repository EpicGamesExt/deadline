.. Copyright Epic Games, Inc. All Rights Reserved.

=======================
Movie Pipeline Deadline
=======================

The Unreal Engine's cinematic sequence rendering system, called **Movie Render Queue**,
enables users to output rendered sequence frames or movies to disk.
For more information about **Movie Render Queue**, please refer to the `Movie Render Queue <https://docs.unrealengine.com/5.1/en-US/render-cinematics-in-unreal-engine/>`_ documentation.

This section explains how to use the **Movie Pipeline Deadline** plugin to submit cinematic render jobs to a deadline repository.

.. warning::
    This documentation assumes that you have enabled the **Deadline Service** plugin
    and created a **Deadline Preset Library** for job submissions. If you have not, it is recommended that you read the documentation first before proceeding.

Enabling the Plugin
-------------------

To enable the Movie Pipeline Deadline plugin:

#. From the main menu, select **Edit > Plugins** to open the plugin browser.
#. Search for **Movie Pipeline Deadline** in the search dialog.
#. Enable the plugin and restart the Editor.

.. note::
    If you cannot find the plugin, ensure that the plugin files are located under **<EngineVersion>/Engine/Plugins**,
    any custom subdirectory in the **Engine/Plugins** folder or **<CustomProjectDirectory>/Plugins**. Also, make sure
    that the plugin is compiled for the current engine version. The Editor
    will notify you on the next launch if the plugin has not been compiled and will suggest that you compile
    the plugin from source.

Setting Project Settings
------------------------

These settings are optional and are typically used when you want to set a
project wide default configuration.

**Default Preset Library**: This accepts a Deadline Data Asset Preset Library
and enables you to set a project-wide preset library.

**Default Preset Name**: This is the name of the preset to use from the
project's default preset library.

We need to update the `Movie Render Queue` project settings to use
executors defined by the Movie Pipeline Deadline plugin.

.. note::
    Make sure that the **Movie Render Queue** plugin is enabled on the project.

To update the project settings for **Movie Render Queue**:

#. From the main menu, select **Edit > Project Settings** to open the project settings browser.
#. Search for **Movie Render Pipeline** in the search browser.
#. In the **Default Remote Executor** option, search for *MoviePipelineDeadlineRemoteExecutor* and set the option. This setting determines how a job is submitted when the **Render Remote** button clicked.
#. In the **Default Executor Job** option, search for *MoviePipelineDeadlineExecutorJob* and set the option. This setting augments the details panel in the Movie Render Queue UI with our custom Deadline job definition.

Submitting a Deadline Job
-------------------------

To submit a Deadline job, you have two options:
    - Shot-By-Shot submission
    - Queue submission

To submit a shot-by-shot based Deadline Job:

#. Open the Movie Render Queue (MRQ) user interface by selecting **Window > Cinematics > Movie Render Queue** from the main menu.
#. Add a sequence to the job queue.
#. In the details panel under **Deadline**, set the Batch Name for the render job.
#. Set the preset Library to the Data Asset you created for sequence rendering. If you leave this blank, the default project Preset Library will be used.
#. Set the preset name you wish to use from the Preset Library. This is a required setting when using the Preset Library.
#. Optionally, you can override the output location and file name format for the current job.

To submit a queue based Deadline Job:

#. Open the Queue Asset submitter by selecting **Deadline > Submit Movie Render Queue Asset** from the main menu.
#. Set the Batch Name similar to the shot-by-shot workflow.
#. Set the Preset Library to the Data Asset you created for sequence rendering.
#. Set the submission and remote preset library name. The *submission preset name* represents the preset that is going to be used by the submitter and the *remote preset name* represents the preset used by the remote process.

    .. note::
        For a detailed explanation on these settings, see the `Demo`_ video below.

Demo
----

This `video <https://epicgames.box.com/s/oa0rs2oayvfvpzqzvn1hv2dzc5yjf03q>`_ demonstrates how to enable the plugin and submit render
jobs using a shot-by-shot or queue rendering workflow. Below is the timestamp for the video.

Timestamps
-----------

`00:00:00`   -  Introduction

`00:00:25`   -  Meerkat Demo Project

`00:01:15`   -  Enable Movie Pipeline Deadline plugin

`00:02:05`   -  What is a Deadline Data Asset Preset Library

`00:03:32`   -  How to create a Deadline Data Asset

`00:03:55`   -  What is a Deadline Data Asset Preset

`00:05:30`   -  Set Deadline Job Info Settings

`00:07:48`   -  Set Deadline Plugin info Settings

`00:09:40`   -  Setup Movie Pipeline Deadline Project Settings

`00:12:50`   -  Set Movie Render Pipeline Project Settings

`00:13:30`   -  Set Movie Render Pipeline Remote Executor Project Settings

`00:13:45`   -  Set Movie Render Pipeline Executor Job Project Settings

`00:14:22`   -  Open Movie Render Pipeline UI

`00:14:35`   -  How to Submit a Movie Render Pipeline Job to Deadline

`00:15:26`   -  Set Movie Render Pipeline Deadline Job Settings

`00:17:38`   -  Submit Movie Render Pipeline Job to Deadline

`00:17:55`   -  Job rendering on Deadline Monitor

`00:20:30`   -  Movie Render Pipeline Queue Rendering on Deadline

`00:22:11`   -  Submit Movie Render Pipeline Queue to Deadline

`00:24:11`   -  Deadline Monitor Movie Render Pipeline Queue Render

|

API Reference
-------------

CommandLine Interface
~~~~~~~~~~~~~~~~~~~~~

.. automodule:: mrq_cli
   :members:
   :undoc-members:
   :show-inheritance:

.. automodule:: mrq_cli_modes
   :members:
   :undoc-members:
   :show-inheritance:

Remote Procedure Call (RPC)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. automodule:: mrq_rpc
   :members:
   :undoc-members:
   :show-inheritance:
